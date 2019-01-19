//c file
#include <stdlib.h>
#include <stdio.h>
#include <libtex2bmp.h>


int main(int argc,char *argv[]){
int bitest;
PIXELS *pdat;
pdat=malloc((256*256)/2);
PALETTE *palette;
BMP *bmFile;
bitest=0x424344;
int i=0,pcount;

 printf("Palettes Avalble %d\n",detectPalettes("pal.bin") );
 //printf( "palettes found: %d\n", pcount);
 palette = createPalette(15);
 populatePalette("pal.bin",palette,4);
 bmFile  = createBitmap(256,256,pdat,palette);
  printf("loadtex: %d\n", loadTexture("tex.bin",256,256,bmFile));
 writeBitmap("bitmap.bmp",bmFile);
 //free(pdat);
 freeBitmap(bmFile);
return 0;


}



