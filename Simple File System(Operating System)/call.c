#include "call.h"
#include <string.h>
#include <stdlib.h>
#include <math.h> 

const char *HD = "HD";
char dest[10][10]={""};
int splitpath(char *pathname);
int print_dir_mappings2(int fd, int i_number, int target);
int print_data(int fd, int i_number, int offset,void *buf, int count);


int open_t(char *pathname)
{	int fd = open (HD, O_RDWR);
	int inode_number;
	int number=-1;
	number=splitpath(pathname);
	if (number==0)
		return 0;
	for(int i=0;i<number;i++){
		if (i==0){
			inode_number=print_dir_mappings2(fd,0,i);
			}
		else
		{
			inode_number=print_dir_mappings2(fd,inode_number,i);
		}
		if (inode_number==-1)
			return -1;
	}
	//print_dir_mappings(HD,0,"dir5");
	return inode_number;
}


int read_t(int i_number, int offset, void *buf, int count)
{	int fd = open (HD, O_RDWR);
	int read_bytes;
	read_bytes=print_data(fd,i_number,offset,buf,count);
	return read_bytes; 
}

int splitpath(char *pathnametmp){//char test1[10]="/dir1"; char *test2[10];
    char pathname[30];
	strcpy(pathname,pathnametmp);
    if (strcmp(pathname,"/")==0)
        return 0;
	char *token=strtok(pathname, "/");
	int i=0;
   while( token != NULL ) {
      strcpy(dest[i],token);
      i++;
      token = strtok(NULL, "/");
   }
	return i;//return no. of level. 0=root, 1=first level and so on
}


inode* read_inode(int fd, int i_number){
	inode* ip = malloc(sizeof(inode));
	int currpos=lseek(fd, I_OFFSET + i_number * sizeof(inode), SEEK_SET);
	if(currpos<0){
		printf("Error: lseek()\n");
		return NULL;
	}
	
	//read inode from disk
	int ret = read(fd, ip, sizeof(inode));
	if(ret != sizeof (inode) ){
		printf("Error: read()\n");
		return NULL;
	}
	return ip;
}
int print_dir_mappings2(int fd, int i_number, int target)
{
	inode* ip;
	ip = read_inode(fd, i_number);
	if(ip->f_type != DIR)
	{
		printf("Wrong path!\n");
		return -1;
	}
	DIR_NODE* p_block = (DIR_NODE* )malloc(BLK_SIZE);
	// Consider that SFS only supports at most 100 inodes so that only direct_blk[0] will be used,
	// the implementation is much easier
	int blk_number = ip->direct_blk[0];
	int currpos=lseek(fd, D_OFFSET + blk_number * BLK_SIZE, SEEK_SET);
	read(fd, p_block, BLK_SIZE);

	int file_idx = 0;
	for(file_idx = 0; file_idx < ip->f_num; file_idx++)
	{	
		if (strcmp(p_block[file_idx].dir_name,dest[target])==0)
			return p_block[file_idx].i_number;
	}
	free(p_block);
	return -1;
}

int print_data(int fd, int i_number, int offset,void *buf, int count)
{	int tmpreturn;
	double tmp;
	int indirecttmp;
	int blk_number;
	int currpos;
	int count2=0;
	int f_size;
	inode* ip;
	ip = read_inode(fd, i_number);
	
	printf("ftype: %d\n",ip->f_type);
	f_size=ip->f_size;
	
	DIR_NODE* p_block = (DIR_NODE* )malloc(BLK_SIZE);
	// Consider that SFS only supports at most 100 inodes so that only direct_blk[0] will be used,
	// the implementation is much easier
	if (offset+count>MAX_FILE_SIZE)
	{
		printf("offset overflow!\n");
		return 0;
	}
	if (offset+count>f_size)
	{
		if (offset>f_size)
		{
			free(p_block);
			return 0;
		}
		else
		{
			count=f_size-offset;
		}
	}
	tmp=offset/4096.0;
	int start=(int)tmp;
	tmp=(offset+count-1)/4096.0;
	int end=(int)tmp;

	if (offset+count>MAX_FILE_SIZE)
	{
		printf("offset overflow!\n");
		return 0;
	}

	

	printf("Start: %d end: %d\n",start,end);
	if (start<2)
	{
		blk_number = ip->direct_blk[start];
		currpos=lseek(fd, D_OFFSET + (offset - start * BLK_SIZE) + blk_number * BLK_SIZE, SEEK_SET);
	}
	else
	{
		blk_number = ip->indirect_blk;
		currpos=lseek(fd, D_OFFSET  + blk_number * BLK_SIZE + (start-2) * sizeof(int) , SEEK_SET);
		read(fd, &indirecttmp, sizeof(int));
		
		currpos=lseek(fd, D_OFFSET  + (offset - start * BLK_SIZE) + indirecttmp * BLK_SIZE , SEEK_SET);
	}

	printf("Three block: %d %d %d\n",ip->direct_blk[0],ip->direct_blk[1],ip->indirect_blk);


	if ((offset - start * BLK_SIZE)+count<BLK_SIZE)
	{	printf("Byb\n");
		tmpreturn=read(fd, buf, count);
		free(p_block);
		return tmpreturn;
	}
	else
	{
		tmpreturn=read(fd, buf, BLK_SIZE-(offset - start * BLK_SIZE));//finish first block first
		count2+=BLK_SIZE-(offset - start * BLK_SIZE);
	}//before
	for (int i=start+1;i<=end;i++)
	{

			if (i<2)
			{
				blk_number = ip->direct_blk[i];
				currpos=lseek(fd, D_OFFSET + blk_number * BLK_SIZE, SEEK_SET);
			}
			else
			{
				blk_number = ip->indirect_blk;
				currpos=lseek(fd, D_OFFSET  + blk_number * BLK_SIZE + (i-2) * sizeof(int) , SEEK_SET);
				read(fd, &indirecttmp, sizeof(int));
				
				currpos=lseek(fd, D_OFFSET + indirecttmp * BLK_SIZE , SEEK_SET);
			}
			if (count-count2>=BLK_SIZE)
			{
			tmpreturn=read(fd,(char*) buf+count2, BLK_SIZE);
			if (indirecttmp==0)
				break;
			count2+=BLK_SIZE;
			}
			else
			{
			tmpreturn=read(fd,(char*) buf+count2, count-count2);
			if (indirecttmp==0)
				break;
			count2+=count-count2;
			}

	}

	free(p_block);
	return count2;
}



