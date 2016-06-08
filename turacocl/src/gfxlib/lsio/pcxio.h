/*
 *  pcxio
 *
 *   simple save/loader for pcx images
 *
 * $Id: pcx.h,v 1.7 2003/04/08 14:57:24 jerry Exp $
 *
 * $Log: pcx.h,v $
 * Revision 1.7  2003/04/08 14:57:24  jerry
 * Full load and save of paletted images now.
 * Save does not properly support 24 bit images, but load does.
 *
 * Revision 1.6  2003/04/04 05:54:43  jerry
 * Much of the PCX save routines are mostly done.
 * I still need to:
 * 	write the actual scanline encoder
 * 	test saving out 24 bit and 8 bit paletted images
 *
 * Revision 1.5  2003/04/03 19:46:28  jerry
 * 8bit (paletted) and 24bit (truecolor/multiplanar) image loading complete
 *
 * Revision 1.4  2003/04/02 22:38:12  jerry
 * Test main.c
 * added in the header and 256 color palette reader
 *
 * Revision 1.3  2003/04/02 21:53:20  jerry
 * Updated a lot of stuff.
 * changed 'xxx_ValidFile' to xxx_SupportedFile
 *
 * Revision 1.2  2003/04/02 21:46:32  jerry
 * Fleshed out more of the PCX stuff
 *
 * Revision 1.1  2003/03/31 17:31:40  jerry
 * Added pcx headers
 *
 *
 */

#include "image.h"
#include "ls.h"		/* for "FileFormat" */

#ifndef __PCX_H__
#define __PCX_H__

#define u8      unsigned char
#define u16     unsigned int

typedef struct{
    u8  manufacturer;   /* 0x0A: ZSoft */
    u8  version;
                        /* 0x02 with palette
                           0x03 without palette
                           0x05 24 bit pcx or paletted
                         */

    u8  encoding;       /* 0x01: PCX RLE */
    u8  bitsPerPixel;   /* number of bits per pixel 1, 2, 4, 8 */
    u16 xMin;           /* window */
    u16 yMin;
    u16 xMax;
    u16 yMax;
    u16 hDpi;
    u16 vDpi;
    u8  colormap[48];   /* first 16 colors, R, G, B as 8 bit values */
    u8  reserved;       /* 0x00 */
    u8  nplanes;        /* number of planes */
    u16 bytesPerLine;   /* number of bytes that represent a scanline plane
                           == must be an even number! */
    u16 paletteInfo;    /* 0x01 = color/bw
                           0x02 = grayscale
                         */
    u16 hScreenSize;    /* horizontal screen size */
    u16 vScreenSize;    /* vertical screen size */
    u8  filler[54];     /* padding to 128 bytes */
} PCX_Header;


typedef struct{
    PCX_Header hdr;     /* the above header */
    PIXEL pal[256];     /* the palette at the end of the file (if applicable) */

    unsigned char * bufr;	/* the image data */
    long bufrSize;

    int width;
    int height;
} PCX_Image;



/* helper decode functions (see source for more info.) */
int PCX_encget( int * pbyt, int * pcnt, FILE * fid );

/* helper Load functions (see source for more info.) */
int PCX_LoadHeader(     PCX_Image * pi, FILE * fp );
int PCX_LoadDecodeData( PCX_Image * pi, FILE * fp );
int PCX_LoadPalette(    PCX_Image * pi, FILE * fp );

IMAGE * PCX_toImage( PCX_Image * pi );


/* helper encode functions (see source for more info.) */
int PCX_encput( unsigned char byt, unsigned char cnt, FILE * fid );
int PCX_encLine( unsigned char *inBuff, int inLen, FILE *fp );

/* helper Save functions (see source for more info.) */
int PCX_SaveHeader(     PCX_Image * pi, FILE * fp );
int PCX_SaveEncodeData( PCX_Image * pi, FILE * fp );
int PCX_SavePalette(    PCX_Image * pi, FILE * fp );

PCX_Image * PCX_fromImage( IMAGE * i, IMAGE * palette );


/* gfxlib functions */
   IMAGE * PCX_Load( char * filename );
   void    PCX_Save( char * filename, IMAGE * tosave, IMAGE * palette );
FileFormat PCX_SupportedFile( char * filename );

int PCX_Query( void );

void PCX_Dump( PCX_Image * pi );

#endif
