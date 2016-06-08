/* main.c
 *
 * $Id: main.c,v 1.33 2003/05/29 21:22:52 jerry Exp $
 */

#include <stdio.h>
#include <stdlib.h>  /* malloc */
#include <string.h>  /* strcmp */
#include "params.h"
#include "turaco.h"
#include "romio.h"
#include "errors.h"
#include "locale.h"
#include "app.h"



/* version
 *
 *  display version information
 */
void version( void )
{
    fprintf( stderr, "Turaco CL  v" BL_VERSION "  " BL_DATE "\n" );
    fprintf( stderr, "    by Scott Lawrence\n" );
    fprintf( stderr, "       yorgle@gmail.com\n" );
    fprintf( stderr, "       http://www.umlautllama.com\n" );
#ifdef USE_MAGICK
    fprintf( stderr, "    Using ImageMagick (MagickWand) for PNG support\n" );
#endif
    fprintf( stderr, "\n" );
}


/* usage
 *
 *  display usage information
 */
void usage( char ** argv )
{
    version();
    fprintf( stderr, "%s: \t%s [%s]\n\n" , 
			locale_("Usage"),
			argv[0],
			locale_("options") );
    fprintf( stderr, "%s:\n", locale_("Options") );
    params_Usage( stderr );
}



/* some globals. */
TuracoInstance * ti = NULL;


/* myexit
 *
 *  clear memory, and clean up after ourselves.
 */
void myexit( void )
{
    /* be sure to wipe out the memory we allocated */
    turaco_Destroy( ti );
}


/* main
 *
 *  it's like the main routine or something
 */
int main( int argc, char ** argv )
{
    int r = 0;

    /* register an exit handler to clean up everything */
    (void) atexit( myexit );

    /* setup the proper locale code */
    locale_SetCode( "en" ); /* English for now */

    ti = turaco_Create();
    if( !ti )
    {
	fprintf( stderr, "%s: %s.\n", 
			locale_( "ERROR" ),
			error_toString( ERR_NO_MEMORY ));
	return( ERR_NO_MEMORY );
    }

    /* parse the user parameters on the command line */
    ti->up = params_Parse( ti->up, argc, argv );

    /* check for -v flag */
    if( ti->up && ti->up->v )
    {
	version();
	return( ERR_NONE );
    }

    /* check for -h flag */
    if( ti->up && ti->up->h )
    {
	usage( argv );
	return( ERR_NONE );
    }

    if( ti->up->gui )
    {
	run_gui_app();
	version();
	return( ERR_NONE );
    }

    if (!ti->up || ti->up->h || (r = params_Verify( ti->up )))
    {
	/* bad params, or user wanted help */
	if (r != ERR_NO_TROMS && r != ERR_NO_TDRIVERS)
	    usage( argv );
	return( ERR_NONE );
    }


    /* try to find and load the proper driver... */
    r = turaco_LoadDriver( ti );
    if( r )  return( r );

    /* verify parameters based on the loaded driver */
    r = turaco_VerifyDriver( ti );
    if( r )  return( r );

    /* set up the proper filenames */
    r = turaco_ConfigureFilenames( ti );
    if( r )  return( r );

    if( ti->up->dmp == 1 )
	game_Dump( ti->gd );

    if( !strcmp( ti->up->inf, "IMG" ) )    /***  IMG to ROM  ***/
    {
    /* These might be unnecessary */
	/* determine and load the necessary roms (BACKFILL ROMSPACE) */
	r = romio_LoadROMs( ti );
	if( r != ERR_NONE ) {
		fprintf( stderr, "%s: %s.\n", 
				locale_( "ERROR" ),
				error_toString( r ));
		return( r );
	}

	/* decode the rom into an image buffer */
	/* this might be unnecessary */
	r = turaco_DecodeImageFromRomBuffer( ti );
	if( r != ERR_NONE ) {
		fprintf( stderr, "%s: %s.\n", 
				locale_( "ERROR" ),
				error_toString( r ));
		return( r );
	}
    /* */

	/* load in image file(s) */
	r = turaco_LoadImages( ti );
	if( r != ERR_NONE ) {
		fprintf( stderr, "%s: %s.\n", 
				locale_( "ERROR" ),
				error_toString( r ));
		return( r );
	}

	/* import the tilemap image information */ /* this is a hack. sorry. */
	turaco_OverlayTilemapImages( ti );

	/* encode and save out the roms */
	r = turaco_EncodeRomBufferFromImage( ti );
	if( r != ERR_NONE ) {
		fprintf( stderr, "%s: %s.\n", 
				locale_( "ERROR" ),
				error_toString( r ));
		return( r );
	}

	r = romio_SaveROMs( ti, ROM_FORMAT_BINARY );
	if( r != ERR_NONE ) {
		fprintf( stderr, "%s: %s.\n", 
				locale_( "ERROR" ),
				error_toString( r ));
		return( r );
	}
    }

    if( !strcmp( ti->up->inf, "ROM" ) )    /*** ROM to IMG  ***/
    {
	/* determine and load the necessary roms */
	r = romio_LoadROMs( ti );
	if( r != ERR_NONE ) {
		fprintf( stderr, "%s: %s.\n", 
				locale_( "ERROR" ),
				error_toString( r ));
		return( r );
	}

	/* decode the rom into an image buffer */
	r = turaco_DecodeImageFromRomBuffer( ti );
	if( r != ERR_NONE ) {
		fprintf( stderr, "%s: %s.\n", 
				locale_( "ERROR" ),
				error_toString( r ));
		return( r );
	}

	/* roms are loaded! */

	/* save out the bank, key, and checkerboard image files */
	r = turaco_SaveImages( ti );
	if( r != ERR_NONE ) {
		fprintf( stderr, "%s: %s.\n", 
				locale_( "ERROR" ),
				error_toString( r ));
		return( r );
	}
    }

    return( ERR_NONE );
}

#ifdef IMGIOTEST
#include "pcx.h"
#include "ppm.h"

int testmain( int argc, char ** argv )
{
    IMAGE * i = PCX_Load( "24bit.pcx" );
    PPM_Save( "xv.ppm", i, SAVE_ASCII );
    Image_Destroy( i );
    return( 0 );
}
#endif
