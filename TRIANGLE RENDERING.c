#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void multiplication(float v0[3],float v1[3][3],float v2[3]);
void crossproduct(float ax,float ay,float az,float bx,float by,float bz,float c[3]);
void dotproduct(float ax,float ay,float az,float bx,float by,float bz,float *c);
float sub(float a,float b,float *c);

int main(int argc,char *argv[])
{

int i=0,j=0,r,c,COLS=256,ROWS=256;
unsigned char data[100];
char format[10];
int vertices,face,nr;
float minimumv0=0,minimumv1=0,minimumv2=0,maximummv0=0,maximummv1=0,maximummv2=0;
float centerv0,centerv1,centerv2,camera[3],up[3],camerav0=1,camerav1=0,camerav2=0,up0=0,up1=0,up2=1,intersect[3];
float e0,e1,e2,e,a;
float tmp[3],tmp1[3],left[3],right[3],top[3],bottom[3],topleft[3];
float rotation1[3][3],rotation2[3][3],rotation3[3][3];
float theta1,theta2,theta3;
float image[ROWS*COLS][3];
unsigned char pixelcolor[ROWS*COLS];
float A,B,C,D,temp[3],n,d;
float dot1,dot2,dot3;
float new1[3],new2[3];
float temp1,temp2,temp3,temp4,temp5,temp6;
FILE *fpt1,*fpt2;	
if (argc !=5)
	{
	printf("Invalid no of arguments");
	exit(0);
	}
if ((fpt1=fopen(argv[1],"rb"))==NULL)
	{ 
	printf("unable to open %s for reading \n",argv[1]);
	exit(0);
	}
	fpt2=fopen("NEW.ppm","wb");
	fscanf (fpt1, "%s", format);

while(strcmp(data,"end_header")!=0)
	{	
		fscanf(fpt1,"%s",data);
		if(strcmp(data,"vertex")==0)
			fscanf(fpt1,"%d",&vertices);
		else if(strcmp(data,"face")==0)
			fscanf(fpt1,"%d",&face);
		
		
	}
float v0[vertices],v1[vertices],v2[vertices];
int xf[face],yf[face],zf[face];
	fprintf(fpt2,"P5 256 256 255\n");

	for(i=0;i<vertices;i++)
	{
		fscanf(fpt1,"%f %f %f",&v0[i],&v1[i],&v2[i]);
	}
	
	for(i=0;i<face;i++)
	{
		fscanf(fpt1,"%d %d %d %d",&nr,&xf[i],&yf[i],&zf[i]);
	}
	
	minimumv0=v0[0];
	minimumv1=v1[0];
	minimumv2=v2[0];

	for(i=0;i<vertices;i++) 				//FINDING MIN AND MAX
	{
		if(v0[i]<minimumv0)
			minimumv0=v0[i];
		 if(v1[i]<minimumv1)
			minimumv1=v1[i];
		if(v2[i]<minimumv2)
			minimumv2=v2[i];
		 if(v0[i]>maximummv0)
			maximummv0=v0[i];
		if(v1[i]>maximummv1)
			maximummv1=v1[i];
		if(v2[i]>maximummv2)
			maximummv2=v2[i];
	}

centerv0=(minimumv0+maximummv0)/2;
centerv1=(minimumv1+maximummv1)/2;
centerv2=(minimumv2+maximummv2)/2;

	e0=maximummv0-minimumv0;
	e1=maximummv1-minimumv1;
	e2=maximummv2-minimumv2;

	if(e0 > e1 && e0 > e2)
		e=e0;
	 if(e1 > e0 && e1 > e2)
		e=e1;
	if(e2 > e1 && e2 > e0)
		e=e2;


theta1=atoi(argv[2]);
theta1=M_PI*theta1/180.0;
theta2=atoi(argv[3]);
theta2=M_PI*theta2/180.0;
theta3=atoi(argv[4]);
theta3=M_PI*theta3/180.0;

rotation1[0][0]=1;
rotation1[0][1]=0;
rotation1[0][2]=0;
rotation1[1][0]=0;
rotation1[1][1]=cos(theta1);
rotation1[1][2]=-sin(theta1);
rotation1[2][0]=0;
rotation1[2][1]=sin(theta1);
rotation1[2][2]=cos(theta1);

rotation2[0][0]=cos(theta2);
rotation2[0][1]=0;
rotation2[0][2]=sin(theta2);
rotation2[1][0]=0;
rotation2[1][1]=1;
rotation2[1][2]=0;
rotation2[2][0]=-sin(theta2);
rotation2[2][1]=0;
rotation2[2][2]=cos(theta2);

rotation3[0][0]=cos(theta3);
rotation3[0][1]=-sin(theta3);
rotation3[0][2]=0;
rotation3[1][0]=sin(theta3);
rotation3[1][1]=cos(theta3);
rotation3[1][2]=0;
rotation3[2][0]=0;
rotation3[2][1]=0;
rotation3[2][2]=1;

camera[0]=camerav0;
camera[1]=camerav1;
camera[2]=camerav2;

	multiplication(camera,rotation1,tmp);			//matrix multiplication
	multiplication(tmp,rotation2,tmp1);
	multiplication(tmp1,rotation3,camera);

up[0]=up0;
up[1]=up1;
up[2]=up2;

	multiplication(up,rotation1,tmp);
	multiplication(tmp,rotation2,tmp1);
	multiplication(tmp1,rotation3,up);

camerav0=camera[0];
camerav1=camera[1];
camerav2=camera[2];

up0=up[0];
up1=up[1];
up2=up[2];

	camerav0=1.5*e*camerav0 +centerv0;
	camerav1=1.5*e*camerav1 +centerv1;
	camerav2=1.5*e*camerav2 +centerv2;

	crossproduct(up0,up1,up2,(centerv0-camerav0),(centerv1-camerav1),(centerv2-camerav2),left);

	a=sqrt(pow(left[0],2)+pow(left[1],2)+pow(left[2],2));

	left[0]=(e/(2*a))*left[0]+centerv0;
	left[1]=(e/(2*a))*left[1]+centerv1;
	left[2]=(e/(2*a))*left[2]+centerv2;

	crossproduct((centerv0-camerav0),(centerv1-camerav1),(centerv2-camerav2),up0,up1,up2,right);
	
	right[0]=(e/(2*a))*right[0]+centerv0;
	right[1]=(e/(2*a))*right[1]+centerv1;
	right[2]=(e/(2*a))*right[2]+centerv2;

	top[0]=(e/(2.0))*up0+centerv0;
	top[1]=(e/(2.0))*up1+centerv1;
	top[2]=(e/(2.0))*up2+centerv2;

	bottom[0]=(-e/(2.0))*up0+centerv0;
	bottom[1]=(-e/(2.0))*up1+centerv1;
	bottom[2]=(-e/(2.0))*up2+centerv2;

	topleft[0]=(e/(2.0))*up0+left[0];
	topleft[1]=(e/(2.0))*up1+left[1];
	topleft[2]=(e/(2.0))*up2+left[2];

for(r=0;r<ROWS;r++)
{
for(c=0;c<COLS;c++)
{
image[r*COLS+c][0]=topleft[0]+((float) c/(float)(255))*(right[0]-left[0])+((float) r/((float)(255)))*(bottom[0]-top[0]);
image[r*COLS+c][1]=topleft[1]+((float) c/(float)(255))*(right[1]-left[1])+((float) r/((float)(255)))*(bottom[1]-top[1]);
image[r*COLS+c][2]=topleft[2]+((float) c/(float)(255))*(right[2]-left[2])+((float) r/((float)(255)))*(bottom[2]-top[2]);
}}

for(j=0;j<face;j++)
	{
sub(v0[xf[j]],v0[yf[j]],&temp1);
sub(v1[xf[j]],v1[yf[j]],&temp2);
sub(v2[xf[j]],v2[yf[j]],&temp3);
sub(v0[xf[j]],v0[zf[j]],&temp4);
sub(v1[xf[j]],v1[zf[j]],&temp5);
sub(v2[xf[j]],v2[zf[j]],&temp6);
crossproduct((temp1),(temp2),(temp3),(temp4),(temp5),(temp6),temp);
A=temp[0],B=temp[1],C=temp[2];
dotproduct(-A,-B,-C,v0[xf[j]],v1[xf[j]],v2[xf[j]],&D);
dotproduct(-A,-B,-C,(camerav0),(camerav1),(camerav2),&n);
n=n-D;
for(i=0;i<ROWS*COLS;i++)
{		
dotproduct(A,B,C,(image[i][0]-camerav0),(image[i][1]-camerav1),(image[i][2]-camerav2),&d);
	if(fabs(d)<0.0001)
		continue;
intersect[0]=camerav0+(n/d)*(image[i][0]-camerav0);
intersect[1]=camerav1+(n/d)*(image[i][1]-camerav1);
intersect[2]=camerav2+(n/d)*(image[i][2]-camerav2);
sub(v0[xf[j]],v0[yf[j]],&temp1);
sub(v1[xf[j]],v1[yf[j]],&temp2);
sub(v2[xf[j]],v2[yf[j]],&temp3);
sub(v0[xf[j]],v0[zf[j]],&temp4);
sub(v1[xf[j]],v1[zf[j]],&temp5);
sub(v2[xf[j]],v2[zf[j]],&temp6);

crossproduct((temp4),(temp5),(temp6),(temp1),(temp2),(temp3),new1);
			
sub(v0[xf[j]],intersect[0],&temp4);
sub(v1[xf[j]],intersect[1],&temp5);
sub(v2[xf[j]],intersect[2],&temp6);
crossproduct((temp4),(temp5),(temp6),(temp1),(temp2),(temp3),new2);
dotproduct(new1[0],new1[1],new1[2],new2[0],new2[1],new2[2],&dot1);
sub(v0[yf[j]],v0[xf[j]],&temp1);
sub(v1[yf[j]],v1[xf[j]],&temp2);
sub(v2[yf[j]],v2[xf[j]],&temp3);
sub(v0[yf[j]],v0[zf[j]],&temp4);
sub(v1[yf[j]],v1[zf[j]],&temp5);
sub(v2[yf[j]],v2[zf[j]],&temp6);
crossproduct((temp1),(temp2),(temp3),(temp4),(temp5),(temp6),new1);
			
sub(v0[yf[j]],intersect[0],&temp1);
sub(v1[yf[j]],intersect[1],&temp2);
sub(v2[yf[j]],intersect[2],&temp3);
crossproduct((temp1),(temp2),(temp3),(temp4),(temp5),(temp6),new2);
dotproduct(new1[0],new1[1],new1[2],new2[0],new2[1],new2[2],&dot2);

sub(v0[zf[j]],v0[yf[j]],&temp1);
sub(v1[zf[j]],v1[yf[j]],&temp2);
sub(v2[zf[j]],v2[yf[j]],&temp3);
sub(v0[zf[j]],v0[xf[j]],&temp4);
sub(v1[zf[j]],v1[xf[j]],&temp5);
sub(v2[zf[j]],v2[xf[j]],&temp6);

crossproduct((temp1),(temp2),(temp3),(temp4),(temp5),(temp6),new1);

sub(v0[zf[j]],intersect[0],&temp1);
sub(v1[zf[j]],intersect[1],&temp2);
sub(v2[zf[j]],intersect[2],&temp3);
crossproduct((temp1),(temp2),(temp3),(temp4),(temp5),(temp6),new2);
dotproduct(new1[0],new1[1],new1[2],new2[0],new2[1],new2[2],&dot3);

			if(dot1>0){						//checks if intersection is inside triangle
				if(dot2>0){
					if(dot3>0)
			{
				pixelcolor[i]=155+(j%100);			//sets pixel colour
			}}}
		}
	
		
	}

	
	fwrite(pixelcolor,1,ROWS*COLS,fpt2);
	

	fclose(fpt1);
	fclose(fpt2);
}



void multiplication(float v0[3],float v1[3][3],float v2[3])
{
	v2[0]=v0[0]*v1[0][0]+v0[1]*v1[1][0]+v0[2]*v1[2][0];
	v2[1]=v0[0]*v1[0][1]+v0[1]*v1[1][1]+v0[2]*v1[2][1];
	v2[2]=v0[0]*v1[0][2]+v0[1]*v1[1][2]+v0[2]*v1[2][2];
}

void crossproduct(float ax,float ay,float az,float bx,float by,float bz,float c[3])
{
	c[0] = (ay*bz)-(az*by);
	c[1] = (az*bx)-(ax*bz);
	c[2] = (ax*by)-(ay*bx);

}

void dotproduct(float ax,float ay,float az,float bx,float by,float bz,float *c)
{
	*c=(ax*bx)+(ay*by)+(az*bz);
}

float sub(float a,float b,float *c)
{
	*c=b-a;
}	
