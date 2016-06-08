/*
 *  ls.h
 *
 *   image load and save functions
 *
 * $Id: ls.h,v 1.3 2003/04/08 14:58:31 jerry Exp $
 *
 * $Log: ls.h,v $
 * Revision 1.3  2003/04/08 14:58:31  jerry
 * Implemented the 'Query' operation
 * Save now has a palette associated with it (for paletted images)
 * Lots of other stuff I can't remember right now.
 *
 * Revision 1.2  2003/03/31 17:31:40  jerry
 * Added pcx headers
 *
 * Revision 1.1  2003/03/31 17:07:54  jerry
 * Moved out the load/save methods from image.c into ls.c
 * moved the io_util back up to gfxlib/
 *
 *
 *
 */
#ifndef __LS_H__
#define __LS_H__

#include "image.h"

typedef enum {
    ff_UNKNOWN = 0,
    ff_PPM_BINARY,
    ff_PPM_ASCII,
    ff_PPM,
#ifdef USE_MAGICK
    ff_PNG,
#endif
    ff_PCX
} FileFormat;

FileFormat LS_DetermineFormat( char * filename );

void LS_Save( char * filename, IMAGE * toSave, IMAGE * palette, FileFormat ff );
IMAGE * LS_Load( char * filename, FileFormat ff );


#define QUERY_UNKNOWN	(0x00)	/* bad file format? */
#define QUERY_TRUECOLOR	(0x01)	/* there is support for truecolor images */
#define QUERY_PALETTED 	(0x02)	/* there is support for paletted images */

int LS_Query( FileFormat ff );

FILE * LS_ReadableOpen( char * filename );
FILE * LS_WritableOpen( char * filename );

#endif
