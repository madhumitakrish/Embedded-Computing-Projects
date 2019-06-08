#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char *argv[]) {

	Display *Monitor;
	Window ImageWindow;
	GC ImageGC;
	XWindowAttributes Atts;
	XImage *Picture;

	int ROWS,COLS,MAXX;
	unsigned char *displaydata;
	char header[20];
	unsigned char red= 0x00; 
	unsigned char green = 0x00;
	unsigned char blue = 0x00;
	FILE *fpt = NULL;
	int totalbytes;
	int bytes;
	int i,j=0;
	unsigned char *inputdata = NULL;
if (argc != 2)
{
printf("Invalid no of arguments");
exit(0);
}
if ((fpt=fopen(argv[1],"rb"))==NULL)
{
printf("unable to open %s for reading \n",argv[1]);

exit(0);
}
	        
fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&MAXX); 

if(strcmp(header, "P5") == 0) //greyscale image
 {
bytes = 1;
} 
else if(strcmp(header, "P6") == 0) //rgb image
{
bytes = 3;
}

totalbytes= ROWS * COLS * bytes;

fseek(fpt, sizeof(*displaydata), 1); 
	
inputdata=(unsigned char *)calloc(totalbytes,sizeof(*inputdata)); //allocating memory to inputdata

fread(inputdata,sizeof(*inputdata),totalbytes,fpt);

displaydata = (unsigned char *)calloc(ROWS*COLS*2,sizeof(*displaydata));
if(bytes == 1) {
for(i = 0; i < (ROWS*COLS); i++) {
red = inputdata[i] & 0xF8;           
green = inputdata[i] & 0xFC; 
blue = inputdata[i] & 0xF8;
displaydata[j+1] |= red | (green >> 5);
displaydata[j] |= (green << 3) | (blue >> 3);
j+=2;

} }
else {
for(i = 0; i < (ROWS*COLS*3); i+=3) {
red = inputdata[i] & 0xF8;
green= inputdata[i+1] & 0xFC;
blue = inputdata[i+2] & 0xF8;
displaydata[j+1] |= red | (green >> 5);
displaydata[j] |= (green << 3) | (blue >> 3); 
j+=2;
}
}

Monitor=XOpenDisplay(NULL);
if (Monitor == NULL) {
printf("Unable to open graphics display\n");
exit(0);
}

ImageWindow=XCreateSimpleWindow(Monitor,RootWindow(Monitor,0),
50,10,	/* x,y on screen */
COLS,ROWS,	/* width, height */
2, /* border width */
BlackPixel(Monitor,0),
WhitePixel(Monitor,0)
);

ImageGC=XCreateGC(Monitor,ImageWindow,0,NULL);

XMapWindow(Monitor,ImageWindow);
XFlush(Monitor);

while(1) {
XGetWindowAttributes(Monitor,ImageWindow,&Atts);
if (Atts.map_state == IsViewable)
 break;
}

Picture=XCreateImage(Monitor, DefaultVisual(Monitor,0),
 	DefaultDepth(Monitor,0),
		ZPixmap,	/* format */
		0,		/* offset */
		displaydata,/* the data */
		COLS,ROWS,	/* size of the image data */
		16,		/* pixel quantum (8, 16 or 32) */
		0);		/* bytes per line (0 causes compute) */

XPutImage(Monitor,ImageWindow,ImageGC,Picture,
		0,0,0,0,	/* src x,y and dest x,y offsets */
		COLS,ROWS);	/* size of the image data */

XFlush(Monitor);
sleep(2);
XCloseDisplay(Monitor);
fclose(fpt);


}


