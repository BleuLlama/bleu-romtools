/*
 * tilemap.c
 *
 * $Id: tilemap.c,v 1.11 2003/05/31 03:40:20 jerry Exp $
 *
 * $Log: tilemap.c,v $
 * Revision 1.11  2003/05/31 03:40:20  jerry
 * Fixed build for MS-DOS (snprintf #ifdef)
 *
 * Revision 1.10  2003/05/29 21:22:32  jerry
 * Tilemap image loader/decoder added.
 * Flaws in system noticed. (it should overlay over the images, not decode.)
 * Tilemap import working now.
 *
 * Revision 1.9  2003/05/29 04:52:40  jerry
 * Started working on the tilemap Load routine
 *
 * Revision 1.8  2003/05/29 04:16:51  jerry
 * Tilemap generation working!  YAY!
 *
 * Revision 1.7  2003/05/20 03:41:18  jerry
 * First jab at tilemap generation
 *
 * Revision 1.6  2003/05/15 19:50:33  jerry
 * Intermediary files.  Got stuck when making the tilemap saver
 *
 * Revision 1.5  2003/05/15 01:28:39  jerry
 * Fixed the driver to be correct
 * Implemented the tilemap_SetContains() function
 *
 * Revision 1.4  2003/05/14 01:23:54  jerry
 * Moved the coding rom routines into turaco.[ch]
 *
 * Revision 1.3  2003/05/13 04:35:29  jerry
 * Added better text drawing for checkerboard files.
 * (if >8 wide, draw four digits instead of 2)
 *
 * Revision 1.2  2003/05/05 15:52:56  jerry
 * builds now.
 *
 * Revision 1.1  2003/05/05 15:50:19  jerry
 * Added tilemap.[ch]
 * Added the remaining bit of driver and dumper for the tilemaps
 *
 */

#include <stdio.h>
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strlen */
#include "turaco.h"
#include "params.h"
#include "coding.h"
#include "gfxlib.h"
#include "misc.h"
#include "errors.h"


#define BUFSIZE (1024)

/* tilemap_IncorrectBank
 *
 *  returns 0 if the bank is correct
 *         ERR_ if things were bad
 */
int
tilemap_IncorrectBank( TuracoInstance * ti, int which )
{
    TileMap * tm;
    if( !ti )      return( ERR_NO_TURACO_STRUCT );
    if( !ti->gd )  return( ERR_NO_DRIVER_STRUCT );
    if( !ti->up )  return( ERR_NO_PARAMS_STRUCT );

    if( which > ti->gd->ntilemaps )  return( ERR_TM_OUT_OF_RANGE );

    tm = &ti->gd->tileMaps[which];

    if( (tm->bank) != ti->up->bnk )    return( ERR_TM_BANK_INCONSISTANT );

    return( ERR_NONE );
}


 
/* tilemap_Save
 * 
 * save out a specific tilemap
 *  returns 0 on success.
 */
int
tilemap_Save( TuracoInstance * ti, int which )
{
    char buf[ BUFSIZE ];
    int ret;
    int r,c;
    TileMap * tm;
    IMAGE * tmi;

    /* make sure the set is in the bank selected */
    ret = tilemap_IncorrectBank( ti, which );
    if( ret != ERR_NONE ) return( ret );


    /* generate an IMAGE, based on tilemap's w & h and sprite's w & h */
    tm = &ti->gd->tileMaps[which];

    tmi = Image_Create( tm->width * ti->gd->gfxDecodes[ tm->bank ].width, 
    			tm->height * ti->gd->gfxDecodes[ tm->bank ].height, 
			ti->loadedGraphics->bits );
    if( !tmi ) return( ERR_NO_IMAGE );

    /* iterate over the bank driver */
	/* fill in the image based on the driver */
    for( r=0 ; r<tm->height ; r++)
    {
	for( c=0 ; c<tm->width ; c++)
	{
	    coding_DecodeSprite( &ti->gd->gfxDecodes[ti->up->bnk],
			         tm->row[r][c], tmi, c,r, 
				 ti->romBuffer, ti->romBufferSize );
	}
    }

    /* generate the proper filename */
#ifdef MSDOS
    sprintf( buf, "%s.%s", tm->fname, ti->up->ffs );
#else
    snprintf( buf, BUFSIZE, "%s.%s", tm->fname, ti->up->ffs );
#endif
    printf( "       Tilemap: %s\n", buf );

    /* save out the IMAGE to the proper filename */
    LS_Save( buf, tmi, ti->colorKey, ti->up->ff );

    Image_Destroy( tmi );

    /* return success (0) */
    return( 0 );
}


/* tilemap_Load
 *
 *  load in a specific tilemap
 *  returns 0 on success.
 */
int
tilemap_Load( TuracoInstance * ti, int which )
{
    char buf[ BUFSIZE ];
    int ret;
    int r,c;
    int error = 0;
    TileMap * tm = NULL;
    IMAGE * tmi = NULL;

    /* make sure the set is in the bank selected */
    ret = tilemap_IncorrectBank( ti, which );
    if( ret != ERR_NONE ) return( ret );

    /* generate the proper filename */
    tm = &ti->gd->tileMaps[which];
#ifdef MSDOS
    sprintf( buf, "%s.%s", tm->fname, ti->up->ffs );
#else
    snprintf( buf, BUFSIZE, "%s.%s", tm->fname, ti->up->ffs );
#endif
    printf( "       Tilemap: %s\n", buf );

    /* load in the IMAGE, fail if not found */
    tmi = LS_Load( buf, ti->up->ff );

    if( !tmi )
    {
	/*fprintf( stderr, "%s: Unable to load image!\n", buf ); */
	return( -1 );
    }

    /* if it's the wrong size, fail */
    /* check width */
    if( tmi->width != (tm->width * ti->gd->gfxDecodes[ tm->bank ].width))
    {
	fprintf( stderr, "%s: Image is too wide.  Was %d, expecting %d\n",
		buf, 
		tmi->width, 
		tm->width * ti->gd->gfxDecodes[ tm->bank ].width );
	error++;
    }
    /* check height */
    if( tmi->height != (tm->height * ti->gd->gfxDecodes[ tm->bank ].height))
    {
	fprintf( stderr, "%s: Image is too wide.  Was %d, expecting %d\n",
		buf, 
		tmi->width, 
		tm->width * ti->gd->gfxDecodes[ tm->bank ].width );
	error++;
    }


    if( error )
    {
	return( -1 );
    }

    /* iterate over the tilemap */
    for( r=0 ; r<tm->height ; r++)
    {   
        for( c=0 ; c<tm->width ; c++)
        {
            coding_EncodeSprite( &ti->gd->gfxDecodes[ti->up->bnk],
                                 tm->row[r][c], tmi, c,r,
                                 ti->romBuffer, ti->romBufferSize );
        }
    }

    if( tmi )
	Image_Destroy( tmi );

    return( 0 );
}


/* tilemap_SetContains
 * 
 *   determines whether a sprite number is in the selected set
 *   returns: 1 if it is used in a set
 *            0 if it is not used
 *            0 on failure
 *   **ASSUMES** the bank has been checked already
 */
int
tilemap_SetContains( TuracoInstance * ti, int spriteNumber )
{
    int tm, mn;
    int r,c;

    /* if there is no defined set, return 0 */
    if( !ti->up->tms || ti->up->tmsn < 0 )
	return( 0 );

    /*printf("Tilemap Contains %d?\n", spriteNumber ); */

    /* make sure the set is in the bank selected */

    /* for each tilemap in the set */
    for( tm = 0 ; 
	 ti->gd->tileSets[ ti->up->tmsn ].setnos[tm] >= 0 ;
	 tm++)
    {
	mn = ti->gd->tileSets[ ti->up->tmsn ].setnos[tm] -1;

	/* for each row */
	for( r=0 ; r<ti->gd->tileMaps[mn].height ; r++ )
	{
	    /* for each column */
	    for( c=0 ; c<ti->gd->tileMaps[mn].width ; c++ )
	    {
		/* if found, return 1 */
		if( spriteNumber == ti->gd->tileMaps[mn].row[r][c] )
		    return( 1 );
	    }
	}
    }
    return( 0 );
}

