//hfile


#ifndef __LIBTEX2BMP_H__
#define __LIBTEX2BMP_H__
typedef unsigned int uint;
typedef unsigned short ushort;
typedef char PIXELS; //pixel array

typedef struct{
unsigned short		magic;
unsigned int 		fileSize;
unsigned int 		reserved;
unsigned int 		pixOffset;
//core header
unsigned int		headerSize;
int			width;
int			height;
unsigned short		planes;
unsigned short		bitsPerPixel;
unsigned int		compression;
unsigned int		pixArraySize;
int			hRes;
int			vRes;
unsigned int		colourCount;
unsigned int		importantCol;
} BMPHEADER;

typedef struct{
char R;
char G;
char B;
char X;
}COLOUR;

typedef struct{
int colours;
COLOUR *colour;
} PALETTE;

typedef struct{ 
BMPHEADER *header;
PIXELS *data;
PALETTE *pal;
} BMP; //bitmap

extern char *__PALETTE_FILE__;
extern PALETTE *__PALETTE_TABLE__;


//bitmap fuctions
int writeBitmap(char *fileName, BMP *bitmap);
BMP *createBitmap(int width,int height,PIXELS *pixels, PALETTE *palette);
int loadTexture(char *texFile,int width,int height, BMP *bmData);

//palette functions
int detectPalettes(char *palFile);
int populatePalette(char *palFile,PALETTE *pal,int paln);
PALETTE *createPalette(int colours);
int replaceColour(PALETTE *palette, int number,char R,char G, char B,char X );
//misc
uint getLoc(uint x,uint y,uint w,uint h);
//clean up
int freeBitmap( BMP *bmp );



#endif




