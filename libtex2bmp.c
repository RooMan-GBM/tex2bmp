//c file
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <libtex2bmp.h>

char *__PALETTE_FILE__;
PALETTE *__PALETTE_TABLE__;

/****************************************************
bitmap data and file functions
****************************************************/
//preps the bitmap structure and returns a pointer
BMP *createBitmap(int width,int height,PIXELS *pixels, PALETTE *palette){
BMP *_bmp;
BMPHEADER *_header;
 _header		=	malloc(sizeof(BMPHEADER));
 _bmp			=	malloc(sizeof(BMP));

 _bmp->header		=	_header;
 _bmp->data		=	pixels;
 _bmp->pal 		=	palette;

 _header->magic		=	0x4D42;
 _header->fileSize	= 	54 + (width*height)/2 + (palette->colours*4);
 _header->headerSize	=	40;
 _header->pixOffset	=	54 + (palette->colours*4);
 _header->width		=	width;
 _header->height	=	height;
 _header->planes	=	1;
 _header->bitsPerPixel	=	4;
 _header->compression	=	0;
 _header->pixArraySize	=	( width *height )/2;
 _header->hRes		=	2835;
 _header->vRes		=	2835;
 _header->colourCount	=	palette->colours;
 _header->importantCol	=	0;

return _bmp;
} 

int loadTexture(char *texFile,int width,int height, BMP *bmData){
 FILE *fp;
 fp=fopen(texFile,"rb");
 int rv;
 if(!fp){
  printf("loadTexture: unable to open file\n");
  return -1;
 }
 fread(bmData->data,(width*height)/2,1,fp);
 fclose(fp);
return rv;
}

//writes a bitmap and returns > 0 on sucsess
int writeBitmap(char *fileName, BMP *bitmap){
FILE *fp;
int i=0;
 fp=fopen(fileName,"wb");
 if(!fp){
  printf("writeBitmap: failed to open file\n"); 
   return -1;
}
 //begin writing
 fwrite(&bitmap->header->magic,		sizeof(short)	,1,	fp);
 fwrite(&bitmap->header->fileSize,	sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->reserved,	sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->pixOffset,	sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->headerSize,	sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->width,		sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->height,	sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->planes	,	sizeof(short)	,1,	fp);
 fwrite(&bitmap->header->bitsPerPixel,	sizeof(short)	,1,	fp);
 fwrite(&bitmap->header->compression,	sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->pixArraySize,	sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->hRes,		sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->vRes,		sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->colourCount,	sizeof(int)	,1,	fp);
 fwrite(&bitmap->header->importantCol,	sizeof(int)	,1,	fp);
 // write palette
 for(i=0;i<bitmap->pal->colours;i++){
  fwrite( &bitmap->pal->colour[i].B,	1	,1,	fp);
  fwrite( &bitmap->pal->colour[i].G,	1	,1,	fp);
  fwrite( &bitmap->pal->colour[i].R,	1	,1,	fp);
  fwrite( &bitmap->pal->colour[i].X,	1	,1,	fp);
}
 //write pixels
 for(i= ((bitmap->header->width * bitmap->header->height ) /2)-bitmap->header->width/2 ;i>0;i-=bitmap->header->height/2){
  fwrite( &bitmap->data[i],
  bitmap->header->width/2 ,1, fp );
 }
fclose(fp);
}


/****************************************************
colour and palette stuff
****************************************************/

// attempts to count palettes in file. returns pallete count, -1 on error
int detectPalettes(char *palFile){
FILE *fp;
__PALETTE_FILE__=palFile;
unsigned int fileSize;
int i, paletteCount=0, rcolour, ccount;
 //open file
 fp = fopen( palFile, "rb" );
 if( !fp ){
  printf( "detectPalettes: palette file is unreadable or doesn't exist\n" );
  return -1;
 }
 //get palette size and validity
 fseek( fp, 0, SEEK_END );
 fileSize = ftell( fp );
 if( !fileSize % 4 ){ // if the file size doesn't devide by 4 then it is incomplete
  printf( "detectPalettes: palette file doesn't seem valid\n" );
  fclose( fp );
  return -1;
 }

 //printf( "detectPalettes: size is %d\n", fileSize);
 //read colours until end of palette flag
 COLOUR *cols;
 fseek( fp, 0, SEEK_SET );
 for(i=0;i<fileSize/4;i++){
  fread( &rcolour, 4, 1, fp);
  //printf( "detectPalettes: found a colour %x\n", rcolour);
  if(rcolour==0){ // check for end of palette flag
   paletteCount++;
   printf( "detectPalettes: end of palette\n" );
  }
 }
 fclose( fp );
 return paletteCount;
}


//fills the palette with the set palette number
//this dosen work!!!!
int populatePalette(char *palFile,PALETTE *pal,int paln){
FILE *fp;
__PALETTE_FILE__ = palFile;
unsigned int fileSize;
int i, paletteCount=0, rcolour, ccount=0;
 //open file
 fp = fopen( palFile, "rb" );
 if( !fp ){
  printf( "detectPalettes: palette file is unreadable or doesn't exist\n" );
  return -1;
 }
 //get palette size and validity
 fseek( fp, 0, SEEK_END );
 fileSize = ftell( fp );
 if( !fileSize % 4 ){ // if the file size doesn't devide by 4 then it is incomplete
  printf( "populatePallete: palette file doesn't seem valid\n" );
  fclose( fp );
  return -1;
 }

 fseek( fp, 0, SEEK_SET );
 for(i=0;i<16;i++){
  fread( &rcolour, 4, 1, fp);
  
  if(rcolour==0){ // check for end of palette flag
   if( paln == paletteCount ){
    pal->colours = ccount;
    break;
   }
   paletteCount++;
   ccount=0;
   i=0;
   continue;
   //fread( &rcolour, 4, 1, fp);
   
  }
  if(paletteCount==paln){
   pal->colour[ccount].R	=	(rcolour &(0x000000ff) ) ;
   pal->colour[ccount].G	=	(rcolour &(0x0000FF00) ) >>8;
   pal->colour[ccount].B	=	(rcolour &(0x00ff0000) ) >>16;
   pal->colour[ccount].X	=	(rcolour &(0xff000000) ) >>24;
   ccount++;
}
  

  
 }
 
 fclose( fp );
 return paletteCount;
}



//preps a pallete and returns pointer on succsess
PALETTE *createPalette( int colours ){
 PALETTE *_pal;
 _pal	= 	malloc( sizeof( PALETTE ) );
 if( !_pal ){
  return NULL;
}
 _pal->colour	=	malloc(colours*sizeof(COLOUR) );
 _pal->colours	=	colours;
return _pal;
}

//sets a colour in palette to RGBX and returns > 0 on sucsess
int replaceColour( PALETTE *palette, int number, char R, char G, char B, char X ){
 if(!palette){return -1;}
 palette->colour[number].R	=	R;
 palette->colour[number].G	=	G;
 palette->colour[number].B	=	B;
 palette->colour[number].X	=	X;
 return 1;
}


 

/****************************************************
bunch of misc functions
****************************************************/

//returns the location in the pixel array where x,y points
uint getLoc( uint x ,uint y ,uint w ,uint h ){
 return( x+y*w) ;
}


/****************************************************
claen up functions
****************************************************/

int freeBitmap( BMP *bmp ){
 free (bmp -> pal -> colour);
 free( bmp -> pal );
 free( bmp -> data );
 free( bmp -> header );
 free ( bmp );
 return 1;
}
