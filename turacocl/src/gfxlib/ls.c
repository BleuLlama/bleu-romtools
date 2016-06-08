/*
 *  ls.c
 *
 *   image load and save functions
 *
 * $Id: ls.c,v 1.6 2003/04/08 14:58:31 jerry Exp $
 *
 * $Log: ls.c,v $
 * Revision 1.6  2003/04/08 14:58:31  jerry
 * Implemented the 'Query' operation
 * Save now has a palette associated with it (for paletted images)
 * Lots of other stuff I can't remember right now.
 *
 * Revision 1.5  2003/04/02 21:53:20  jerry
 * Updated a lot of stuff.
 * changed 'xxx_ValidFile' to xxx_SupportedFile
 *
 * Revision 1.4  2003/03/31 17:38:21  jerry
 * updated the structure
 * changed ppmio to ppm
 * added pcx
 *
 * Revision 1.3  2003/03/31 17:31:40  jerry
 * Added pcx headers
 *
 * Revision 1.2  2003/03/31 17:19:56  jerry
 * Updated everything to read "Jerry Carpenter"  ;)
 *
 * Revision 1.1  2003/03/31 17:07:54  jerry
 * Moved out the load/save methods from image.c into ls.c
 * moved the io_util back up to gfxlib/
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"	/* image structure */
#include "ls.h"		/* for the FileFormat enum */
#ifdef USE_MAGICK
#include "pngio.h"	/* for PNG loader/savers */
#endif
#include "ppmio.h"	/* for PPM loader/savers */
#include "pcxio.h"	/* for PCX loader/savers */


FileFormat
LS_DetermineFormat(
	char * filename
)
{
    FileFormat ff;
    
         if ( (ff = PPM_SupportedFile( filename )) != ff_UNKNOWN) return( ff );
    else if ( (ff = PCX_SupportedFile( filename )) != ff_UNKNOWN) return( ff );
#ifdef USE_MAGICK
    else if ( (ff = PNG_SupportedFile( filename )) != ff_UNKNOWN) return( ff );
#endif

    return( ff_UNKNOWN );
}


IMAGE *
LS_Load(
	char * filename,
	FileFormat ff
)
{
    switch( ff )
    {
#ifdef USE_MAGICK
	case( ff_PNG ):
	    return( PNG_Load( filename ));
#endif

	case( ff_PCX ):
	    return( PCX_Load( filename ));

	case( ff_PPM_BINARY ):
	case( ff_PPM_ASCII ):
	case( ff_PPM ):
	    return( PPM_Load( filename ));

	case( ff_UNKNOWN ):
	default:
	    break;
    }
    return( NULL );
}


void
LS_Save(
	char * filename,
	IMAGE * toSave,
	IMAGE * palette,
	FileFormat ff
)
{
    switch( ff )
    {
#ifdef USE_MAGICK
	case( ff_PNG ):
	    (void)( PNG_Save( filename, toSave, palette ));
	    return;

#endif
	case( ff_PPM ):
	case( ff_PPM_BINARY ):
	    (void)( PPM_Save( filename, toSave, SAVE_BINARY ));
	    return;

	case( ff_PPM_ASCII ):
	    (void)( PPM_Save( filename, toSave, SAVE_ASCII ));
	    return;

	case( ff_PCX ):
	    (void)( PCX_Save( filename, toSave, palette ));
	    return;

	case( ff_UNKNOWN ):
	default:
	    fprintf( stderr, "Unable to save to desired file format" );
	    break;

    }
}


int
LS_Query(
	FileFormat ff
)
{
    switch( ff )
    {
#ifdef USE_MAGICK
	case( ff_PNG ):
	    return( PNG_Query() );
#endif
	case( ff_PPM ):
	case( ff_PPM_BINARY ):
	case( ff_PPM_ASCII ):
	    return( PPM_Query() );

	case( ff_PCX ):
	    return( PCX_Query() );

	case( ff_UNKNOWN ):
	default:
	    return( QUERY_UNKNOWN );
    }
    return( QUERY_UNKNOWN );
}


FILE *
LS_ReadableOpen(
	char * filename
)
{
	FILE * fp = NULL;

	/* check filename is ok */
	if( !filename ) return NULL;

	/* open for reading... */
	fp = fopen( filename, "rb" );
	if( !fp ) return NULL;

	/* It's Okay! */
	return fp;
}

FILE *
LS_WritableOpen(
	char * filename
)
{
	FILE * fp = NULL;

	/* check filename is ok */
	if( !filename ) return NULL;

	/* open for reading... */
	fp = fopen( filename, "wb" );
	if( !fp ) return NULL;

	/* It's Okay! */
	return fp;
}
