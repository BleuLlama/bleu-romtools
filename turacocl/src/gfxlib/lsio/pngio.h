/*
 *  pngio
 *
 *   simple save/loader for png data (via image magick)
 *
 * $Id: PNG.h,v 1.6 2003/04/08 14:57:44 jerry Exp $
 *
 * $Log: PNG.h,v $
 */

#include "image.h"
#include "ls.h"		/* for "FileFormat" */


#ifndef __PNGIO_H__
#define __PNGIO_H__

IMAGE * PNG_Load( char * filename );
void    PNG_Save( char * filename, IMAGE * tosave, IMAGE * palette );
FileFormat PNG_SupportedFile( char * filename );

int PNG_Query( void );

#endif
