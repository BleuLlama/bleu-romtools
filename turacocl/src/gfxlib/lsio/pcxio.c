/*
 *  pcxio
 *
 *   simple save/loader for pcx images 
 *
 * $Id: pcx.c,v 1.13 2003/04/09 02:08:31 jerry Exp $
 *
 * $Log: pcx.c,v $
 * Revision 1.13  2003/04/09 02:08:31  jerry
 * Hopefully fixed this so that it will output the data block on MS-DOS?
 *
 * Revision 1.12  2003/04/08 14:57:24  jerry
 * Full load and save of paletted images now.
 * Save does not properly support 24 bit images, but load does.
 *
 * Revision 1.11  2003/04/04 05:54:43  jerry
 * Much of the PCX save routines are mostly done.
 * I still need to:
 * 	write the actual scanline encoder
 * 	test saving out 24 bit and 8 bit paletted images
 *
 * Revision 1.10  2003/04/03 19:54:18  jerry
 * removed the PCX_Dump line from PCX_Load()
 * removed the test files
 *
 * Revision 1.9  2003/04/03 19:46:28  jerry
 * 8bit (paletted) and 24bit (truecolor/multiplanar) image loading complete
 *
 * Revision 1.8  2003/04/02 22:38:12  jerry
 * Test main.c
 * added in the header and 256 color palette reader
 *
 * Revision 1.7  2003/04/02 21:53:20  jerry
 * Updated a lot of stuff.
 * changed 'xxx_ValidFile' to xxx_SupportedFile
 *
 * Revision 1.6  2003/04/02 21:46:32  jerry
 * Fleshed out more of the PCX stuff
 *
 * Revision 1.5  2003/04/02 21:24:56  jerry
 * Added endian.[ch] for use with the PCX encoding and decoding.
 *
 * Revision 1.4  2003/04/01 04:15:06  jerry
 * Added info about the header
 *
 * Revision 1.3  2003/04/01 03:13:12  jerry
 * Shoved in the ZSoft source code.
 *
 * Revision 1.2  2003/03/31 17:38:21  jerry
 * updated the structure
 * changed ppmio to ppm
 * added pcx
 *
 * Revision 1.1  2003/03/31 17:31:40  jerry
 * Added pcx headers
 *
 */

/*
 * NOTE: 

	Some of the code in this .c file has been swiped from the
	"ZSoft PCX File Format Technical Reference Manual", from
	the section titled "Sample 'C' Routines".  It is my
	understanding that this provided source code is free to
	use without restriction, although I will provide this note
	and the copyright for that document in here anyway.  This
	referenced document is available upon request.

	The "ZSoft PCX File Format Technical Reference Manual" is
	Copyright c 1985, 1987, 1988, 1990, 1991, ZSoft Corporation.
	All Rights Reserved.

	Reformatted from K&R C into ANSI C by Jerry Carpenter, 2003
	(Basically just some function prototypes. meh.)
 */


/*
 * KNOWN BUGS FOR THIS IMPLEMENTATION:

	- Probably doesn't read in 1,2, or 4 bit-per-pixel images properly
	    (completely untested, but it shouldn't matter for Turaco CL)
	- only saves out single-plane type 5
	- only reads in types 2, 3, and 5
	- probably doesn't save out non-even width files properly

 * DESIGN OOPS'ES:

	- palette structure inconsistancies. oops.

 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"
#include "endian.h"
#include "io_util.h"
#include "ls.h"
#include "pcxio.h"


/*
 Decoding .PCX Files

	First, find the pixel dimensions of the image by calculating
	[XSIZE = Xmax - Xmin + 1] and [YSIZE = Ymax - Ymin + 1].
	Then calculate how many bytes are required to hold one
	complete uncompressed scan line:

	TotalBytes = NPlanes * BytesPerLine

	Note that since there are always an even number of bytes
	per scan line, there will probably be unused data at the
	end of each scan line.  TotalBytes shows how much storage
	must be available to decode each scan line, including any
	blank area on the right side of the image.  You can now
	begin decoding the first scan line - read the first byte
	of data from the file.  If the top two bits are set, the
	remaining six bits in the byte show how many times to
	duplicate the next byte in the file.  If the top two bits
	are not set, the first byte is the data itself, with a
	count of one.

	Continue decoding the rest of the line.  Keep a running
	subtotal of how many bytes are moved and duplicated into
	the output buffer.  When the subtotal equals TotalBytes,
	the scan line is complete.  There should always be a decoding
	break at the end of each scan line.  But there will not be
	a decoding break at the end of each plane within each scan
	line.  When the scan line is completed, there may be extra
	blank data at the end of each plane within the scan line.
	Use the XSIZE and YSIZE values to find where the valid
	image data is.  If the data is multi-plane, BytesPerLine
	shows where each plane ends within the scan line.

	Continue decoding the remainder of the scan lines (do not
	just read to end-of-file).  There may be additional data
	after the end of the image (palette, etc.)
 */

/*////////////////////////////////////////////////////////////////////////////*/
void
PCX_Dump(
	PCX_Image * pi
)
{
    int c, d, e;
    if( !pi ) return;

    printf( "PCX Image:\n");
    printf( "  Manufacturer: 0x%02x (0x0a)\n", pi->hdr.manufacturer );
    printf( "       Version: %d\n", pi->hdr.version );
    printf( "      Encoding: %d\n", pi->hdr.encoding );
    printf( "  BitsPerPixel: %d\n", pi->hdr.bitsPerPixel );
    printf( "        Window: (%d %d)-(%d %d)\n", 
		    pi->hdr.xMin, pi->hdr.yMin, pi->hdr.xMax, pi->hdr.yMax );

    printf( "                %d x %d\n", pi->width, pi->height ); 

    printf( "         H DPI: %d\n", pi->hdr.hDpi );
    printf( "         V DPI: %d\n", pi->hdr.vDpi );
    printf( "      reserved: %d\n", pi->hdr.reserved );
    printf( "      n planes: %d\n", pi->hdr.nplanes );
    printf( "  bytesPerLine: %d\n", pi->hdr.bytesPerLine );
    printf( "   paletteInfo: %d\n", pi->hdr.paletteInfo );
    printf( "   hScreenSize: %d\n", pi->hdr.hScreenSize );
    printf( "   vScreenSize: %d\n", pi->hdr.vScreenSize );

    printf( " 16 color palette header data:\n ");
    for( c=0, d=3, e=0 ; c<48 ; c++, d++, e++ )
    {
	if( e == 12 )
	{
	    d=0; e=0;
	    printf( "\n    [%02x]", c/3 );
	} else if( d == 3 )
	{
	    d=0;
	    printf( "   [%02x]", c/3 );
	}
	printf( " %02x", pi->hdr.colormap[c] );
    }
    printf("\n");

    printf( " 256 color palette data:\n ");
    for( c=0, d=0 ; c<256 ; c++, d++ )
    {
	if( d == 4 )
	{
	    d=0; 
	    printf( "\n " );
	}
	printf( "   [%02x] ", c );
	printf( "%02x %02x %02x", pi->pal[c].r, pi->pal[c].g, pi->pal[c].b );
    }
    printf("\n");

    printf( "%6ld bytes\n", pi->bufrSize );
    printf( "%6d pixels\n", pi->width * pi->height );
    
}

/*////////////////////////////////////////////////////////////////////////////*/



/* 
 * PCX_encget
 *
 *  This procedure reads one encoded block from the image 
 *  file and stores a count and data byte.
 */
int 
PCX_encget(
	int * pbyt,	/* where to place data */
	int * pcnt,	/* where to place count */
	FILE * fid	/* image file handle */
)
{
    int i;
    *pcnt = 1;        /* assume a "run" length of one */

    /* check for EOF */
    if (EOF == (i = getc(fid)))
	return (EOF);

    if (0xC0 == (0xC0 & i)) /* is it a RLE repeater */
    {
	/* YES.  set the repeat count */
	*pcnt = 0x3F & i;

	/* doublecheck the next byte for EOF */
	if (EOF == (i = getc(fid)))
	    return (EOF);
    }
    /* set the byte */
    *pbyt = i;

    /* return an 'OK' */
    return( 0 );
}


/*////////////////////////////////////////////////////////////////////////////*/


int
PCX_LoadHeader( 
	PCX_Image * pi,
	FILE * fp
)
{
    int c;
    if( !pi || !fp )  return( -1 );

    /* go to the beginning of the file */
    rewind( fp );

    /* read in the header blocks. */
    pi->hdr.manufacturer = endian_LittleRead8( fp );
    pi->hdr.version      = endian_LittleRead8( fp );
    pi->hdr.encoding     = endian_LittleRead8( fp );
    pi->hdr.bitsPerPixel = endian_LittleRead8( fp );

    pi->hdr.xMin = endian_LittleRead16( fp );
    pi->hdr.yMin = endian_LittleRead16( fp );
    pi->hdr.xMax = endian_LittleRead16( fp );
    pi->hdr.yMax = endian_LittleRead16( fp );

    /* do a little precomputing... */
    pi->width  = pi->hdr.xMax - pi->hdr.xMin + 1;
    pi->height = pi->hdr.yMax - pi->hdr.yMin + 1;

    pi->hdr.hDpi = endian_LittleRead16( fp );
    pi->hdr.vDpi = endian_LittleRead16( fp );

    /* read in 16 color colormap */
    for( c=0 ; c<48 ; c++ )
    {
	pi->hdr.colormap[c] = endian_LittleRead8( fp );
    }

    pi->hdr.reserved     = endian_LittleRead8( fp );
    pi->hdr.nplanes      = endian_LittleRead8( fp );
    pi->hdr.bytesPerLine = endian_LittleRead16( fp );
    pi->hdr.paletteInfo  = endian_LittleRead16( fp );
    pi->hdr.hScreenSize  = endian_LittleRead16( fp );
    pi->hdr.vScreenSize  = endian_LittleRead16( fp );

    /* ignore the filler */

    return( 0 );
}



unsigned char PCX_defaultPalette[48] = {
	0x00, 0x00, 0x00,    0x00, 0x00, 0x80,    0x00, 0x80, 0x00,
	0x00, 0x80, 0x80,    0x80, 0x00, 0x00,    0x80, 0x00, 0x80,
	0x80, 0x80, 0x00,    0x80, 0x80, 0x80,    0xc0, 0xc0, 0xc0,
	0x00, 0x00, 0xff,    0x00, 0xff, 0x00,    0x00, 0xff, 0xff,
	0xff, 0x00, 0x00,    0xff, 0x00, 0xff,    0xff, 0xff, 0x00,
	0xff, 0xff, 0xff 
};


int
PCX_LoadPalette(
	PCX_Image * pi, 
	FILE * fp
)
{
    int c;
    int checkbyte;

    if( !pi || !fp ) return( -1 );

    if( pi->hdr.version == 3 )
    {
	/* copy over the default palette to the header */
	for( c=0 ; c<48 ; c++ )
	{
	    pi->hdr.colormap[c] = PCX_defaultPalette[c];
	}

	/* copy over the default palette to the palette structure */
	for( c=0 ; c<16 ; c++ )
	{
	    pi->pal[c].r = PCX_defaultPalette[c*3 + 0];
	    pi->pal[c].g = PCX_defaultPalette[c*3 + 1];
	    pi->pal[c].b = PCX_defaultPalette[c*3 + 2];
	}
	return( 0 );
    }

    if( pi->hdr.bitsPerPixel != 8 )
    {
	/* no palette in the image, just return */
	return( 0 );
    }

    /* first seek to the end of the file -769 */
    fseek( fp, -769, SEEK_END );
    checkbyte = endian_LittleRead8( fp );

    if( checkbyte != 12 ) /* magic value */
    {
	printf("Expected a 256 color palette, didn't find it.\n");
	return( -1 );
    }

    /* okay. so we're at the right part of the file now, we just need
	to populate our palette! */
    for( c=0 ; c<256 ; c++ )
    {
	pi->pal[c].r = endian_LittleRead8( fp );
	pi->pal[c].g = endian_LittleRead8( fp );
	pi->pal[c].b = endian_LittleRead8( fp );
    }

    /* now copy over the first 16 colors into the header */
    for( c=0 ; c<16 ; c++ )
    {
	pi->hdr.colormap[c*3 + 0] = pi->pal[c].r;
	pi->hdr.colormap[c*3 + 1] = pi->pal[c].g;
	pi->hdr.colormap[c*3 + 2] = pi->pal[c].b;
    }
    
    return( 0 );
}


int
PCX_LoadDecodeData(
	PCX_Image * pi, 
	FILE * fp
)
{
    int i;
    long l;
    int chr, cnt;
    unsigned char * bpos;

    if( !pi || !fp )  return( -1 );

    /* first, we need to go to the right point in the file */
    fseek( fp, 128, SEEK_SET ); /* oh yeah. this is important */

    /* Here's a program fragment using PCX_encget.  This reads an 
	entire file and stores it in a (large) buffer, pointed 
	to by the variable "bufr". "fp" is the file pointer for 
	the image */

    pi->bufrSize = (long )  pi->hdr.bytesPerLine 
		   * pi->hdr.nplanes 
		   * (1 + pi->hdr.yMax - pi->hdr.yMin);

    if( pi->bufr )  free( pi->bufr );

    pi->bufr = (unsigned char *) malloc( 
			sizeof( unsigned char ) * pi->bufrSize );

    bpos = pi->bufr;

    for (l = 0; l < pi->bufrSize; )  /* increment by cnt below */
    {
	if (EOF == PCX_encget(&chr, &cnt, fp))
	    break;

	for (i = 0; i < cnt; i++)
	    *bpos++ = chr;

	l += cnt;
    }

    return( 0 );
}



/* for the 24 bit demuxing */
#define PLANE_RED   ( 0 ) 
#define PLANE_GREEN ( 1 ) 
#define PLANE_BLUE  ( 2 ) 

IMAGE *
PCX_toImage(
	PCX_Image * pi
)
{
    int pcx_pos, image_pos, set_aside;
    int x, y, p;
    IMAGE * i = NULL;

    if( !pi )  return( NULL );

    i = Image_Create( pi->width, pi->height, pi->hdr.bitsPerPixel );

    if( pi->hdr.nplanes == 1 )
    {
	/* paletted image! */
	pcx_pos = image_pos = 0;
	for( y = 0 ; y < pi->height ; y++ )
	{
	    for( x = 0 ; x < pi->hdr.bytesPerLine ; x ++ )
	    {
		/* the width might be different than 'bytesPerLine */
		if( x < pi->width )
		{
		    i->data[image_pos].r = pi->pal[ pi->bufr[pcx_pos] ].r;
		    i->data[image_pos].g = pi->pal[ pi->bufr[pcx_pos] ].g;
		    i->data[image_pos].b = pi->pal[ pi->bufr[pcx_pos] ].b;
		    i->data[image_pos].a = pi->bufr[pcx_pos];
		    image_pos++;
		}
		pcx_pos++;
	    }
	}

    } else {

	/* 24 bit image */
	pcx_pos = image_pos = 0;
	for( y = 0 ; y < pi->height ; y++ )
	{
	    set_aside = image_pos; /* since they're muxed weird */
	    for( p = 0 ; p < pi->hdr.nplanes ; p++ )
	    {
		image_pos = set_aside;
		for( x = 0 ; x < pi->hdr.bytesPerLine ; x++ )
		{
		    /* the width might be different than 'bytesPerLine */
		    if( x < pi->width )
		    {
			if( p == PLANE_RED )
			    i->data[image_pos].r = pi->bufr[pcx_pos];

			if( p == PLANE_GREEN )
			    i->data[image_pos].g = pi->bufr[pcx_pos];

			if( p == PLANE_BLUE )
			    i->data[image_pos].b = pi->bufr[pcx_pos];

			image_pos++;
		    }
		    pcx_pos++;
		}
	    }
	}
    }
    return( i );
}


IMAGE *
PCX_Load(
	char * filename
)
{
    PCX_Image pi;
    FILE * in_fp;
    IMAGE * i;

    if (filename == NULL)
    {
	printf("Unable to open PCX: No filename.\n");
	return( NULL );
    }

    in_fp = fopen(filename, "r");
    if (in_fp == NULL)
    {
	printf ("%s: Unable to open file\n", filename);
	return( NULL );
    }


    /* make sure this is null'ed */
    pi.bufr = NULL;

    PCX_LoadHeader( &pi, in_fp );
    PCX_LoadPalette( &pi, in_fp );
    PCX_LoadDecodeData( &pi, in_fp );
    fclose( in_fp );

    /*PCX_Dump( &pi ); */

    i = PCX_toImage( &pi );

    if( pi.bufr )  free( pi.bufr );

    return( i );
}



/*////////////////////////////////////////////////////////////////////////////*/


/* PCX_encput
 *
 *  Subroutine for writing an encoded byte pair (or single byte 
 *  if it doesn't encode) to a file. It returns the count of 
 * bytes written, 0 if error
*/
int
PCX_encput(
	unsigned char byt,
	unsigned char cnt,
	FILE * fid
)
{
    if (cnt)
    {
	if ((cnt == 1) && (0xC0 != (0xC0 & byt)))
	{
	    if (EOF == putc((int )byt, fid))
		return(0);     /* disk write error (probably full) */

	    return(1);
	}
	else
	{
	    if (EOF == putc((int)0xC0 | cnt, fid))
		return (0);      /* disk write error */

	    if (EOF == putc((int)byt, fid))
		return (0);      /* disk write error */
	    return (2);
	}
    }
    return( 0 );
}


/* PCX_encLine
 *
 *  This subroutine encodes one scanline and writes it to a file.   
 *  It returns number of bytes written into outBuff, 0 if failed.
*/
int				/* number of bytes encoded */
PCX_encLine(
	unsigned char *inBuff,	/* pointer to scanline data */
	int inLen,		/* length of raw scanline in bytes */
	FILE *fp		/* file to be written to */
)
{
    unsigned char this, last;
    int srcIndex, i;
    register int total;
    register unsigned char runCount;     /* max single runlength is 63 */

    total = 0;
    runCount = 1;
    last = *(inBuff);       

    /*
	 Find the pixel dimensions of the image by calculating
	[XSIZE = Xmax - Xmin + 1] and [YSIZE = Ymax - Ymin + 1].
	Then calculate how many bytes are in a "run"
    */

    for (srcIndex = 1; srcIndex < inLen; srcIndex++)
    {
	this = *(++inBuff);
        if( this == last )   /* There is a "run" in the data, encode it */
	{
	    runCount++;

	    if( runCount == 63 )
	    {
		if( !(i = PCX_encput(last, runCount, fp)))   
		    return (0);

		total += i;
		runCount = 0;
	    }
	}
        else 	/* No "run"  -  this != last */ 
	{
	    if (runCount)
	    {
		if( !(i = PCX_encput(last, runCount, fp)))
		    return(0);

		total += i;
	    }

	    last = this;
	    runCount = 1;
	}
    } /* endloop */

    if (runCount)	/* finish up */
    {
	if( !(i = PCX_encput(last, runCount, fp)))   
	    return( 0 );

        return (total + i);
    }
    return( total );
}


int PCX_SaveHeader(
	PCX_Image * img,
	FILE * fp
)
{
    int c;

    if (!img || !fp)  return( -1 );

    endian_LittleWrite8( img->hdr.manufacturer, fp );	/* 0x0A */
    endian_LittleWrite8( img->hdr.version, fp );	/* 2,3,5 */
    endian_LittleWrite8( img->hdr.encoding, fp );	/* 0x01 */
    endian_LittleWrite8( img->hdr.bitsPerPixel, fp );	/* 1,2,4,8 */

    endian_LittleWrite16( img->hdr.xMin, fp );
    endian_LittleWrite16( img->hdr.yMin, fp );
    endian_LittleWrite16( img->hdr.xMax, fp );
    endian_LittleWrite16( img->hdr.yMax, fp );

    endian_LittleWrite16( img->hdr.hDpi, fp );
    endian_LittleWrite16( img->hdr.vDpi, fp );

    for( c=0 ; c<48 ; c++ )
    {
	endian_LittleWrite8( img->hdr.colormap[c], fp );
    }

    endian_LittleWrite8( img->hdr.reserved, fp );
    endian_LittleWrite8( img->hdr.nplanes, fp );

    endian_LittleWrite16( img->hdr.bytesPerLine, fp );
    endian_LittleWrite16( img->hdr.paletteInfo, fp );

    endian_LittleWrite16( img->hdr.hScreenSize, fp );
    endian_LittleWrite16( img->hdr.vScreenSize, fp );

    /* pad to 128 bytes */
    for( c=0 ; c<54 ; c++ )
    {
	endian_LittleWrite8( img->hdr.filler[c], fp );
    }

    return( 0 );
}


int PCX_SaveEncodeData(
	PCX_Image * img,
	FILE * fp
)
{
    int row;
    int rowoffs;

    if( !img || !fp )  return( -1 );

    fseek( fp, 128, SEEK_SET ); /* we might need this for MS-DOS */

    for( row=0 ; row < img->height ; row++ )
    {
	/* encode the buffer and output it to the file */
	rowoffs = row * img->hdr.bytesPerLine;

	/* this probably fails in fire on 24 bit images.  :(  */
	PCX_encLine( &img->bufr[rowoffs], img->hdr.bytesPerLine, fp );
    }

    return( 0 );
}


int PCX_SavePalette(
    	PCX_Image * img,
	FILE * fp
)
{
    int c;

    if (!img || !fp)  return( -1 );

    /* go to the end of the file */
    fseek( fp, 0, SEEK_END );

    /* output a '12' decimal */
    endian_LittleWrite8( 12, fp );  /* magic value */

    /* now dump out the palette data */
    for( c=0 ; c<256 ; c++ )
    {
	endian_LittleWrite8( img->pal[c].r, fp );
	endian_LittleWrite8( img->pal[c].g, fp );
	endian_LittleWrite8( img->pal[c].b, fp );
    }
    return( 0 );
}



PCX_Image *
PCX_fromImage(
	IMAGE * tosave,
	IMAGE * palette  /* pointer to a palette, or null if truecolor */
)
{
    int c, y, x;
    int ncolors = 0;
    PCX_Image * pi = NULL;

    /* allocate the structure */
    pi = (PCX_Image *) malloc( sizeof( PCX_Image ));
    if( !pi )  return( NULL );

    /* figure out stuff with the palette */
    if( palette )
    {
	ncolors = palette->width * palette->height;
    }

    /* fill in the header */
    memset( pi, 0, sizeof( PCX_Image ));
    pi->hdr.manufacturer = 0x0A;
    pi->hdr.version      = 5; /* we will always save the palette block */
    pi->hdr.encoding     = 1; 
    pi->hdr.bitsPerPixel = tosave->bits;
    pi->hdr.xMin         = 0;
    pi->hdr.yMin         = 0;
    pi->hdr.xMax         = tosave->width-1;
    pi->hdr.yMax         = tosave->height-1;
    pi->hdr.hDpi         = 72; /* hardcoded */
    pi->hdr.vDpi         = 72; /* hardcoded */

    pi->width  = tosave->width;
    pi->height = tosave->height;

    /* save the first 16 colors of the colormap to the header */
    for( c=0 ; (c<16) && (c<ncolors) ; c++ )
    {
	pi->hdr.colormap[c*3 + 0] = palette->data[c].r;
	pi->hdr.colormap[c*3 + 1] = palette->data[c].g;
	pi->hdr.colormap[c*3 + 2] = palette->data[c].b;
    }

    pi->hdr.reserved     = 0x00;
    pi->hdr.nplanes      = 1;
    pi->hdr.bytesPerLine = tosave->width;
    pi->hdr.paletteInfo  = 0x01;
    pi->hdr.hScreenSize  = tosave->width;
    pi->hdr.vScreenSize  = tosave->height;

    /* transfer over the palette data */
    for( c=0 ; (c<256) && (c<ncolors) ; c++ )
    {
	pi->pal[c].r = palette->data[c].r;
	pi->pal[c].g = palette->data[c].g;
	pi->pal[c].b = palette->data[c].b;
    }


    /* transfer over the image data */
    pi->bufrSize = (long )  pi->hdr.bytesPerLine 
		   * pi->hdr.nplanes 
		   * (1 + pi->hdr.yMax - pi->hdr.yMin);

    pi->bufr = (unsigned char *) malloc( 
			sizeof( unsigned char ) * pi->bufrSize );

    c=0;
    for( y=0 ;  y<tosave->height ; y++ )
    {
	for( x=0 ; x<pi->hdr.bytesPerLine ; x++ )
	{
	    if( x < tosave->width )
	    {
		pi->bufr[c++] = tosave->data[ (y * tosave->width) + x ].a;
	    } else {
		pi->bufr[c++] = 0;
	    }
	}
    }



    return( pi );
}


void
PCX_Save (
	char * filename, 
	IMAGE * tosave,
	IMAGE * colorKey /* pointer to a palette, or null if truecolor */
)
{
    PCX_Image * pi;
    FILE * fp;

    if( !tosave || !filename )  return;

#ifdef MSDOS
    fp = fopen( filename, "wb" );
#else
    fp = fopen( filename, "w" );
#endif


    if( !fp )  return;

    pi = PCX_fromImage( tosave, colorKey );

    if( pi )
    {
	PCX_SaveHeader( pi, fp );
	PCX_SaveEncodeData( pi, fp );

	if( colorKey )
	    PCX_SavePalette( pi, fp );

	if( pi->bufr )  free( pi->bufr );
	if( pi )  free( pi );
    }

    fclose( fp );
    
}


/*////////////////////////////////////////////////////////////////////////////*/


FileFormat
PCX_SupportedFile(
	char * filename
)
{
    int a, b;
    FILE * fp;

    /* check to see if the magic cookie is in the file. */
    /* that is to saym the first byte needs to be 0x0A, and the next 
       needs to be 2, 3, or 5 */

    fp = fopen( filename, "r" );
    if( !fp )  return( ff_UNKNOWN );

    a = endian_LittleRead8( fp );  /* 0x0A */
    b = endian_LittleRead8( fp );  /* 2, 3, 5 */
    fclose( fp );
    if( a == 0x0a )
    {
	if( (b == 2) || (b == 3) || (b == 5) )
	{
	    return( ff_PCX );
	}
    }

    return( ff_UNKNOWN ); 
}


/*////////////////////////////////////////////////////////////////////////////*/

int
PCX_Query(
        void
)
{
    /* return( QUERY_PALETTED | QUERY_TRUECOLOR ); */
    return( QUERY_PALETTED );
}


