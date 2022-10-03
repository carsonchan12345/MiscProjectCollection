#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void templateindex(float frequencyindex[]){
frequencyindex[0]=8.167/100;
frequencyindex[1]=1.492/100;
frequencyindex[2]=2.782/100;
frequencyindex[3]=4.253/100;
frequencyindex[4]=12.702/100;
frequencyindex[5]=2.228/100;
frequencyindex[6]=2.015/100;
frequencyindex[7]=6.094/100;
frequencyindex[8]=6.996/100;
frequencyindex[9]=0.153/100;
frequencyindex[10]=0.772/100;
frequencyindex[11]=0.4025/100;
frequencyindex[12]=2.406/100;
frequencyindex[13]=6.749/100;
frequencyindex[14]=7.507/100;
frequencyindex[15]=1.929/100;
frequencyindex[16]=0.095/100;
frequencyindex[17]=5.987/100;
frequencyindex[18]=6.327/100;
frequencyindex[19]=9.056/100;
frequencyindex[20]=2.758/100;
frequencyindex[21]=0.978/100;
frequencyindex[22]=2.36/100;
frequencyindex[23]=0.15/100;
frequencyindex[24]=1.974/100;
frequencyindex[25]=0.074/100;
}

void reading(FILE* cipher,char cipherdata[]){
fgets (cipherdata , 300, cipher);

}

  void wordsplit(char cipherdata[],char onee[],char twoo[],char threee[],char fourr[]){

       for(int i=0;i<66;i++){

            onee[i]=cipherdata[(4*i)];

            twoo[i]=cipherdata[(4*i)+1];

            threee[i]=cipherdata[(4*i)+2];

            fourr[i]=cipherdata[(4*i)+3];

   }

   }

void frequencycount(float count[][27],char cipherdata[],char onee[],char twoo[],char threee[],char fourr[]){


    for (int a=0;a<66;a++){
        int temp=(int)onee[a]-65;
        count[0][temp]=count[0][temp]+1;

    }
       for (int a=0;a<66;a++){
                int temp=(int)twoo[a]-65;
        count[1][temp]=count[1][temp]+1;


    }

        for (int a=0;a<66;a++){

        int temp=(int)threee[a]-65;
        count[2][temp]=count[2][temp]+1;

    }
        for (int a=0;a<66;a++){

        int temp=(int)fourr[a]-65;
        count[3][temp]=count[3][temp]+1;

    }

}


void normalized(float count[][27]){
for (int i=0;i<26;i++){
    printf("%c = %f\n",'A'+i,count[0][i]);
    count[0][i]=count[0][i]/66.0;

}
for (int i=0;i<26;i++){

    count[1][i]=count[1][i]/66;

}
for (int i=0;i<26;i++){
count[2][i]=count[2][i]/66;


}for (int i=0;i<26;i++){

    count[3][i]=count[3][i]/66;

};



}


void correlationcheck(float frequencyindex[],float count[][27],float correlation[][27]){






for (int i=0;i<26;i++){
    for(int a=0;a<26;a++){
            int temp=(a+i)%26;
    if (temp<0){
        temp=65+temp;
        correlation[0][i]=correlation[0][i]+(pow(count[0][temp]-frequencyindex[a]*66,2)/(frequencyindex[a]*66));


    }
     else if (temp>=0){
       correlation[0][i]=correlation[0][i]+(pow(count[0][temp]-frequencyindex[a]*66,2)/(frequencyindex[a]*66));


    }
    }

}

for (int i=0;i<26;i++){
    for(int a=0;a<26;a++){
            int temp=(a+i)%26;
    if (temp<0){
        temp=65+temp;
       correlation[1][i]=correlation[1][i]+(pow(count[1][temp]-frequencyindex[a]*66,2)/(frequencyindex[a]*66));

    }
     else if (temp>=0){
       correlation[1][i]=correlation[1][i]+(pow(count[1][temp]-frequencyindex[a]*66,2)/(frequencyindex[a]*66));
    }
    }

}
for (int i=0;i<26;i++){
    for(int a=0;a<26;a++){
            int temp=(a+i)%26;
    if (temp<0){
        temp=65+temp;
       correlation[2][i]=correlation[2][i]+(pow(count[2][temp]-frequencyindex[a]*66,2)/(frequencyindex[a]*66));
    }
     else if (temp>=0){
       correlation[2][i]=correlation[2][i]+(pow(count[2][temp]-frequencyindex[a]*66,2)/(frequencyindex[a]*66));
    }
    }

}
for (int i=0;i<26;i++){
    for(int a=0;a<26;a++){
           int temp=(a+i)%26;
    if (temp<0){
        temp=65+temp;
      correlation[3][i]=correlation[3][i]+(pow(count[3][temp]-frequencyindex[a]*66,2)/(frequencyindex[a]*66));

    }
     else if (temp>=0){
      correlation[3][i]=correlation[3][i]+(pow(count[3][temp]-frequencyindex[a]*66,2)/(frequencyindex[a]*66));
    }
    }

}
}


void choosingkey(float correlation[][27],int key[]){

for (int a=0;a<4;a++){


for (int i=1;i<26;i++){
    if (correlation[a][i]<correlation[a][key[a]])
        key[a]=i;

}

}


}



void decipher(char cipherdata[],char decipherdata[], int key[]){
int a=0;
for (int i=0;i<264;i++){
        if (cipherdata[i]-key[a]<65)
        decipherdata[i]=cipherdata[i]-key[a]+26;
    else if (cipherdata[i]-key[a]>=65)
        decipherdata[i]=cipherdata[i]-key[a];

    a++;
    if (a>=4)
        a=0;




}




}


int main(int argc, char *argv[]){

float frequencyindex[27]={0}; //this is the array of frequency of english character
templateindex(frequencyindex);//this is the function of entering frequency of english character
int key[5]={0};
float count[5][27]={0};
float correlation[5][27]={0};
char cipherdata[300]={0};
char decipherdata[300]={0};
FILE * cipher=fopen(argv[1],"r");
char onee[75]={0};
char twoo[75]={0};
char threee[75]={0};
char fourr[75]={0};
reading(cipher,cipherdata);

wordsplit(cipherdata,onee,twoo,threee,fourr);//split word into 4 list
printf("splited list:\n%s\n%s\n%s\n%s\n",onee,twoo,threee,fourr);

frequencycount(count,cipherdata,onee,twoo,threee,fourr);//count frequency
//normalize and give the percentage
correlationcheck(frequencyindex,count,correlation);
printf("Frequency count:\n");
for (int i=0;i<26;i++){

    printf("%c = %f %f %f %f\n",'A'+i,count[0][i],count[1][i],count[2][i],count[3][i]);
}
printf("\n\n");
for (int i=0;i<26;i++){

    printf("%c = %f\n",'A'+i,correlation[0][i]);
}
printf("\n\n");
for (int i=0;i<26;i++){

    printf("%c = %f\n",'A'+i,correlation[1][i]);
}
printf("\n\n");
for (int i=0;i<26;i++){

    printf("%c = %f\n",'A'+i,correlation[2][i]);
}
printf("\n\n");
for (int i=0;i<26;i++){

    printf("%c = %f\n",'A'+i,correlation[3][i]);
}
printf("\n\n");


choosingkey(correlation,key);
printf("key is %c%c%c%c\n\n\n",65+key[0],65+key[1],65+key[2],65+key[3]);

decipher(cipherdata,decipherdata,key);
printf("The decrypted text are:\n%s\n",decipherdata);
return 0;



}
