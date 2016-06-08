/*
 * romio.h
 *
 * $Id: romio.c,v 1.2 2003/05/20 03:41:18 jerry Exp $
 *
 * $Log: romio.c,v $
 * Revision 1.2  2003/05/20 03:41:18  jerry
 * First jab at tilemap generation
 *
 * Revision 1.1  2003/05/19 21:47:12  jerry
 * Moved the ROM IO routines out of turaco.[ch] into romio.[ch]
 *
 *
 */

#include <stdio.h>
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strlen */
#include <sys/types.h> /* fstat */
#include <sys/stat.h>  /* fstat */
#include "turaco.h"
#include "tilemap.h"
#include "params.h"
#include "coding.h"
#include "gfxlib.h"
#include "misc.h"
#include "errors.h"
#include "romio.h"

#define BUFLEN (1024)
#define FILEBUFSIZE (4096)
#define MAXPATH (1024)



/* _setBuffer
 *
 *  just a little something to put something into the roms
 */
void
_setBuffer( void * b, size_t l )
{
    int c = 0;
    char *str = "Jerry Carpenter  umlautllama.com  ";
    int len = strlen( str );
    char * cb = (char *) b;

    while( c<l )
    {
	cb[c] = str[c % len];
	c++;
    }
}


/* sssh. */
char * _find_file( char * path, char * filename );


/*
 * romio_FindROM    
 *
 *   This recurses down $(DRIVERS) to find the driver named "drivername".
 *   it puts the proper filename and path down in the buffer 'buf'
 *   if it was not found, buf[0] = NULL;
 */          
char *
romio_FindROM( TuracoInstance * ti, char * buf, int bufsize, char * romname )
{
    char dpath[MAXPATH];
    char * r;

    if( !buf || bufsize <= 0 || !ti->up )
    {
        if (buf)
            buf[0] = '\0';
        return( buf );
    }

    /* check the selected directory */
    if( ti->up->rom )
    {
	/* start off the path */
	(void) strncpy( dpath, ti->up->rom, MAXPATH );
    } else {
#ifdef MSDOS
	(void) sprintf( dpath, "." );
#else
	(void) snprintf( dpath, MAXPATH, "." );
#endif
    }

    /* tack on a slash if needed */
    if ( dpath[strlen(dpath)-1] != DIRSLASHC )
    {
        (void) strncat( dpath, DIRSLASH, MAXPATH );
    }

    /* recurse down that directory looking for the rom */
    r = _find_file( dpath, romname );
    if( r )
    {
        strncpy( buf, r, MAXPATH );
    }
    return( buf );
}



/* romio_LoadROMs
 *
 *  loads the ROM data as selected in the user params 
 */
int
romio_LoadROMs( TuracoInstance * ti )
{
    int c, d, e;
#ifdef CRAFTYBUTFLAWEDROMCHECK
    int min, max;
    int min2, max2;
#endif
    int maxRomSpace = 0;
    struct stat statbuf;

    char * r;
    char buf[MAXPATH];

    FILE * fileROM;
    char filebuf[FILEBUFSIZE];
    int bytesread;

    /* just in case... */
    if( !ti )       return( ERR_NO_INSTANCE );
    if( !ti->up )   return( ERR_NO_PARAMS_STRUCT );
    if( !ti->gd )   return( ERR_NO_DRIVER_STRUCT );
    if( !ti->gd )   return( ERR_NO_ROMFILE_DESCS );

    if( ti->up->bnk < 0 || ti->up->bnk-1 > ti->gd->ndecodes )
	return( ERR_BANK_OUT_OF_RANGE );

    /* for each graphics rom */
    /*	determine if it is needed for the bank selected ; set the flag */
    for( c=0 ; c< ti->gd->nroms ; c++)
    {
#ifdef CRAFTYBUTFLAWEDROMCHECK
	min = ti->gd->romFileDescs[c].offset;
	max = ti->gd->romFileDescs[c].offset + ti->gd->romFileDescs[c].size -1;

	d = ti->up->bnk;
	
	min2 = ti->gd->gfxDecodes[d].startaddress;
	max2 = min2 
		+ ti->gd->gfxDecodes[d].nsprites 
		* ti->gd->gfxDecodes[d].charincrement/8 
		- 1;

	/* if either min2 or maxw are within min-max, it is needed */
	if(    (min2 >= min && min2 <= max)
	    || (max2 >= min && max2 <= max) )
	{
	    ti->gd->romFileDescs[c].needed = 1;

	    if (max2 >= maxRomSpace) maxRomSpace = max2;
	}
#else
	maxRomSpace += ti->gd->romFileDescs[c].size;

	/* JJJ force it to be needed... */
	ti->gd->romFileDescs[c].needed = 1;
#endif
    }

    /* allocate ROM buffer size needed */
    ti->romBuffer = (char *) calloc( maxRomSpace, 1 );
    ti->romBufferSize = maxRomSpace;
    if( !ti->romBuffer )  return( ERR_NO_MEMORY );
    
    /* zero the buffer */
    _setBuffer( ti->romBuffer, maxRomSpace );

    printf( "\n== ROMs ==\n");

    /* find the rom in the directory tree */
    if( ti->up->rom )
	strncpy( buf, ti->up->rom, MAXPATH );
    else 
	strncpy( buf, ".", MAXPATH );

    /* load the roms themselves into the buffer */
    for( c=0 ; c< ti->gd->nroms ; c++ )
    {
	if( ti->gd->romFileDescs[c].needed )
	{
	    r = romio_FindROM( ti, buf, MAXPATH, 
				ti->gd->romFileDescs[c].filename );
	    if ( r )
	    {
		printf( " %10s: %s\n", ti->gd->romFileDescs[c].filename, r );

		/* check the size to see if it's too big. */
		if( stat( r, &statbuf ))
		{
		    fprintf( stderr, "Error stat'ing file!\n" );
		    return( ERR_BAD_FILE );
		}

		if( statbuf.st_size != ti->gd->romFileDescs[c].size )
		{
		    fprintf( stderr, "Source file was not found or the wrong size.\n" );
		    fprintf( stderr, "Was %d, expecting %d.\n", 
				(int)statbuf.st_size,
				(int)ti->gd->romFileDescs[c].size );
		    fprintf( stderr, "Starting with a blank buffer..." );

		} else {

		    fileROM = fopen( r, "rb" );
		    if( fileROM )
		    {
			/* starting position in the buffer to load into */
			d = ti->gd->romFileDescs[c].offset; 

			/* read from the file into the buffer... */
			do
			{
			    bytesread = fread( filebuf, 1, FILEBUFSIZE, fileROM );
			    for( e = 0 ; e < bytesread ; e++ )
				ti->romBuffer[d+e] = filebuf[e];

			    d += bytesread;
			} while( bytesread );

			/* done.  close the file. */
			fclose( fileROM );
		    }
		}
	    }
	}
    }

    /* if a rom is not found, continue anyway */
    return( ERR_NONE );
}



/* romio_SaveROMs
 *
 *  saves out the internal ROM data to the rom files
 */
int
romio_SaveROMs( TuracoInstance * ti, int format )
{
    int c;
    FILE * outfile;
    char rpath[MAXPATH];

    if (!ti)            return( ERR_NO_INSTANCE );
    if (!ti->gd )       return( ERR_NO_DRIVER_STRUCT );
    if (!ti->up )       return( ERR_NO_PARAMS_STRUCT );

    printf( "\n== Generating ROMs ==\n");
    /* determine which roms are affected */
    for( c=0 ; c< ti->gd->nroms ; c++)
    {
	/* if rom number [c] is needed... */
	if( ti->gd->romFileDescs[c].needed )
	{
	    /* save out each rom file */
#ifdef MSDOS
	    (void) sprintf( rpath,
		    "%s/%s", 
		    ti->up->rod, ti->gd->romFileDescs[c].filename );
#else
	    (void) snprintf( rpath, MAXPATH, 
		    "%s/%s", 
		    ti->up->rod, ti->gd->romFileDescs[c].filename );
#endif

	    printf("  %s\n", rpath );

	    if( (outfile = fopen( rpath, "wb" )))
	    {
		if( format == ROM_FORMAT_BINARY )
		{
		    fwrite( 
			ti->romBuffer+(ti->gd->romFileDescs[c].offset), 
			ti->gd->romFileDescs[c].size, 
			1, outfile);
		} else {
		    /* IHX http://www.8052.com/tutintel.phtml	 */
		    printf("Selected format is unsupported.\n" );
		    exit( -1002 );
		}
		fclose( outfile );
	    }
	}
    }

    return( ERR_NONE );
}
