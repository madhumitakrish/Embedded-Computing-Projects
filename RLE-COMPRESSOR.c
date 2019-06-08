#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

int main(int argc,char *argv[])
{

FILE *fpt1,*fpt2; ;
unsigned char A,B,runcount=1;
bool marker=false;
const char flag='~';
if (argc != 2)
{
printf("Invalid no of arguments");
exit(0);
}
if ((fpt1=fopen(argv[1],"rb"))==NULL)
{ 
printf("unable to open %s for reading \n",argv[1]);
exit(0);
}

char *j=argv[1];
char *fn = malloc(strlen(j+5)); 
sprintf(fn, "%s.rle" , j);       // Copy the name with extensions into fn.
fpt2 = fopen(fn,"wb");
fread(&A,1,1,fpt1);

while(1 == fread(&B, 1, 1, fpt1))
 {

if(A==B) 
{
if(runcount==255)		//check if runcount is maximum
{
if(marker==true)
{
fwrite(&flag,1,1,fpt2);
fwrite(&runcount,1,1,fpt2);
fwrite(&A,1,1,fpt2);
marker=false;
A=B;
runcount=0;
}
else
{
fwrite(&runcount,1,1,fpt2);
fwrite(&A,1,1,fpt2);
A=B;
runcount=0;
}}
else 
{
runcount ++;			//if runcount < 255 increment it
A=B;
}}

else
{

if((runcount>1) && (marker==true))
{					//writes the flag value,runcount,symbol consecutively
fwrite(&flag,1,1,fpt2);
fwrite(&runcount,1,1,fpt2);
fwrite(&A,1,1,fpt2);
marker=false;
runcount=1;
}
else if((runcount>1) && (marker==false))
{
fwrite(&runcount,1,1,fpt2);
fwrite(&A,1,1,fpt2);
runcount=1;
}

else
{
				//if runcount is 1 check marker status
if(marker==true)
{
fwrite(&A,1,1,fpt2);
}
if(marker==false)
{
fwrite(&flag,1,1,fpt2);
marker=true;
fwrite(&A,1,1,fpt2);

if ((marker==true) && (A==flag))
{
fwrite(&A,1,1,fpt2);
}
if((marker==false) && (A==flag))
{
fwrite(&flag,1,1,fpt2);
}
}


runcount=1;
}}
A=B;
}
fclose(fpt1);
fclose(fpt2);
return(0);
}

