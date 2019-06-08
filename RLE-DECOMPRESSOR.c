#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

int main(int argc, char *argv[]) {

if (argc != 2)
{
printf("Invalid no of arguments");
exit(0);
}

FILE *fpt1, *fpt2 ;
unsigned char A,B,runcount=0;
const unsigned char flag='~';
bool marker=false;

if ((fpt1=fopen(argv[1],"rb"))==NULL)
{ 
printf("unable to open %s for reading \n",argv[1]);
exit(0);
}
char filename[50];
char *j=argv[1];
int len=strlen(argv[1]);
for(int i=0;i<(len-4);i++)
filename[i]=j[i];

fpt2 = fopen(filename, "wb");	

int i;
while(1==fread(&B, 1, 1, fpt1)) {
if (B=='~')
{			
if(marker==false)
{       			//if first flag is read then rle is off
A=B;
fread(&B, 1, 1, fpt1);
fwrite(&B,1, 1, fpt2);
marker=true;
}

else
{
fread(&runcount,1,1,fpt1);	//flag is read and if flag is already true rle is on- read 						runcount and symbol
fread(&B,1,1,fpt1);
for(i=0;i<runcount;i++)
fwrite(&B,1,1,fpt2);
marker=false;
}
}
else if(marker==true)		//flag is not read but marker is true -rle is off
{
fwrite(&B,1,1,fpt2);
}
else
{
runcount=B;			//marker is false and flag is not read-rle is on
fread(&B,1,1,fpt1);
for(i = 0; i <runcount; i++)
		
fwrite(&B, 1, 1, fpt2);


}}
fclose(fpt1);
fclose(fpt2);
return(0);
}



