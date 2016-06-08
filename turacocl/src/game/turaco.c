/*
 * turaco.h
 *
 * $Id: turaco.c,v 1.60 2003/05/31 03:16:03 jerry Exp $
 *
 * $Log: turaco.c,v $
 * Revision 1.60  2003/05/31 03:16:03  jerry
 * Fixed Bug 15. (Photoshop 5 color images)
 *
 * Revision 1.59  2003/05/30 22:21:20  jerry
 * Forgot about 1<<n for "2 to the nth power"... duh.
 *
 * Revision 1.58  2003/05/30 21:53:39  jerry
 * Updated for bug 15
 *
 * Revision 1.57  2003/05/29 21:21:33  jerry
 * turaco_OverlayTilemapImages() added to do the tilemap import hack
 *
 * Revision 1.56  2003/05/29 04:53:05  jerry
 * Beginnings of the tilemap load routine
 *
 * Revision 1.55  2003/05/29 04:16:51  jerry
 * Tilemap generation working!  YAY!
 *
 * Revision 1.54  2003/05/29 02:50:00  jerry
 * added -dmp option to dump out driver info.
 * (should make debugging easier)
 *
 * Revision 1.53  2003/05/20 03:41:18  jerry
 * First jab at tilemap generation
 *
 * Revision 1.52  2003/05/19 21:47:12  jerry
 * Moved the ROM IO routines out of turaco.[ch] into romio.[ch]
 *
 * Revision 1.51  2003/05/19 20:44:58  jerry
 * Moved the checkerboard generation out of "turaco.c" and into "checker.c"
 * (some changes are going to happen to it for tilemap stuff.  It needs to be more modular.)
 *
 * Revision 1.50  2003/05/15 19:50:33  jerry
 * Intermediary files.  Got stuck when making the tilemap saver
 *
 * Revision 1.49  2003/05/15 02:43:26  jerry
 * Added in the herringbone generation, even though it's not
 * really a herringbone... more of a small checkerboard.
 *
 * Revision 1.48  2003/05/15 01:28:39  jerry
 * Fixed the driver to be correct
 * Implemented the tilemap_SetContains() function
 *
 * Revision 1.47  2003/05/14 20:33:45  jerry
 * Moved VerifyDriver() into turaco.c from params.c
 * Implemented the tileset checker in VerifyDriver()
 * If tile set wasn't found, it lists them out.
 *
 * Revision 1.46  2003/05/14 15:18:51  jerry
 * C++isms are not welcome in a .c file.  oops.
 *
 * Revision 1.45  2003/05/14 01:23:54  jerry
 * Moved the coding rom routines into turaco.[ch]
 *
 * Revision 1.44  2003/05/13 04:36:43  jerry
 * Added better text drawing for checkerboard files.
 * (if >8 wide, draw four digits instead of 2)
 *
 * Revision 1.43  2003/05/03 06:10:09  jerry
 * Updated the checkerboard
 * finished changing the en/de code routines
 * Starting to add the tilemap driver
 *
 * Revision 1.42  2003/04/27 23:59:09  jerry
 * Fixed Bug 03
 * Started to implement the Tilemap stuff
 *
 * Revision 1.41  2003/04/17 15:59:21  jerry
 * Changed the ordering for the subdots
 *
 * Revision 1.40  2003/04/17 15:44:26  jerry
 * Fixed the '-rod' bug.  It now outputs files where you want them
 * It also displays which roms' filenames are being generated.
 *
 * Revision 1.39  2003/04/17 15:20:43  jerry
 * Updated the checkerboarding to produce smaller font, and dotted font
 * so that each sprite has its own unique look (less than 1,100 sprites anyway)
 *
 * Revision 1.38  2003/04/12 19:36:56  jerry
 * "ROMs Used" -> "ROMs"
 * changed checkerboard to use decimal instead of hex for consistancy
 *
 * Revision 1.37  2003/04/12 19:32:05  jerry
 * "ROMs used" -> "ROMs"
 *
 * Revision 1.36  2003/04/11 21:26:49  jerry
 * Oops. we do need misc.h for the slash thing.
 *
 * Revision 1.35  2003/04/11 21:25:23  jerry
 * Eliminated the need for misc.h
 *
 * Revision 1.34  2003/04/11 21:01:41  jerry
 * Moved stuff out into misc.h (MIN, MAX, os-dependant #defines, etc)
 * FINALLY fixed the stupid MS-DOS bug.  fopen( filename, "rb" )
 *
 * Revision 1.33  2003/04/11 02:31:11  jerry
 * Switched to use the first three letters of a game name for the
 * saved image filenames
 *
 * Revision 1.32  2003/04/09 21:07:11  jerry
 * fixed a typo in a comment. oops.
 *
 * Revision 1.31  2003/04/09 02:09:05  jerry
 * Added some switches for the MS-DOS path selection \ instead of /
 * etc.
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
#include "checker.h"

#define BUFLEN (1024)
#define FILEBUFSIZE (4096)
#define MAXPATH (1024)

/* turaco_Create
 * 
 *  set up an initial instance
 */
TuracoInstance *
turaco_Create( void )
{
    TuracoInstance * ti = (TuracoInstance *) malloc( sizeof( TuracoInstance ));

    if (ti)
    {
	ti->up = NULL;
	ti->gd = NULL;

	ti->paletteNumber = 0;
	ti->bankNumber    = 0;

	ti->loadedGraphics = NULL;
	ti->colorKey       = NULL;

	ti->romstart  = 0;
	ti->romBufferSize   = 0;
	ti->romBuffer = NULL;

	ti->fn_bank[0]  = '\0';
	ti->fn_key[0]   = '\0';
	ti->fn_check[0] = '\0';
    }

    return( ti );
}


/* turaco_Destroy
 * 
 *  get rid of an instance by freeing all of the alloc'd items.
 */
void
turaco_Destroy( TuracoInstance * ti )
{
    if (!ti)  return;

    if( ti->up )
	free( ti-> up );

    if( ti->gd )
	game_DestroyDriver( ti->gd );

    if( ti->loadedGraphics )
	Image_Destroy( ti->loadedGraphics );

    if( ti->colorKey )
	Image_Destroy( ti->colorKey );

    if( ti->romBuffer )
	free( ti->romBuffer );

    free( ti );
}


/* turaco_ConfigureFilenames
 *
 *  sets up all of the internal filename strings apropriately
 */
int
turaco_ConfigureFilenames( TuracoInstance * ti )
{
    int c = 0;
    char driver3[8];

    /* initial checks */
    if (!ti)     return( ERR_NO_TURACO_STRUCT );
    if (!ti->up) return( ERR_NO_PARAMS_STRUCT );
    if (!ti->gd) return( ERR_NO_DRIVER_STRUCT );

    /* copy over the first three characters over from the driverName */
    if( ti->gd->driverName )
    {
	for( c=0 ; (c<3) && (ti->gd->driverName[c] !='\0') ; c++ )
	{
	    driver3[c] = ti->gd->driverName[c];
	}
    }
    driver3[c] = '\0';

    /* Rom Data     (-dbf or generate) */

    if( ti->up->dbf )
    {
	strncpy( ti->fn_bank, ti->up->dbf, FN_MAX );
    } else {
#ifdef MSDOS
	sprintf( ti->fn_bank,
		"%s_%d.%s", driver3, ti->up->bnk+1, ti->up->ffs );
#else
	snprintf( ti->fn_bank, FN_MAX,
		"%s_%d.%s", driver3, ti->up->bnk+1, ti->up->ffs );
#endif
    }


    /* Color Key    (-key or genrerate) */

    if( ti->up->key )
    {
	strncpy( ti->fn_key, ti->up->key, FN_MAX );
    } else {
#ifdef MSDOS
	sprintf( ti->fn_key,
		"%s_p%d.%s", driver3, ti->up->pal+1, ti->up->ffs );
#else
	snprintf( ti->fn_key, FN_MAX,
		"%s_p%d.%s", driver3, ti->up->pal+1, ti->up->ffs );
#endif
    }


    /* Checkerboard (-chk or generate) */

    if( ti->up->chk )
    {
	strncpy( ti->fn_check, ti->up->chk, FN_MAX );
    } else {
#ifdef MSDOS
	sprintf( ti->fn_check,
		"%s_%dc.%s", driver3, ti->up->bnk+1, ti->up->ffs );
#else
	snprintf( ti->fn_check, FN_MAX,
		"%s_%dc.%s", driver3, ti->up->bnk+1, ti->up->ffs );
#endif
    }

    return( ERR_NONE );
}




/* turaco_LoadDriver
 * 
 *  load the driver selected by the userParams
 *  returns [[0]] on success
 */
int
turaco_LoadDriver( TuracoInstance * ti )
{
    char buf[BUFLEN];
    char * driverPathName;

    if( !ti )  return( ERR_NO_TURACO_STRUCT );

    /* try to find the proper driver... */
    if( !ti->up->ini )
    {
	driverPathName = game_FindDriver( ti->up, buf, BUFLEN );
	if( !driverPathName )
	{
	    fprintf( stderr, "%s: Unable to find driver.\n", ti->up->drv );
	    return( ERR_FIND_DRIVER_FAILED );
	}

	/* try to load the driver... */
	ti->gd = game_LoadDriver( ti->up, driverPathName );

	if( !ti->gd )
	{
	    fprintf( stderr, "%s: Unable to load driver.\n", ti->up->drv );
	    return( ERR_LOAD_DRIVER_FAILED );
	}

    } else {
	ti->gd = game_LoadDriver( ti->up, ti->up->ini );
	if( !ti->gd )
	{
	    fprintf( stderr, "%s: Unable to load driver.\n", ti->up->ini );
	    return( ERR_LOAD_DRIVER_FAILED );
	}
    }

    return( ERR_NONE );
}



/* turaco_VerifyDriver
 *
 *  checks the validity of driver-based parameters (range checking, etc)
 */
int
turaco_VerifyDriver( TuracoInstance *ti )
{
    if( !ti )      return( ERR_NO_PARAMS_STRUCT );
    if( !ti->up )  return( ERR_NO_PARAMS_STRUCT );

    if( (ti->up->bnk < 1) || (ti->up->bnk > ti->gd->ndecodes) )
    {
	fprintf( stderr, "ERROR: Bank out of range!\n");
	return( ERR_BANK_OUT_OF_RANGE );
    }
    ti->up->bnk--;

    if( (ti->up->pal < 1) || (ti->up->pal > ti->gd->ndecodes) )
    {
	fprintf( stderr, "ERROR: Palette out of range!\n");
	return( ERR_PAL_OUT_OF_RANGE );
    }
    ti->up->pal--;

    if( ti->up->rom )
    {
	/* make sure directory exists */
	if( !params_IsDir( ti->up->rom ))
	{
	    fprintf( stderr, "ERROR: \"%s\" is a bad dir.\n", ti->up->rom);
	    return( ERR_SRC_DIR_BAD );
	}
 
    }

    if( ti->up->rod )
    {
	/* zot the trailing slash */
	if( strlen(ti->up->rod) > 0)
	{
	    if( ti->up->rod[ strlen(ti->up->rod)-1 ] == '/' ||
		ti->up->rod[ strlen(ti->up->rod)-1 ] == '\\' )
		ti->up->rod[ strlen(ti->up->rod)-1 ] = '\0';
	}
	/* make sure the directory exists */
	if( !params_IsDir( ti->up->rod ))
	{
	    fprintf( stderr, "ERROR: \"%s\" is a bad dir.\n", ti->up->rod);
	    return( ERR_DST_DIR_BAD );
	}

    } else {
	ti->up->rod = ".";
    }

    if( !strcmp( ti->up->inf, "IMG" ))
    {
	/* make sure color key image exists */
	/* make sure databank image exists */
    }

    /* check the tilemap set selected */
    if( ti->up->tms )
    {
	int wtms, tm;

	ti->up->tmsn = -1;

	/* iterate over the Tilemap/SetN[0] for the right one */
	for( wtms=0 ; wtms < ti->gd->nsets ; wtms++ )
	{
	    if( !strcmp( ti->up->tms, ti->gd->tileSets[wtms].name ))
	    {
		ti->up->tmsn = wtms;
	    }
	}

	/* if not found, print and return ERROR */
	if( ti->up->tmsn < 0 )
	{
	    fprintf( stderr, 
			"ERROR: \"%s\" tileset not found.\n", ti->up->tms );

	    if( ti->gd->nsets )
	    {
		fprintf( stderr, "  Defined tilesets:\n");
		for( wtms=0 ; wtms < ti->gd->nsets ; wtms++ )
		{
		    fprintf( stderr, "\t%s\n", ti->gd->tileSets[wtms].name );
		}

	    } else {
		fprintf( stderr, "  No tilesets defined in driver.\n");
	    }

	    return( ERR_SET_NOT_FOUND );
	}

	/* make sure the tilemaps are named too. */
	for( tm=0 ; tm < ti->gd->ntilemaps ; tm++ )
	{
	    if( !strlen( ti->gd->tileMaps[tm].fname ))
	    {
		fprintf( stderr, 
			"ERROR: Missing \"fname\" in tilemap %d\n", tm+1 );
		return( ERR_NAME_NOT_DEFINED );
	    }
	}

    } else {
	ti->up->tmsn = -2;
    }

    return( ERR_NONE );
}



/* turaco_LoadImages
 *
 *  loads the Image data as selected in the user params
 */
int
turaco_LoadImages( TuracoInstance * ti )
{
    /* load in the graphics image png */
    if( ti->loadedGraphics)  Image_Destroy( ti->loadedGraphics );
    if( ti->colorKey )       Image_Destroy( ti->colorKey );

    ti->colorKey = ti->loadedGraphics = NULL;

    /* load in the key image png */
    if( ti->up->ff != ff_PCX)
    {
	ti->colorKey = LS_Load( ti->fn_key, ti->up->ff );
	if( !ti->colorKey )
	    return( ERR_KEY_MISSING );
    }

    /* load in the primary image png */
    ti->loadedGraphics = LS_Load( ti->fn_bank, ti->up->ff );
    if( !ti->loadedGraphics )
	return( ERR_IMAGE_MISSING );

    printf( "\n== Loading images ==\n" );
    printf( " Bank Graphics: %s\n", ti->fn_bank );
    if( ti->up->ff != ff_PCX)
    {
	printf( "     Color Key: %s\n", ti->fn_key );
    }


    /* normalize the image back to indexed mode */
    return( ERR_NONE );
}


/* turaco_OverlayTilemapImages
 *
 *  loads the proper tilemaps and decodes them into the ROM buffer
 */
void turaco_OverlayTilemapImages( TuracoInstance * ti )
{
    int tm;

    /* load in the tilemaps */
    if( ti->up->tms && (ti->up->tmsn >= 0) )
    {
	printf( "\n== Loading tilemap set \"%s\" ==\n", ti->up->tms );

	for( tm = 0 ;
	     ti->gd->tileSets[ ti->up->tmsn ].setnos[tm] >= 0 ;
	     tm++)
	{
	    (void) tilemap_Load( ti, 
		ti->gd->tileSets[ ti->up->tmsn ].setnos[tm]-1 );
	}
    }
}




/* turaco_SaveImages
 *  
 *  saves out the internal images 
 */
int
turaco_SaveImages( TuracoInstance * ti )
{
    int caps;
    int tm;
    IMAGE * timg;

    if( !ti )       return( ERR_NO_INSTANCE );
    if( !ti->up )   return( ERR_NO_PARAMS_STRUCT );
    if( !ti->gd )   return( ERR_NO_ROMFILE_DESCS );
    if( !ti->loadedGraphics )  return( ERR_NO_IMAGE );
    if( !ti->colorKey )        return( ERR_NO_IMAGE );

    caps = LS_Query( ti->up->ff );
    
    /* save the core graphics */
    LS_Save( ti->fn_bank, ti->loadedGraphics, ti->colorKey, ti->up->ff );

    if( caps & QUERY_TRUECOLOR ) /* no need to save it if we're paletted */
    {
	/* save the palette key */
	LS_Save( ti->fn_key, ti->colorKey, NULL, ti->up->ff );
    }

    /* generate and save the checkerboard */
    timg = Image_Duplicate( ti->colorKey );

    if( timg )
    {
	timg->data[0].r = timg->data[0].g = timg->data[0].b = 0;

	timg->data[1].r = timg->data[1].g = timg->data[1].b = 255;

	timg->data[2].r = timg->data[2].g = timg->data[2].b = 0;
	timg->data[2].g = 255;

	timg->data[3].r = timg->data[3].g = timg->data[3].b = 0;
	timg->data[3].r = 255;
    }

    checker_Generate( ti );
    LS_Save( ti->fn_check, ti->loadedGraphics, timg, ti->up->ff );

    printf( "\n== Saving images ==\n" );
    printf( " Bank Graphics: %s\n", ti->fn_bank );
    printf( "  Checkerboard: %s\n", ti->fn_check );
    if( ti->up->ff != ff_PCX)
    {
	printf( "     Color Key: %s\n", ti->fn_key );
    }

    /* save out the tilemaps */
    if( ti->up->tms && (ti->up->tmsn >= 0) )
    {
	printf( "                (\"%s\" tilemap set)\n", ti->up->tms );
	for( tm = 0 ;
	     ti->gd->tileSets[ ti->up->tmsn ].setnos[tm] >= 0 ;
	     tm++)
	{
	    (void) tilemap_Save( ti, 
		ti->gd->tileSets[ ti->up->tmsn ].setnos[tm]-1 );
	}
    }

    Image_Destroy( timg );

    return( ERR_NONE );
}



/* turaco_GenerateColorKeyFromDriver
 *
 *  generates the color key image from the driver parameters
 */
int
turaco_GenerateColorKeyFromDriver( TuracoInstance * ti )
{
    int x;

    /* zot any existing image */
    if( ti->colorKey )  Image_Destroy( ti->colorKey );

    /* create a new palette image */
    ti->colorKey = Image_Create( 
			1, ti->gd->gamePalettes[ti->up->bnk].ncolors, 8 );

    for( x=0 ; x< ti->gd->gamePalettes[ti->up->bnk].ncolors ; x++)
    {
	ti->colorKey->data[x].r = ti->gd->gamePalettes[ti->up->pal].p[x].r;
	ti->colorKey->data[x].g = ti->gd->gamePalettes[ti->up->pal].p[x].g;
	ti->colorKey->data[x].b = ti->gd->gamePalettes[ti->up->pal].p[x].b;
    }
    

    return( ERR_NONE );
}


/* turaco_PalettedToRGB
 *
 *  convert the alpha channel with the palette selected to RGB
 */
void
turaco_PalettedToRGB( TuracoInstance * ti )
{
    int x;

    for( x=0 ; 
	 x< (ti->loadedGraphics->width * ti->loadedGraphics->height) ;
	 x++ )
    {
	ti->loadedGraphics->data[x].r = 
	  ti->gd->gamePalettes[ti->up->pal].p[ti->loadedGraphics->data[x].a].r;
	ti->loadedGraphics->data[x].g = 
	  ti->gd->gamePalettes[ti->up->pal].p[ti->loadedGraphics->data[x].a].g;
	ti->loadedGraphics->data[x].b = 
	  ti->gd->gamePalettes[ti->up->pal].p[ti->loadedGraphics->data[x].a].b;
	/* phew! */
    }
}



/* _turaco_PAL_Lookup
 *
 *  internal method to do the palette lookup for us
 */
int
_turaco_PAL_Lookup( long * palette, int ncolors, long muxcolor )
{
    int x;
    for( x=0 ; x<ncolors && x<256 ; x++ )
    {
	if ( palette[x] == muxcolor )
	    return( x );
    }

    return( 0 );
}


/* turaco_RGBToPaletted
 *
 *  convert the RGB image to a paletted image into the alpha channel
 *  using the loaded color key
 */
void
turaco_RGBToPaletted( TuracoInstance * ti )
{
    int i, ncolors;
    long mypalette[256]; /* stored as 0xuuRRGGBB */
    long muxcolor;

    /* first, count up the number of colors in the palette */
    ncolors = ti->colorKey->width * ti->colorKey->height;

    /* wipe the local palette */
    memset( mypalette, 0, 256 * sizeof( long ) );

    /* first, generate the local palette */
    for ( i=0 ; i<256 && i<ncolors ; i++ )
    {
	mypalette[i] =  ((ti->colorKey->data[i].b << 16) & 0x00ff0000) +
			((ti->colorKey->data[i].g <<  8) & 0x0000ff00) +
			((ti->colorKey->data[i].r      ) & 0x000000ff);
    }

    /* now we need to iterate over each pixel in the image */
    for ( i=0 ; 
	  i< (ti->loadedGraphics->width * ti->loadedGraphics->height) ; 
	  i++ )
    {
	/* make the muxedcolor */
	muxcolor = ((ti->loadedGraphics->data[i].b << 16) & 0x00ff0000) +
                   ((ti->loadedGraphics->data[i].g <<  8) & 0x0000ff00) +
                   ((ti->loadedGraphics->data[i].r      ) & 0x000000ff);

	/* look it up and shove the value back into the alpha channel. */

	ti->loadedGraphics->data[i].a = 
		_turaco_PAL_Lookup( mypalette, ncolors, muxcolor );
    }
}


/* turaco_DecodeRom
 *         
 * takes in a [gfxDecode], and fabricates an [image] from the [romBuffer]
 * 
 * Range checking is assumed to have happened before this, therefore
 * there is no return value.
 *          
 * NOTE: This looks for the image data in the alpha channel of the IMAGE.
 *       It needs to be converted externally to load it properly
 */
void
turaco_DecodeRom(  
	TuracoInstance * ti,
        GfxDecode * gd,
        IMAGE * image,
        char * romBuffer,
        long bufsize )
{
    int c, xpos,ypos;

    /* make sure the parameters aren't bogus. */
    if( !ti || !gd || !image || !romBuffer )  return;

    for( c=0, xpos=0, ypos=0 ; c < gd->nsprites ; c++ )
    {
        /* adjust where we are in the image based on sprite number */
        if( (gd->width * (xpos+1) ) > image->width )
        {
            ypos++;
            xpos = 0;
        }

	if( tilemap_SetContains( ti, c ) )
	{
	    /* herringbone */;
	    coding_GenHerringbone( gd, image, xpos, ypos );
	} else {
	    coding_DecodeSprite( gd, c, image, xpos, ypos, romBuffer, bufsize );
	}

        xpos++;
    }

    /* now decode the tilemaps */
}



/* turaco_DecodeImageFromRomBuffer
 *
 *  generates the internal graphics and key IMAGEs by decoding the RomBuffer
 */
int
turaco_DecodeImageFromRomBuffer( TuracoInstance * ti )
{
    int r;
    int wide, tall;
    if( ti->loadedGraphics)  Image_Destroy( ti->loadedGraphics );

    /* determine the image size */
    if(0)
    {
	wide = 256 / ti->gd->gfxDecodes[ti->up->bnk].width;
    }
    wide = ti->up->wid;
    tall = ti->gd->gfxDecodes[ti->up->bnk].nsprites / wide;

    /* JJJ Fix? for non-divisor widths. */
    if( ((float)ti->gd->gfxDecodes[ti->up->bnk].nsprites / (float) wide ) 
		> tall)
    {
	tall++;
    }

    /* create the new image structure */
    ti->loadedGraphics = Image_Create( 
		wide * ti->gd->gfxDecodes[ti->up->bnk].width,
		tall * ti->gd->gfxDecodes[ti->up->bnk].height,
		8 );

    /* decode the rom into the image structure */
    turaco_DecodeRom( ti, &ti->gd->gfxDecodes[ti->up->bnk],
			ti->loadedGraphics,
			ti->romBuffer, ti->romBufferSize );

    /* convert the palette into an RGB likeness */
    turaco_PalettedToRGB( ti ); 

    /* finally, generate the color key associated with the above */
    r = turaco_GenerateColorKeyFromDriver( ti );

    return( ERR_NONE );
}


/* turaco_EncodeRom   
 *         
 * takes in a [gfxDecode], and Converts the [image] into the [romBuffer]
 *
 * Range checking is assumed to have happened before this, therefore
 * there is no return value.
 *
 * NOTE: This loads the image data into the alpha channel of the IMAGE.  
 *       It needs to be converted externally to save it properly
 */
void        
turaco_EncodeRom(   
	TuracoInstance * ti,
        GfxDecode * gd,
        IMAGE * image,
        char * romBuffer,
        long bufsize )
{
    int c;
    int xoffs, yoffs;   

    for( c=0,xoffs=0,yoffs=0 ; c< gd->nsprites ; c++ )
    {
        /* first, adjust where we are in the image based on the sprite number */
        if( (gd->width * (xoffs+1) ) > image->width )
        {
            yoffs++;
            xoffs=0;
        }

	if( tilemap_SetContains( ti, c ) )
	{
	    /* herringbone */;
	} else {
	    coding_EncodeSprite( gd, c, image, xoffs, yoffs, romBuffer, bufsize );
	}

        xoffs++;
    }
}


/* turaco_EncodeRomBufferFromImage
 *
 *  generates the internal RomBuffer by encoding the internal IMAGE 
 */
int
turaco_EncodeRomBufferFromImage( TuracoInstance * ti )
{
    /* first convert the RGB image to a paletted image using the key */
    if( ti->up->ff != ff_PCX )
    {
	turaco_RGBToPaletted( ti );
    }

	    
    /* next reduce the nuber of colors in the paletted portion of the image */
    /* BUG 15 fix */ 
    if( ti->up->ff != ff_PCX )
    {
	Color_Reduce24( ti->loadedGraphics, ti->colorKey,
		  1 << ( ti->gd->gfxDecodes[ti->up->bnk].planes));
    } else {
	Color_ReduceIntegrated( ti->loadedGraphics,
		  1 << ( ti->gd->gfxDecodes[ti->up->bnk].planes));
    }


    /* next, encode the image using the graphics layout */
    turaco_EncodeRom( ti, &ti->gd->gfxDecodes[ti->up->bnk],
			ti->loadedGraphics,
			ti->romBuffer, ti->romBufferSize );

    return( ERR_NONE );
}

