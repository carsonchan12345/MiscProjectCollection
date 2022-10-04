#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

struct deckinfo{
char poker[4];
int flip;
};



void cardshuffle (struct deckinfo deck[52])
{
int x;
char temp[4];
srand(time(NULL));

for (int i=0;i<=51;i++)
{
x=rand()%52;
strcpy(temp,deck[i].poker);
strcpy(deck[i].poker,deck[x].poker);
strcpy(deck[x].poker,temp);
}
}


void printing (struct deckinfo deck[52])
{

    for (int i=0;i<=51;i++){

    if (deck[i].flip==0)
        printf("xxx   ");
    else if (strlen(deck[i].poker)==2)
        printf("%c %c   ",deck[i].poker[0],deck[i].poker[1]);
        else if (strlen(deck[i].poker)==3)
        printf("%s   ",deck[i].poker);

    if (((i+1)%13)==0)
        printf("\n");
    }
    printf("\n\n");
}

void comparing (struct deckinfo deck[52],int x,int y,int *turns,int playerflip[2])
{
    if (deck[x].poker[1]==deck[y].poker[1])
    {   deck[x].flip=1;
        deck[y].flip=1;
        printf("Yeah match ar!\n");
        if (*turns%2==0)
            playerflip[1]+=2;
        else playerflip[0]+=2;

    }
        else
        {
            printf("Nel not match.\n");
        }
        *turns+=1;
}

void d2d2 (int r,int c,int *input)
{
    *input=r*13+c-1;


}



void comturn (struct deckinfo deck[52],int *turns,int playerflip[2],int mode)
{   int i=0;

int x,y;
if (playerflip[0]+playerflip[1]==52)
    return ;
    while (i==0){
    x=rand()%52;
    if (deck[x].flip==0)
        break;
    }
    if (mode==1)
    printf("Computer choose (%d, %d) and it is %s\n",x/13,x%13,deck[x].poker);
    else printf("Computer%d choose (%d, %d) and it is %s\n",((*turns+1)%2+1),x/13,x%13+1,deck[x].poker);


    while (i==0){
        y=rand()%52;
            if (deck[y].flip==0)
        {
            if (x!=y)
                break;
        }
    }
     if (mode==1)
    printf("Computer choose (%d, %d) and it is %s\n",y/13,y%13,deck[y].poker);
    else printf("Computer%d choose (%d, %d) and it is %s\n",((*turns+1)%2+1),y/13,y%13+1,deck[y].poker);

     comparing(deck,x,y,turns,playerflip);


}



int main()
{


int turns=1,mode;
int i=0;
int row,col,input1,input2;





do {
        struct deckinfo deck[52]={{"CA",0},{"C2"},{"C3"},{"C4"},{"C5"},{"C6"},{"C7"},{"C8"},{"C9"},{"C10"},{"CJ"},{"CQ"},{"CK"},{"DA"},{"D2"},{"D3"},{"D4"},{"D5"},{"D6"},{"D7"},{"D8"},{"D9"},{"D10"},{"DJ"},{"DQ"},{"DK"},{"SA"},{"S2"},{"S3"},{"S4"},{"S5"},{"S6"},{"S7"},{"S8"},{"S9"},{"S10"},{"SJ"},{"SQ"},{"SK"},{"HA"},{"H2"},{"H3"},{"H4"},{"H5"},{"H6"},{"H7"},{"H8"},{"H9"},{"H10"},{"HJ"},{"HQ"},{"HK"}};
        int playerflip[2]={0};     //total flipped card of two players
cardshuffle(deck);                 //shuffle cards
 turns=1;
        printf("********************************\n*Welcome to concentration game*\n\n********************************\nPlease choose a mode of the game!\n1. Human v.s. Computer\n2. Computer v.s. Computer\n0. Quit\nYour choice:\n");



        scanf("%d",&mode);
if (mode==1){
    while (playerflip[0]+playerflip[1]!=52){


while (i==0){
        printf("Enter your first row and col:\n");
scanf("%d %d",&row,&col);
    if ((row>=0 && row<=3) && (col>=1 && col<=13))
    {
        d2d2(row,col,&input1);
        if (deck[input1].flip==0)
            break;
        else printf("The card cant be flipped ar\n");
    }
      else printf("Row must within 0-3 and column within 1-13\n");
}
printf("Your first card is(%d,%d) and it is %s\n",row,col,deck[input1].poker);


while (i==0){
    printf("Enter your second row and col:\n");
scanf("%d %d",&row,&col);
    if ((row>=0 && row<=3) && (col>=1 && col<=13))
    {
        d2d2(row,col,&input2);
        if (deck[input2].flip==0)
        {
            if (input1!=input2)
             break;
             else printf("Should not pick first card\n");
        }


        else printf("The card cant be flipped ar\n");
    }
      else printf("Row must within 0-3 and column within 1-13\n");
}
printf("Your second card is(%d,%d) and it is %s\n",row,col,deck[input2].poker);

comparing(deck,input1,input2,&turns,playerflip);

printing(deck);

comturn(deck,&turns,playerflip,mode);



printf("Flipped card:%d; You : %d; Computer : %d;\n\n\n",playerflip[0]+playerflip[1],playerflip[0],playerflip[1]);

    }
    if (playerflip[0]>playerflip[1])
    printf("Player win\n");
else  if (playerflip[0]<playerflip[1])
    printf("Com win\n");
    else printf("Draw\n");


}







else if (mode==2)
{
    printing(deck);
    while (playerflip[0]+playerflip[1]!=52)
        {



        comturn(deck,&turns,playerflip,mode);
        comturn(deck,&turns,playerflip,mode);


printf("Flipped card:%d; Computer 1: %d; Computer 2: %d;\n\n\n",playerflip[0]+playerflip[1],playerflip[0],playerflip[1]);
    }
    printf("End Game\n\n\n");
    printing(deck);
printf("Turns:%d\n",turns);
if (playerflip[0]>playerflip[1])
    printf("Com1 win\n");
else  if (playerflip[0]<playerflip[1])
    printf("Com2 win\n");
    else printf("Draw\n");

}
else if (mode!=0)
    printf("You should enter 1, 2 or 0 only!\n\n");

}while (mode!=0);

    return 0;
}
