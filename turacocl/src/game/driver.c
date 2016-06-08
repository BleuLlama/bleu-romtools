/*
 * driver.c
 *
 * $Id: driver.c,v 1.24 2003/05/29 18:13:01 jerry Exp $
 *
 * $Log: driver.c,v $
 * Revision 1.24  2003/05/29 18:13:01  jerry
 * Made the indexing for the Dump match the driver.
 * (numbers start at 1, not zero)
 *
 * Revision 1.23  2003/05/29 04:53:44  jerry
 * Fixed a bug where a printf might fail if there was no description
 *
 * Revision 1.22  2003/05/29 04:46:41  jerry
 * Added some more output to display the driver name.
 *
 * Revision 1.21  2003/05/29 04:16:51  jerry
 * Tilemap generation working!  YAY!
 *
 * Revision 1.20  2003/05/20 03:41:17  jerry
 * First jab at tilemap generation
 *
 * Revision 1.19  2003/05/15 19:50:33  jerry
 * Intermediary files.  Got stuck when making the tilemap saver
 *
 * Revision 1.18  2003/05/15 01:28:39  jerry
 * Fixed the driver to be correct
 * Implemented the tilemap_SetContains() function
 *
 * Revision 1.17  2003/05/14 20:33:45  jerry
 * Moved VerifyDriver() into turaco.c from params.c
 * Implemented the tileset checker in VerifyDriver()
 * If tile set wasn't found, it lists them out.
 *
 * Revision 1.16  2003/05/13 04:37:45  jerry
 * changed some text output a little.
 *
 * Revision 1.15  2003/05/05 15:50:19  jerry
 * Added tilemap.[ch]
 * Added the remaining bit of driver and dumper for the tilemaps
 *
 * Revision 1.14  2003/05/03 06:10:09  jerry
 * Updated the checkerboard
 * finished changing the en/de code routines
 * Starting to add the tilemap driver
 *
 * Revision 1.13  2003/04/12 21:19:54  jerry
 * Removed my _CIstrncmp function since there is a stdlib strncasecmp() instead
 *
 * Revision 1.12  2003/04/11 21:01:41  jerry
 * Moved stuff out into misc.h (MIN, MAX, os-dependant #defines, etc)
 * FINALLY fixed the stupid MS-DOS bug.  fopen( filename, "rb" )
 *
 * Revision 1.11  2003/04/09 02:09:05  jerry
 * Added some switches for the MS-DOS path selection \ instead of /
 * etc.
 *
 * Revision 1.10  2003/04/08 15:33:45  jerry
 * Added in the drivername-from-driverfilename yanker
 *
 * Revision 1.9  2003/04/08 15:00:22  jerry
 * Added the palette autoscaler (BRIGHTNESS_HEURISTIC)
 * Added MIN and MAX macros for the above.
 * Added a note for the 'undefined' name issue.  (JJJ)
 *
 * Revision 1.8  2003/03/31 04:24:43  jerry
 * Finished the update to seperate everything around.
 *
 * Revision 1.7  2003/03/19 04:50:27  jerry
 * Fixed the ROMDIR not searched properly bug
 *
 * Revision 1.6  2003/03/18 19:16:18  jerry
 * put the orientation hack back in.
 *
 * Revision 1.5  2003/03/18 17:21:35  jerry
 * Got rid of the 'new style' orientation setting.
 *
 * Revision 1.4  2003/03/18 05:54:33  jerry
 * almost completely done.
 * Orientation stuff integrated in.
 * Trying to get it reading the roms from the spcified directory
 *
 * Revision 1.3  2003/03/12 06:26:36  jerry
 * Wrote the rom reader
 *
 * Revision 1.2  2003/03/11 19:13:13  jerry
 * Updated to use driver.[ch] instead of game.[ch]
 * Added turaco.c to the makefile
 *
 * Revision 1.1  2003/03/11 19:10:44  jerry
 * Moved game.[ch] to driver[ch]
 * Initial version of turaco.[ch]
 *
 * Revision 1.8  2003/03/11 17:10:42  jerry
 * updated the parameters to be a single call for argv and env variables
 * added support for the images for loading/saving graphics into game.[ch]
 *
 * Revision 1.7  2003/03/11 06:10:00  jerry
 * Full game parser in effect.  YAY!
 *
 * Revision 1.6  2003/03/11 03:30:16  jerry
 * pulled the params out of main.c
 * made all of the function protos more consistantly named
 * it checks the driver defined on the command line
 *
 * Revision 1.5  2003/03/11 02:03:59  jerry
 * comparison is case insensitive now.
 * checks files in a directory first, then subdirs
 *
 * Revision 1.4  2003/03/10 22:52:12  jerry
 * Builds properly on OS X
 *
 * Revision 1.3  2003/03/10 22:47:10  jerry
 * Checks for "pacman.INI" now as well.
 * Should probably also check for "PACMAN.INI"... eh.. later.
 *
 * Revision 1.2  2003/03/10 22:20:36  jerry
 * Added in the recursive driver finder.
 * pass in "driver", it finds "$(TDRIVERS)/path/to/driver.ini"
 *
 * Revision 1.1  2003/03/10 21:00:04  jerry
 * Changed INI to pass around a pointer to the list
 * moved the parser into game.c (new)
 * updated the makefile to add game.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>    /* getenv */
#include <sys/types.h> /* dir, stat */
#include <sys/stat.h>  /* stat */
#include <dirent.h>    /* dir */
#include <ctype.h>     /* toupper */
#ifdef SunOS
#include <strings.h>   /* strncpy, strncat */
#else
#include <string.h>   /* strncpy, strncat */
#endif
#include "ini.h"
#include "gfxlib.h"
#include "misc.h"
#include "driver.h"


#define MAXPATH (1024)


/* the heuristic for the palette scaler.  
   if no values are brighter than this value, the palette is scaled brighter */
#define BRIGHTNESS_HEURISTIC (240)


/*
 * _find_file
 * 
 *  a small internal helper to find a specific [[filename]] starting at
 *  a certain [[path]].  If it finds it, it returns the complete path.
 *  if it doesn't find it, it returns a null, and [[path]] is unchanged
 */
char * _find_file( char * path, char * filename )
{
    char dpath[MAXPATH];
    char * r;
    DIR * dd;
    struct dirent * dp;
    struct stat sb;

    dd = opendir( path );
    if( dd )
    {
	/* for each file... */
	while(( dp = readdir(dd)) != NULL)
	{
	    /* dpath = path/direntry */
	    (void) strncpy( dpath, path, MAXPATH );
	    (void) strncat( dpath, dp->d_name, MAXPATH );

	    (void)stat( dpath, &sb );

	    /* if it's a dir, try to recurse down it too. */
	    if( !S_ISDIR( sb.st_mode ) )
	    {
		/* compare the filenames, lowercase */
		if (!strncasecmp( dp->d_name, filename, MAXPATH ) )
		{
		    /* found the file! */
		    (void) strncpy( path, dpath, MAXPATH );
		    return( path );
		}
	    }
	}

	/* start over. ;) */
	rewinddir( dd );

	/* now check the subdirs */
	while(( dp = readdir(dd)) != NULL)
	{
	    /* dpath = path/direntry */
	    (void) strncpy( dpath, path, MAXPATH );
	    (void) strncat( dpath, dp->d_name, MAXPATH );

	    (void)stat( dpath, &sb );

	    /* if it's a dir, try to recurse down it too. */
	    if( S_ISDIR( sb.st_mode ) )
	    {
		if( dp->d_name && dp->d_name[0] != '.')
		{
		    (void) strncat( dpath, DIRSLASH, MAXPATH );
		    r = _find_file( dpath, filename );
		    if( r )
		    {
			closedir( dd );
			return( r );
		    }
		}
	    }
	}
	closedir( dd );
    }
    return( NULL );
}


/*
 * game_FindDriver
 *
 *   This recurses down $(DRIVERS) to find the driver named "drivername".
 *   it puts the proper filename and path down in the buffer 'buf'
 *   if it was not found, buf[0] = NULL;
 */
char * 
game_FindDriver( UserParams * userParams, char * buf, int bufsize )
{
    char dpath[MAXPATH];
    char lfn[MAXPATH];
    char * driverpath;
    char * r;

    if( !buf || bufsize <= 0 || !userParams || !userParams->drv)
    {
	if (buf)
	    buf[0] = '\0';
	return( buf );
    }

    /* check the TDRIVERS directory tree */
    driverpath = userParams->tdrivers;

    /* start off the path */
    (void) strncpy( dpath, driverpath, MAXPATH );

    /* tack on a slash if needed */
    if ( driverpath[strlen(driverpath)-1] != DIRSLASHC )
    {
	(void) strncat( dpath, DIRSLASH, MAXPATH );
    }

    (void) strncpy( lfn, userParams->drv, MAXPATH );
    (void) strncat( lfn, ".ini", MAXPATH );

    /* recurse down that directory looking for drivername.ini */
    r = _find_file( dpath, lfn );
    if( r )
    {
	strncpy( buf, r, MAXPATH );
    }
    return( buf );
}


/* game_ExtractName
 *
 *  extracts out the game name from the passed in filepath
 *   ie:  /users/jerry/driver.ini  becomes  driver
 *        XXXXXXXXXXXX/driver.XXX  becomes  driver
 *        driver.XXX  becomes  driver
 */
char *
game_ExtractName(
	char * inputText
)
{
    int c,d;
    int slashIndex;

    if( !inputText )  return( NULL );

    /* first yank off the extension if it exists */
    for( c=strlen(inputText) ; c>=0 ; c-- )
    {
	if( inputText[c] == '.' )
	{
	    inputText[c] = '\0'; /* end the string right there */
	    c = -1;		/* force the bail out */
	}
    }

    /* then yank off the path if it exists */
    for( c=strlen(inputText), slashIndex=0 ; (c>=0) && (slashIndex==0) ; c-- )
    {
	if(    (inputText[c] == '/')
	    || (inputText[c] == '\\') ) /* YAY for MS-DOS! */
	{
	    slashIndex = c;
	}
    }
    /* now shift the text down... */
    for( c=0, d=slashIndex+1 ;
		(inputText[d] != '\0') && (d < strlen(inputText) )
		 ; c++, d++ )
    {
	inputText[c] = inputText[d];
    }
    inputText[c] = '\0';

    return( inputText );
}


/* game_LoadDriver
 *
 *  loads in the driver defined with [[filename]].  You may need to find
 *   the complete path first using the above method.
 */
GameDriver * 
game_LoadDriver(
	UserParams * userParams, 
	char * filename
)
{
    int c, d, e;
    int maxpal = 1;
    char buf[512];
    char buf2[512];
    GameDriver * gdp = NULL;
    INIStruct *  isp = NULL;
    char * orient = NULL;

    printf( "Driver: %s\n", filename );
    isp = ini_ParseFile( filename );
    if (isp)
    {
	gdp = (GameDriver *) malloc( sizeof( GameDriver ));
	if(!gdp)
	    return( NULL );

	/* setup inital stuff */
	if( userParams->drv )
	{
	    gdp->driverName = strdup( userParams->drv );
	} else if( ini_Query( isp, "General", "Name" ) )
	{
	    gdp->driverName = strdup( ini_Query( isp, "General", "Name" )); 
	} else {

	    /* there was no known game name, so we will yank
	     *	it from the driver filename. 
	     *	(for old drivers specified with -ini)
	     */
	    gdp->driverName = game_ExtractName( strdup( filename ));
	}
	gdp->description = strdup( ini_Query( isp, "General", "Description" ));

	printf( "  Name: %s\n", gdp->driverName );
	if( gdp->description && strlen( gdp->description ) > 0 )
	    printf( "  Desc: %s\n", gdp->description );

	gdp->ndecodes = ini_QueryInt( isp, "Layout", "GfxDecodes");
	gdp->gfxDecodes = (GfxDecode *) malloc( gdp->ndecodes * 
					sizeof( GfxDecode ) );

	/* load in the graphics decode arrays */
	for( c=0 ; c< gdp->ndecodes ; c++)
	{
	    sprintf( buf, "Decode%d", c+1 );
	    gdp->gfxDecodes[c].startaddress = ini_QueryInt( isp, buf, "start" );
	    gdp->gfxDecodes[c].width        = ini_QueryInt( isp, buf, "width" );
	    gdp->gfxDecodes[c].height       = ini_QueryInt( isp, buf, "height" );
	    gdp->gfxDecodes[c].nsprites     = ini_QueryInt( isp, buf, "total" );


	    /* I had a mistake in the driver description file, so I need to
		be sure that all variants of this are valid.  oops. */
	    orient = ini_Query( isp, buf, "orientation" );
	    if ( orient )
	    {
		gdp->gfxDecodes[c].orientation =
				ini_QueryInt( isp, buf, "orientation" );
	    } else {
		/* in case someone is using an incorrect version of the docs */
		orient = ini_Query( isp, "Layout", "orientation" );
		if( orient )
		{
		    gdp->gfxDecodes[c].orientation =
				ini_QueryInt( isp, "Layout", "orientation" );
		} else {
		    gdp->gfxDecodes[c].orientation =
				ini_QueryInt( isp, "Layout", "Orientation" );
		}
	    }

	    gdp->gfxDecodes[c].planes = ini_QueryInt( isp, buf, "planes" );

	    for( d=0 ; d< gdp->gfxDecodes[c].planes ; d++ )
	    {
		gdp->gfxDecodes[c].planeoffsets[d] = 
		    ini_QueryArrayInt( isp, buf, "planeoffsets", d );
	    }

	    for( d=0 ; d< gdp->gfxDecodes[c].width ; d++ )
	    {
		gdp->gfxDecodes[c].xoffset[d] = 
		    ini_QueryArrayInt( isp, buf, "xoffsets", d );
	    }

	    for( d=0 ; d< gdp->gfxDecodes[c].height ; d++ )
	    {
		gdp->gfxDecodes[c].yoffset[d] = 
		    ini_QueryArrayInt( isp, buf, "yoffsets", d );
	    }
	    gdp->gfxDecodes[c].charincrement =
		    ini_QueryInt( isp, buf, "charincrement" );
	}

	/* load in the rom definitions */
	/* first count up the roms... */
	gdp->nroms = 0;
	while( 1 )
	{
	    sprintf( buf, "Rom%d", gdp->nroms+1 );
	    if( !ini_Query( isp, "GraphicsRoms", buf ) )
		break;

	    gdp->nroms++;
	}

	/* now do it again, but this time, wiiff feeewing */
	gdp->romFileDescs = (RomFileDesc * ) malloc( gdp->nroms *
					    sizeof( RomFileDesc ) );
	for( c=0 ; c< gdp->nroms ; c++ )
	{
	    sprintf( buf, "Rom%d", c+1 );

	    gdp->romFileDescs[c].offset = 
		ini_QueryArrayInt( isp, "GraphicsRoms", buf, 0 );

	    gdp->romFileDescs[c].size   =
		ini_QueryArrayInt( isp, "GraphicsRoms", buf, 1 );

	    strcpy( gdp->romFileDescs[c].filename,
		ini_QueryArrayString( isp, "GraphicsRoms", buf, 2 ) );

	    gdp->romFileDescs[c].needed = 0;
	}

	/* load in the palettes */
	/* first, count up the palettes */
	gdp->npalettes = 0;
	while( 1 )
	{
	    sprintf( buf, "Palette%d", gdp->npalettes+1 );
	    if( !ini_Query( isp, "Palette", buf ))
		break;

	    gdp->npalettes++;
	}

	/* again, go through it, reading in the palette data */
	gdp->gamePalettes = (GamePalette *) malloc( gdp->npalettes *
					sizeof( GamePalette ));
	for( c=0 ; c< gdp->npalettes ; c++ )
	{
	    sprintf( buf, "Palette%d", c+1 );

	    gdp->gamePalettes[c].ncolors = 
		ini_QueryArrayInt( isp, "Palette", buf, 0 );

	    for( d=0 ; d< gdp->gamePalettes[c].ncolors ; d++ )
	    {
		gdp->gamePalettes[c].p[d].a = 0;
		gdp->gamePalettes[c].p[d].r =
			ini_QueryArrayInt( isp, "Palette", buf, (1 + d*3) );
		gdp->gamePalettes[c].p[d].g =
			ini_QueryArrayInt( isp, "Palette", buf, (2 + d*3) );
		gdp->gamePalettes[c].p[d].b =
			ini_QueryArrayInt( isp, "Palette", buf, (3 + d*3) );

		maxpal = MAX( gdp->gamePalettes[c].p[d].r, maxpal );
		maxpal = MAX( gdp->gamePalettes[c].p[d].g, maxpal );
		maxpal = MAX( gdp->gamePalettes[c].p[d].b, maxpal );
	    }
	}

	/* now load in the tilemap information */
	gdp->nsets = ini_QueryInt( isp, "Tilemap", "Sets" );
	if( gdp->nsets > 0)
	{
	    gdp->tileSets  = (TileSet *)
				malloc( gdp->nsets * sizeof( TileSet ) );

	    for( c=0 ; c<gdp->nsets ; c++ )
	    {
		sprintf( buf, "Set%d", c+1 );
		strcpy( gdp->tileSets[c].name, 
			ini_QueryArrayString( isp, "Tilemap", buf, 0 ) );

		/* now count up the number of sets and fill the list */
		d = 0;
		while( 1 )
		{
		    if( !ini_QueryArrayString( isp, "Tilemap", buf, d+1 ))
		    {
			break;
		    }
		    gdp->tileSets[c].setnos[d] = 
			    ini_QueryArrayInt( isp, "Tilemap", buf, d+1 );
		    d++;
		}
		/* terminate the list */
		gdp->tileSets[c].setnos[d] = -1; 
	    }
	    gdp->ntilemaps = 0;
	    gdp->tileMaps  = NULL;
	} else {

	    /* no reason to try to load tilemaps, there are no sets. */
	    gdp->ntilemaps = 0;
	    gdp->tileMaps  = NULL;
	    gdp->tileSets  = NULL;
	}

	/* now load the actual tilemap data */
    
	if( gdp->nsets > 0 )
	{
	    gdp->ntilemaps = ini_QueryInt( isp, "Tilemap", "Tilemaps" );

	    /* allocate space for the tilemap array */
	    gdp->tileMaps = (TileMap *)
				malloc( sizeof( TileMap ) * gdp->ntilemaps );

	    /* for each tilemap... fill it */
	    for( c=0 ; c< gdp->ntilemaps ; c++ )
	    {
		sprintf( buf, "Tilemap%d", c+1 );
	    
		gdp->tileMaps[c].fname[0] = '\0';
		gdp->tileMaps[c].name[0] = '\0';
		gdp->tileMaps[c].credit[0] = '\0';
		gdp->tileMaps[c].description[0] = '\0';

		if( ini_Query( isp, buf, "FName" ) )
		    strcpy( gdp->tileMaps[c].fname, 
				ini_Query( isp, buf, "FName" ));

		if( ini_Query( isp, buf, "Name" ) )
		    strcpy( gdp->tileMaps[c].name, 
				ini_Query( isp, buf, "Name" ));

		if( ini_Query( isp, buf, "Credit" ))
		    strcpy( gdp->tileMaps[c].credit, 
				ini_Query( isp, buf, "Credit" ));

		if( ini_Query( isp, buf, "Description" ))
		    strcpy( gdp->tileMaps[c].description, 
				ini_Query( isp, buf, "Description" ));

		gdp->tileMaps[c].bank   = ini_QueryInt( isp, buf, "Bank" ) -1;
		gdp->tileMaps[c].height = ini_QueryInt( isp, buf, "Height" );
		gdp->tileMaps[c].width  =  ini_QueryInt( isp, buf, "Width" );

		gdp->tileMaps[c].row = 
			    (int **)
			    malloc( sizeof( int * ) * gdp->tileMaps[c].height );

		/* now fill the sprite reference arrays (rows) */
		for( d=0 ; d<gdp->tileMaps[c].height ; d++ )
		{
		    /* each row */
		    gdp->tileMaps[c].row[d] = 
		    (int *) malloc( sizeof( int * ) * gdp->tileMaps[c].width );

		    sprintf( buf2, "Row%d", d+1 );
		    for( e=0 ; e<gdp->tileMaps[c].width ; e++ )
		    {
			/* each element in the row */
			gdp->tileMaps[c].row[d][e] =
				ini_QueryArrayInt( isp, buf, buf2, e );
		    }
		}
	    }
	}

	ini_Destroy( isp );
    } else {
	return( NULL );
    }

    /* some games use a palette of [0..63], which we 
	need to scale up to [0..255] for PCX file use */
    if( (maxpal < BRIGHTNESS_HEURISTIC) && (maxpal > 0) )
    {
	int scaleFactor = (int) (255.0 /(float) maxpal);

	/* scale up the palette to be (0..255) */
	for( c=0 ; c< gdp->npalettes ; c++ )
	{
	    for( d=0 ; d< gdp->gamePalettes[c].ncolors ; d++ )
	    {
		gdp->gamePalettes[c].p[d].r *= scaleFactor;
		gdp->gamePalettes[c].p[d].g *= scaleFactor;
		gdp->gamePalettes[c].p[d].b *= scaleFactor;

		/* just in case, we should do this, to cap it off */
		gdp->gamePalettes[c].p[d].r =
				MIN( gdp->gamePalettes[c].p[d].r, 255 );
		gdp->gamePalettes[c].p[d].g =
				MIN( gdp->gamePalettes[c].p[d].g, 255 );
		gdp->gamePalettes[c].p[d].b =
				MIN( gdp->gamePalettes[c].p[d].b, 255 );
	    }
	}
    }

    return( gdp );
}


/* game_DestroyDriver
 *
 *  frees memory taken up by a driver
 */
void game_DestroyDriver( GameDriver * gd )
{
    int c, d;
    if( !gd ) return;

    if( gd->driverName )   free( gd->driverName );
    if( gd->description )  free( gd->description );
    if( gd->tileSets )     free( gd->tileSets );

    /* free the tilemaps too! */
    for( c=0 ; gd->tileMaps && c<gd->ntilemaps ; c++ )
    {
	if( gd->tileMaps[c].row != NULL )
	{
	    for( d=0 ; d<gd->tileMaps[c].height ; d++ )
	    {
		/* free the row */
		free( gd->tileMaps[c].row[d] );
	    }
	    /* and the array of pointers */
	    free( gd->tileMaps[c].row );
	}
    }
    /* finally, free the tilemap struct array */
    if( gd->tileMaps )     free( gd->tileMaps );

    free( gd );
}


/* game_Dump
 *
 * dump out the game driver information (debug use)
 */
void game_Dump( GameDriver * gd )
{
    int c, d, e;
    if( !gd )  return;

    printf( "=======\n" );
    printf( "  Name: %s\n", gd->driverName );
    printf( "  Desc: %s\n", gd->description);
    printf( "  ngfx: %d\n", gd->ndecodes   );
    for( c=0 ; c< gd->ndecodes ; c++)
    {
	printf(" === Decode %d ===\n", c+1 );
	printf("    start: %d\n", gd->gfxDecodes[c].startaddress );
	printf("    width: %d\n", gd->gfxDecodes[c].width );
	printf("   height: %d\n", gd->gfxDecodes[c].height );
	printf(" nsprites: %d\n", gd->gfxDecodes[c].nsprites );
	printf("    c-inc: %d\n", gd->gfxDecodes[c].charincrement );
	printf("   planes: %d\n", gd->gfxDecodes[c].planes );

	printf("   p-offs: ");
	for( d=0 ; d< gd->gfxDecodes[c].planes ; d++ )
	    printf("%3d ", gd->gfxDecodes[c].planeoffsets[d]);
	printf("\n");

	printf("   x-offs: ");
	for( d=0 ; d< gd->gfxDecodes[c].width ; d++ )
	    printf("%3d ", gd->gfxDecodes[c].xoffset[d]);
	printf("\n");

	printf("   y-offs: ");
	for( d=0 ; d< gd->gfxDecodes[c].height ; d++ )
	    printf("%3d ", gd->gfxDecodes[c].yoffset[d]);
	printf("\n");

    }
    printf(" ===\n");

    printf(" nroms: %d\n", gd->nroms  );

    for( c=0 ; c< gd->nroms ; c++ )
    {
	printf(" === Rom %d ===\n", c+1 );
	printf("   offset: 0x%x\n", gd->romFileDescs[c].offset );
	printf("     size: 0x%x\n", gd->romFileDescs[c].size );
	printf(" filename: %s\n", gd->romFileDescs[c].filename );
    }
    printf(" ===\n");

    printf(" npals: %d\n", gd->npalettes  );
    for( c=0 ; c< gd->npalettes ; c++ )
    {
	printf(" === Palette %d ===\n", c+1);
	printf("  ncolors: %d\n", gd->gamePalettes[c].ncolors );
	printf("  entries:");
	for( d=0 ; d< gd->gamePalettes[c].ncolors ; d++ )
	{
	    printf(" %02x %02x %02x ",
			gd->gamePalettes[c].p[d].r,
			gd->gamePalettes[c].p[d].g,
			gd->gamePalettes[c].p[d].b );
	}
	printf("\n");
    }

    printf(" === Tilemap Sets\n");
    printf(" nsets: %d\n", gd->nsets );
    for( c=0 ; c < gd->nsets ; c++ )
    {
	printf("    Set%2d: %s\n", c+1, gd->tileSets[c].name );
	printf("     maps:");
	for( d=0 ; gd->tileSets[c].setnos[d] != -1 ; d++ )
	    printf(" %d", gd->tileSets[c].setnos[d] );
	printf("\n");
    }

    printf(" === Tilemaps\n");
    printf(" ntilemaps: %d\n", gd->ntilemaps );
    for( c=0 ; c < gd->ntilemaps ; c++ )
    {
	printf(" ==== Tilemap %d\n", c+1 );
	printf("   fname: %s.XXX\n", gd->tileMaps[c].fname );
	printf("    name: %s\n", gd->tileMaps[c].name );
	printf("  credit: %s\n", gd->tileMaps[c].credit );
	printf("    desc: %s\n", gd->tileMaps[c].description );
	printf("    bank: %d\n", gd->tileMaps[c].bank+1 );
	printf("   width: %d\n", gd->tileMaps[c].width );
	printf("  height: %d\n", gd->tileMaps[c].height );
	
	for( d=0 ; d<gd->tileMaps[c].height ; d++ )
	{
	    /* print out each row */
	    printf("       -:" );

	    for( e=0 ; e<gd->tileMaps[c].width ; e++ )
	    {
		/* print out each column */
		printf( " %2d", gd->tileMaps[c].row[d][e] );
	    }
	    printf("\n" );
	}
    }
}

