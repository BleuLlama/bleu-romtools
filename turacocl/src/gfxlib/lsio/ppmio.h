/*
 *  ppmio
 *
 *   simple save/loader for ppm data
 *
 * $Id: ppm.h,v 1.6 2003/04/08 14:57:44 jerry Exp $
 *
 * $Log: ppm.h,v $
 * Revision 1.6  2003/04/08 14:57:44  jerry
 * Added the 'Query' operation.
 *
 * Revision 1.5  2003/04/02 21:53:20  jerry
 * Updated a lot of stuff.
 * changed 'xxx_ValidFile' to xxx_SupportedFile
 *
 * Revision 1.4  2003/03/31 17:31:40  jerry
 * Added pcx headers
 *
 * Revision 1.3  2003/03/31 17:19:56  jerry
 * Updated everything to read "Jerry Carpenter"  ;)
 *
 * Revision 1.2  2003/03/31 17:07:54  jerry
 * Moved out the load/save methods from image.c into ls.c
 * moved the io_util back up to gfxlib/
 *
 * Revision 1.1  2003/03/07 20:23:43  jerry
 * Brought over the PPM library to use here for loading and saving images.
 * We'll also use the PPM image structure for our images.
 *
 * Revision 1.1  1999/12/23  19:02:51  sdlpci
 * Initial revision
 *
 *
 */

#include "image.h"
#include "ls.h"		/* for "FileFormat" */


#ifndef __PPMIO_H__
#define __PPMIO_H__

IMAGE * PPM_Load( char * filename );
void    PPM_Save( char * filename, IMAGE * tosave, int as_format );

/* set 'as_format' to one of these two: */
#define SAVE_ASCII  (0)
#define SAVE_BINARY (1)

FileFormat PPM_SupportedFile( char * filename );

int PPM_Query( void );



#endif
