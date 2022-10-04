#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
//Chan Chun Wai 1155127382
//TSANG Ho San	1155127648
//Chan Cheung Hei 1155125806
int find_pipe1(char ** args, int argc)//return first pipe position. Return -1 if no.
{	int pipe1=-1;
	for(int i=0; i<argc-1; i++)
	{
	if ( strcmp(args[i], "|") == 0 )
	{
		pipe1=i;
		break;
	}	
	}
	return pipe1;
}

int find_pipe2(char ** args, int argc)//return second pipe position. Return -1 if no.

{	int pipe1=-1;
	int pipe2=-1;
	for(int i=0; i<argc-1; i++)
	{	
	if ( strcmp(args[i], "|") == 0 )
	{	if(pipe1==-1)
			pipe1=i;
		else 
		{
		pipe2=i;
		break;
		}	
	}	
	}
	return pipe2;
}

void copystring(char **args,char* dest[5], int start, int length)
{
	for(int i=start;i<start+length;i++)
    		dest[i-start]=args[i];
   	dest[length]=NULL;
    return;
}




int shell_execute(char ** args, int argc)
{	char p[100]="";
	int fd[2];
	int fd2[2];
	int child_pid,child_pid2,child_pid3, wait_return, status, pipecount, pipe1, pipe2;
	pipe1=find_pipe1(args,argc);//pipe1 position
	pipe2=find_pipe2(args,argc);
	
	if (pipe1==-1)
		pipecount=0;
	else if(pipe2==-1)
		pipecount=1;
	else pipecount=2;
	//pipecount indicate the number of pipe
	//printf("%d %d %d\n",pipe1,pipe2,pipecount);
        if ( strcmp(args[0], "EXIT") == 0 || strcmp(args[0], "exit") == 0 )
            return -1; 

if(pipe(fd) == -1) {
          perror("Pipe failed");
          exit(1);
        }
if(pipe(fd2) == -1) {
          perror("Pipe failed");
          exit(1);
        }
	//printf("%d %d %d %d\n",fd[0],fd[1],fd2[0],fd2[1]);
	for( int i = 0; i<=pipecount; i++){

////////////////////////////////////////////////////////////////////
	if (i==0)
	child_pid = fork();
	else if (i==1)
	child_pid2 = fork();
	else if (i==2)
	child_pid3 = fork();
/////////////////////////////////////////////////////////////////////
	if (child_pid==0)
		{//first command before pipe
		char *first[10];
		if (pipecount!=0)
		{
        
        close(1);
        dup(fd[1]);
	close (fd[0]);
        close (fd[1]);

				
            copystring(args,first,0,pipe1);
		}
		else
		{
            copystring(args,first,0,argc-1);
		}
		execvp(first[0], first);
		}else 
///////////////////////////////////////////////////////////////////
        if (child_pid2==0)
        {    //second command
            char *second[10];
            if (pipecount==2)
            {	close(fd[1]);
		close(0);
		dup(fd[0]);
		 close (fd[0]);
		close(fd2[0]);
		close(1);
		dup(fd2[1]);
		close (fd2[1]);
		
		
                copystring(args,second,pipe1+1,pipe2-pipe1-1);
		
            }
            if (pipecount==1)
            {close(0);
		dup(fd[0]);
            close (fd[0]);

                copystring(args,second,pipe1+1,argc-pipe1-1);
		//printf("%s %s %s\n",second[0],second[1],second[2]);
            }
		execvp(second[0], second);
		exit(1);
                
        }
 
//////////////////////////////////////////////////////////////////////
		else if (child_pid3==0)
		{//third command
			char *third[10];
			close(fd[0]);
            		close(fd[1]);
			close(fd2[1]);
			close(0);
			dup(fd2[0]);

            		close (fd2[0]);
			
			copystring(args,third,pipe2+1,argc-pipe2-1);
		execvp(third[0], third);
		exit(1);
		}
///////////////////////////////////////////////////////////////////////
		
		
/////////////////////////////////////////////////////////////////////////
else {		
	
	
	
	if ( (wait_return = wait(&status) ) < 0 )
		printf("wait() error \n");

	close(fd[1]);
	if(i==1)
	close(fd2[1]);
					//read(fd2[0],p,9);
			//printf(" testing %s\n",p);
	}
		}
	return 0;

}
