#include <windows.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


LRESULT CALLBACK EventProcessor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

{ return (DefWindowProc(hWnd, uMsg, wParam, lParam)); }
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,

 LPSTR lpCmdLine, int nCmdShow)

{

	WNDCLASS wc;
	HWND WindowHandle;
	int ROWS, COLS, MAXX;
	unsigned char* displaydata;
	char header[80];
	FILE *fpt;
	BITMAPINFO* bm_info;
	HDC hDC;
	int bytes;
	int totalbytes;
	int i,j;
	char file[20];
	unsigned char *inputdata = NULL;
	unsigned char red = 0x00;
	unsigned char green = 0x00;
	unsigned char blue = 0x00;

wc.style = CS_HREDRAW | CS_VREDRAW;

wc.lpfnWndProc = (WNDPROC)EventProcessor;

wc.cbClsExtra = wc.cbWndExtra = 0;

wc.hInstance = hInstance;

wc.hIcon = wc.lpszMenuName = NULL;

wc.hCursor = LoadCursor(NULL, IDC_ARROW);

wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

 wc.lpszClassName = "Image Window Class";

 if (RegisterClass(&wc) == 0)

exit(0);
if (MessageBox(NULL,"PRINT GREYSCALE","", MB_YESNO) == IDYES)
strcpy(file,"bridge.ppm");
else
strcpy(file,"retina.ppm");
fpt = fopen(file,"rb");
if (fpt == NULL) {
MessageBox(NULL, "unable to open file for reading \n", "", MB_OK | MB_APPLMODAL);
}
fscanf(fpt, "%s %d %d %d", header,&COLS, &ROWS, &MAXX);
if (strcmp(header, "P5") == 0)
{
bytes = 1;									//image is greyscale
}
else if (strcmp(header, "P6") == 0)
{
bytes = 3;								    //image is rgb
}
totalbytes = ROWS * COLS * bytes;		   //finding total number of bytes
fseek(fpt,sizeof(*displaydata),1);
inputdata=(unsigned char *)calloc(totalbytes,sizeof(*inputdata)); //allocating memory to inputdata
fread(inputdata,sizeof(*inputdata),totalbytes,fpt);
if (bytes == 3)
{
displaydata=calloc((ROWS*COLS*2),sizeof(*displaydata));
i=0;
for (j = 0; j < ROWS*COLS*3; j += 3)
{
red = inputdata[j] & 0xF8;
green = inputdata[j + 1] & 0xF8;
blue = inputdata[j + 2] & 0xF8;

displaydata[i + 1] |= (red >>1) | (green>> 6);
displaydata[i] |= (green<< 2) | (blue >> 3);
i+=2;
}
}
else
{
displaydata=inputdata;
}
WindowHandle = CreateWindow("Image Window Class", "ECE468 Lab1",
WS_OVERLAPPEDWINDOW,10, 10, COLS+20, ROWS+40,
NULL, NULL, hInstance, NULL);
if (WindowHandle == NULL)
{
MessageBox(NULL, "No window", "Try again", MB_OK | MB_APPLMODAL);
exit(0);
}
ShowWindow(WindowHandle, SW_SHOWNORMAL);
bm_info = (BITMAPINFO*)calloc(1, sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
hDC = GetDC(WindowHandle);
 /* ... set up bmiHeader field of bm_info ... */
 
bm_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
bm_info->bmiHeader.biPlanes = 1;
if(bytes==3)
{
	bm_info->bmiHeader.biBitCount = 16;}
else
{
	bm_info->bmiHeader.biBitCount = 8;}
bm_info->bmiHeader.biWidth = COLS;
bm_info->bmiHeader.biHeight = -ROWS;


for (i = 0; i < 256; i++) /* colormap */
{
bm_info->bmiColors[i].rgbBlue = bm_info->bmiColors[i].rgbGreen = bm_info->bmiColors[i].rgbRed = i;
bm_info->bmiColors[i].rgbReserved = 0;
}
SetDIBitsToDevice(hDC, 0, 0, COLS, ROWS, 0, 0,0, /* first scan line */
ROWS, /* number of scan lines */
displaydata, bm_info, DIB_RGB_COLORS);
ReleaseDC(WindowHandle, hDC);
free(bm_info);
fclose(fpt);
MessageBox(NULL, "Press OK to continue", "", MB_OK | MB_APPLMODAL);
}
