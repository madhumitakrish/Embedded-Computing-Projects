#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{
		
unsigned char C;
unsigned char **d=(unsigned char **)malloc(65536 * sizeof(unsigned char *));
unsigned int *d_length=(unsigned int *)malloc(65536 * sizeof (int));
unsigned char *P=(unsigned char *)malloc(2000);
unsigned char *temp=(unsigned char *)malloc(2000);
unsigned short int val,len=0;
int i,p,pointer;		
FILE *fpt1, *fpt2;
fpt1 = fopen(argv[1],"rb");
char *k=argv[1];
char *fn = malloc(strlen(k+5)); 
sprintf(fn, "%s.lzw" , k);       // Copy the name with extensions into fn.
fpt2 = fopen(fn,"wb");
for(i=0;i<65536;i++)
{
d[i]=(unsigned char *)malloc(2000);
}

		
for(i=0;i<256;i++)
{
d_length[i]=1;
d[i][0]=i;
}
pointer=256;
while (fread(&C,1,1,fpt1) == 1)
{
for(i=0;i<len;i++)						//P+C
temp[i]=P[i];
P[len]=C;
for(i=0;i<pointer;i++)
{
if (d_length[i] == len+1 && memcmp(P,d[i],len+1)==0)		//lookup if P+C is present
{
p=1;
val=(unsigned short int)i;
}}
if(p == 1)							//if present
{
p=0;
len++;
}
else
{
for(i=0; i<pointer; i++)
{
if (d_length[i] == len && memcmp(temp, d[i], len) == 0)
{
val =(unsigned short int)i;
}}
fwrite(&val,sizeof(val),1,fpt2);				//Add P+C to dictionary
for(i=0;i<len+1;i++)
d[pointer][i]=P[i];
d_length[pointer] = len + 1;
pointer++;
*P=C;
len=1;
}
}
val=0;
fwrite(&val,sizeof(val),1,fpt2);
fclose(fpt1);
fclose(fpt2);
}
