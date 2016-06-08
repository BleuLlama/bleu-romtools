/*
 *  pngio
 *
 *   simple save/loader for png data (via image magick)
 *
 * $Id: PNG.c,v 1.10 2003/04/08 14:57:44 jerry Exp $
 *
 * $Log: PNG.c,v $
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"
#include "pngio.h"
#include "ls.h"
#include "io_util.h"
#include "turaco.h"

extern TuracoInstance * ti;

#include <wand/MagickWand.h>

#ifdef USE_GRAPHICSMAGICK
#include <wand/wand_api.h>
#include <wand/pixel_wand.h>
#include <wand/magick_wand.h>
#include <wand/drawing_wand.h>
#define MagickBooleanType int
#endif

/* 
	this started out as an interface that used libPNG, but
	after gouging my eyes out with spoons, I realized that
	doing it that way only brings about pain and suffering
	So i decided instead to use ImageMagick which provides
	more file type support, but I'm only using PNG. 

	Although, now that I'm stuck on adding a
	colormap to the imagemagick side of
	things, I'm starting to think
	that the original task
	might have been a
	better way to
	go.  Oh
	well.

	It should not be difficult to extend this code to add
	in support for more formats.  Later.
*/


IMAGE *
PNG_Load (
	char * filename
)
{
	FILE * fp = LS_ReadableOpen( filename );
	unsigned long x,y;
	unsigned long width=10,height=10;
	IMAGE * newimg = NULL;

	if (fp == NULL)
	{
		printf ("%s: Unable to open file\n", filename);
		return NULL;
	}
	fclose( fp );

	fprintf( stderr, "PNG Loading is not yet available.\n" );
	return NULL;


	/* now convert it to an IMAGE */
	do {
		MagickBooleanType status;
		MagickWand *image_wand;
		PixelWand **pixels;
		PixelIterator *iterator;

		MagickWandGenesis();
		image_wand = NewMagickWand();
		status = MagickReadImage( image_wand, filename );
		if( status == MagickFalse ) {
			printf( "%s: parse error\n", filename );
			return NULL;
		}
		
		/* insert loader code here. */
		width = (long)MagickGetImageWidth( image_wand );
		height = (long)MagickGetImageHeight( image_wand );
		newimg = Image_Create(width,height,8);
		if (newimg == NULL)
		{
			printf ("%s: Unable to load file.  Memory error\n", filename);
		} else {
			/* iterate over the loaded png, and shove it into our user buf */
			iterator = NewPixelIterator( image_wand );
			if( iterator == (PixelIterator *) NULL) {
				/* fail */
				fprintf( stderr, "Couldn't access pixels.  Fail.\n" );
			} else {
				for( y=0 ; y<height ; y++ )
				{
					pixels = PixelGetNextIteratorRow( iterator, &width );
					if( pixels == (PixelWand **)NULL) break;
					for( x=0 ; x<width ; x++ )
					{
						int dpos = (y * width) + x;
						newimg->data[dpos].r = PixelGetRed( pixels[x] );
						newimg->data[dpos].g = PixelGetGreen( pixels[x] );
						newimg->data[dpos].b = PixelGetBlue( pixels[x] );
					}
					/* for write (void) PixelSyncIterator( iterator ); */
				}
			}
			iterator = DestroyPixelIterator( iterator );
		}

		image_wand = DestroyMagickWand( image_wand );
		MagickWandTerminus();

	} while( 0 );

	printf( "%s: file not read. PNG read not yet supported.\n", filename );

	for( y=0 ; y<newimg->height ; y++ )
	{
		for( x=0 ; x<newimg->width ; x++ )
		{
		}
	}


	/* get rid of our temporary mem */
	return newimg;
}




/*
#define logprintf( ... ) printf( __VA_ARGS__ )
*/
#define logprintf( ... )

void
PNG_Save (
	char * filename,  	/* filename to save out to */
	IMAGE * tosave,		/* the image to save */
	IMAGE * palette		/* a palette disguised as an image - null if truecolor */
)
{
	FILE * fp = NULL;
	int width, height;
palette = NULL;

	if( tosave == NULL ) {
		fprintf( stderr, "Nothing to save.\n" );
		return;
	}
	height = tosave->height;
	width = tosave->width;

	if( width == 0 || height == 0)
	{
		fprintf(stderr, "%dx%d: Illegal image dimensions\n", 
			    tosave->width, tosave->height);
		return;
	}

	fp = LS_WritableOpen( filename );
	if( !fp || tosave == NULL || filename == NULL)
	{
		if( fp ) fclose(fp);
		printf("Didn't saved nothing.\n");
		return;
	}
	fclose( fp );


	do {
		MagickWand *image_wand;
		MagickBooleanType status;
		PixelWand * aColor;
		PixelWand **pixels;
		PixelIterator *iterator;
		int x,y;
		unsigned long nw;

		MagickWandGenesis();
		image_wand = NewMagickWand();

		aColor = NewPixelWand();
		if( palette ) {
			/* thought maybe this would trigger it to be paletted */
			/* nope. */
			PixelSetIndex( aColor, 0 );
		} else {
	    		PixelSetColor( aColor, "black" );
		}

		/* attempt to figure out how to get it to be paletted.  grr */
		status = MagickSetDepth( image_wand, 8 );
		logprintf( "MagickSetDepth %s\n", status?"TRUE":"false" );

		/* create our image */
		status = MagickNewImage( image_wand, (int)width, (int)height, aColor );
		logprintf( "MagickNewImage %s\n", status?"TRUE":"false" );

		status = MagickSetImageColorspace( image_wand, RGBColorspace );
		logprintf( "MagickSetImageColorspace %s\n", status?"TRUE":"false" );
		status = MagickSetImageDepth( image_wand, 8 );
		logprintf( "MagickSetImageDepth %s\n", status?"TRUE":"false" );
		status = MagickSetImageChannelDepth( image_wand, AllChannels, 8 );
		logprintf( "MagickSetImageChannelDepth %s\n", status?"TRUE":"false" );


		/* set the appropriate image format */
		if( palette ) {
			status = MagickSetImageFormat( image_wand, "PNG8" );
			logprintf( "MagickSetImageFormat %s\n", status?"TRUE":"false" );
			if( ti->up->trn ) {
				status = MagickSetType( image_wand, PaletteMatteType );
				logprintf( "MagickSetType PaletteMatteType %s\n", status?"TRUE":"false" );
			    	status = MagickSetImageType( image_wand, PaletteMatteType );
				logprintf( "MagickSetImageType PaletteMatteType %s\n", status?"TRUE":"false" );
			} else {
				status = MagickSetType( image_wand, PaletteMatteType );
				logprintf( "MagickSetType PaletteType %s\n", status?"TRUE":"false" );
				status = MagickSetImageType( image_wand, PaletteType );
				logprintf( "MagickSetImageType PaletteType %s\n", status?"TRUE":"false" );
			}
		} else {
			status = MagickSetImageFormat( image_wand, "PNG32" );
			logprintf( "MagickSetImageFormat %s\n", status?"TRUE":"false" );
			if( ti->up->trn ) {
				status = MagickSetType( image_wand, TrueColorMatteType );
				logprintf( "MagickSetType TrueColorMatteType %s\n", status?"TRUE":"false" );
				status = MagickSetImageType( image_wand, TrueColorMatteType );
				logprintf( "MagickSetImageType TrueColorMatteType %s\n", status?"TRUE":"false" );
			} else {
				status = MagickSetType( image_wand, TrueColorType );
				logprintf( "MagickSetType TrueColorType %s\n", status?"TRUE":"false" );
				status = MagickSetImageType( image_wand, TrueColorType );
				logprintf( "MagickSetImageType TrueColorType %s\n", status?"TRUE":"false" );
			}
		}

		/* set up the palette */
		if( palette ) {
			/* 
			status = AllocateImageColormap( GetImageFromMagickWand(image_wand), 255 );
			logprintf( "AllocateImageColormap %s\n", status?"TRUE":"false" );
			*/

			int cmapidx = 0;
			for( y=0 ; y<palette->height ; y++ )
			{
				for( x=0 ; x<palette->width ; x++ )
				{
					PixelWand * pw = NewPixelWand();
					int ppos = (y*palette->width) + x;
					int r = palette->data[ppos].r;
					int g = palette->data[ppos].g;
					int b = palette->data[ppos].b;
					PixelSetRed( pw, r );
					PixelSetGreen( pw, g );
					PixelSetBlue( pw, b );

					/*PixelSetIndex( aColor, cmapidx ); */
					status = MagickSetImageColormapColor( image_wand, cmapidx, pw );

					logprintf( "%s %d: %d %d %d\n", status==MagickTrue?"OK":"BAD", cmapidx, r, g, b );
					cmapidx++;
					DestroyPixelWand( pw );
				}
			}
		}

		/* iterate over the loaded png, and shove it into our user buf */
		iterator = NewPixelIterator( image_wand );
		if( iterator == (PixelIterator *) NULL) {
			/* fail */
			fprintf( stderr, "Couldn't access pixels.  Fail.\n" );
		} else {
			for( y=0 ; y<height ; y++ )
			{
				pixels = PixelGetNextIteratorRow( iterator, &nw );
				if( pixels == (PixelWand **)NULL) break;
				for( x=0 ; x<nw ; x++ )
				{
					int dpos = (y * width) + x;
					int idx = tosave->data[dpos].a; /* A contains index */
					int r = tosave->data[dpos].r;
					int g = tosave->data[dpos].g;
					int b = tosave->data[dpos].b;

					if( palette ) {
						PixelSetIndex( pixels[x], idx );
					} else {
						PixelSetRed( pixels[x], r );
						PixelSetGreen( pixels[x], g );
						PixelSetBlue( pixels[x], b );
					}

					/* set transparency */
					if( ti->up->trn ) {
						PixelSetAlpha( pixels[x], (idx==0)?0:255 );
					}
				}
				(void) PixelSyncIterator( iterator );
			}
		}
		iterator = DestroyPixelIterator( iterator );

		status = MagickWriteImage( image_wand, filename );

		if( status == MagickFalse ) {
			printf( "%s: file save error\n", filename );
		}

		image_wand = DestroyMagickWand( image_wand );
		MagickWandTerminus();

	} while( 0 );
}


/*////////////////////////////////////////////////////////////////////////////*/

FileFormat
PNG_SupportedFile(
	char * filename
)
{
	/* the PNG file signature */
	const unsigned char sig[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
	unsigned char buf[8];
	FILE *fp = LS_ReadableOpen( filename );
	int x;
	int bytesread;

	/* do some basic checks first. */
	if (!fp) return ff_UNKNOWN;

	/* now do the libpng checks */
	bytesread = fread(buf, 1, 8, fp);
	fclose(fp);

	if (bytesread != 8) return ff_UNKNOWN;

	for( x=0 ; x<8 ; x ++ ) {
		if( buf[x] != sig[x] ) return ff_UNKNOWN;
	}

	return ff_PNG;
}


/*////////////////////////////////////////////////////////////////////////////*/

int
PNG_Query(
        void
)
{
	/* XXXXX For now, let's just go truecolor, until i can fix PNG8 */
    /* return( QUERY_PALETTED );  */
    return( QUERY_TRUECOLOR );
}


