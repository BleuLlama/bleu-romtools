/*
	comPACtor

	load in an image file from the command line
	print out the RLE encoded bitmap to stdout

	(c)2007 Scott Lawrence
	Use this for whatever, but don't blame me if something breaks.
*/


#include <sys/types.h>	/* for fstat() */
#include <sys/stat.h>	/* for fstat() */
#include <string.h>	/* for memset() */
#include <stdlib.h>	/* malloc() */

#include <math.h>	/* for sqrt() */

#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include "palette.h"

void usage( void )
{
	fprintf( stderr, "Usage:  \tcomPACtor [options] -Image foo.img\n" );
	fprintf( stderr, "\toptions:\n" );
	fprintf( stderr, "\t\t -Scan\t scan the image, build palette.html\n" );
	fprintf( stderr, "\t\t -Asm\t scan the image, generate .asm source\n" );
	fprintf( stderr, "	FILE.BMP needs to be a 24 bit truecolor .bmp file\n" );
}

unsigned char ARGV_scan = 0;
unsigned char ARGV_asm  = 0;
unsigned char ARGV_help = 0;
char * ARGV_filename = NULL;

int processArgv( int argc, char **argv )
{
	int x;

	for( x=1 ; x<argc ; x++ )
	{
		     if( !strcasecmp( argv[x], "-h"     )) { ARGV_help = 1; }
		else if( !strcasecmp( argv[x], "-?"     )) { ARGV_help = 1; }
		else if( !strcasecmp( argv[x], "-help"  )) { ARGV_help = 1; }
		else if( !strcasecmp( argv[x], "-usage" )) { ARGV_help = 1; }

		else if( !strcasecmp( argv[x], "-scan"  )) { ARGV_scan = 1; }
		else if( !strcasecmp( argv[x], "-asm"   )) { ARGV_asm = 1; }
		else if( !strcasecmp( argv[x], "-image" )) {
			x++;
			ARGV_filename = argv[x];
		}

		else {
			fprintf( stderr, "%s: Unknown option\n", argv[x] );
			ARGV_help = 1;
		}
	}	

	if( ARGV_scan ) fprintf( stderr, "-Scan option detected\n" );
	if( ARGV_asm  ) fprintf( stderr, "-Asm  option detected\n" );

	if( ARGV_filename == NULL  || ARGV_help ) {
		usage();
		return( 1 );
	}
	return( 0 );
}


int main( int argc, char **argv )
{
	SDL_Surface * theImage;

	int ret = processArgv( argc, argv );

	if( ret ) {
		return -1;
	}

	theImage = IMG_Load( ARGV_filename );

	if( !theImage ) {
		fprintf( stderr, "ERROR: %s: %s\n",
				ARGV_filename, IMG_GetError() );
		return -3;
	}

	printf( "Image is %d by %d\n", theImage->w, theImage->h );

	if( ARGV_scan )
	{
/*
		BMP_DumpPalette( bi, "palette.html" );
*/
	}

	if( ARGV_asm )
	{
/*
		BMP_DumpAsm( bi );
*/
	}

	SDL_FreeSurface( theImage );
	return( 0 );
}
