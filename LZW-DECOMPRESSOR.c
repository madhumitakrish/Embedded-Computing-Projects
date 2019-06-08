#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{

unsigned char **d=(unsigned char **)malloc(65536*sizeof(unsigned char *));
unsigned int *d_length=(unsigned int *)malloc(65536 * sizeof (int));
unsigned char *X=(unsigned char *)malloc(2000);
unsigned char *Y=(unsigned char *)malloc(2000);
unsigned char *Z=(unsigned char *)malloc(2000);
unsigned short int C=0,P=0;
int i,pointer, j=0,len=0;
unsigned short int val;
FILE *fpt1,*fpt2;
fpt1=fopen(argv[1],"rb");
char filename[50];
char *k=argv[1];
int Len=strlen(argv[1]);
for(i=0;i<(len-4);i++)
filename[i]=k[i];

sprintf(filename, "%s.deco" , k);       // Copy the name with extensions into fn.
fpt2=fopen(filename,"wb");
for(j=0;j<65536;j++)
{
d[j]=(unsigned char *)malloc(2500);
}

for(i=0;i<256;i++)
{
d_length[i]=1;
d[i][0]=i;
}
pointer=256;
fread(&val,sizeof(val),1,fpt1);
C=val;
fwrite(d[C],1,d_length[C],fpt2);
P=C;
while(fread(&val,sizeof(val),1,fpt1)==1)
{
P=C;
C=val;
for(i=0;i<d_length[P];i++)
X[i]=d[P][i];
for(i=0;i<d_length[P];i++)
d[pointer][i]=d[P][i];

if(C<256)					//C in dictionary
{
fwrite(d[C],1,d_length[C],fpt2);
Y[0]=d[C][0];
d[pointer][d_length[P]]=Y[0];			//Add X+Y
d_length[pointer]=d_length[P]+1;
pointer++;
}
else if(C<pointer)
{
fwrite(d[C],1,d_length[C],fpt2);
Y[0]=d[C][0];
d[pointer][d_length[P]]=Y[0];
d_length[pointer]=d_length[P]+1;
pointer++;
}

else						//C not in dictionary
{			
Z[0]=d[P][0];
fwrite(d[P],1,d_length[P],fpt2);		//output X+Z
fwrite(d[P],1,1,fpt2);
d[pointer][d_length[P]]=Z[0];			//Add X+Z
d_length[pointer]=d_length[P]+1;
pointer++;
}}
val=0;
fwrite(&val,1,1,fpt2);
fclose(fpt1);
fclose(fpt2);
}
