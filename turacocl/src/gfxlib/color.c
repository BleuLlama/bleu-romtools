/*
 *  color.c
 *
 *   color manipulations functions
 *
 * $Id: color.c,v 1.2 2003/05/31 03:16:03 jerry Exp $
 *
 * $Log: color.c,v $
 * Revision 1.2  2003/05/31 03:16:03  jerry
 * Fixed Bug 15. (Photoshop 5 color images)
 *
 * Revision 1.1  2003/05/30 21:54:54  jerry
 * Adding the color reducer for BUG 15 (gfxlib/color.[ch])
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"

int
Color_Find24 (
	IMAGE * palette,
	int ncolors,
	int r, int g, int b
)
{
    int p;

    if( !palette )  return( 0 );

    for( p=0 ; p<255 && p<ncolors ; p++ )
    {
	if(   ( palette->data[p].r == r )
	   && ( palette->data[p].g == g )
	   && ( palette->data[p].b == b ) )
	    return( p );
    }

    return( 0 );
}


/* Color_Reduce
 *
 *   reduce colors in the 'a' channel based on the palette image passed in
 */
void 
Color_Reduce24 (
	IMAGE * img,
	IMAGE * palette,
	int ncolors 
)
{
    int xp[255];
    int t;
    int p;

    /* simple check */
    if( !img || !palette )  return;

    printf("*** Untested code reached AAA.  Contact Jerry\n");

    /* fill the cross-reference palette */
    for( p=0 ; p<255 ; p++ )
    {
	if( p<ncolors )
	    xp[p] = p;	/* correct value */
	else
	    xp[p] = -1;	/* default value of -1 for unknown */
    }

    /* for each pixel in the image */
    for( p=0 ; p < img->height * img->width ; p++ )
    {
	if( img->data[p].a >= ncolors ) 
	{
	    /* check for a reference in the x-ref palette */
	    t = xp[ img->data[p].a ];

	    /* if there wasn't any... */
	    if( t == -1 )
	    {
		/* figure out the proper color to use */
		t = Color_Find24( palette, ncolors, 
				    img->data[p].r,
				    img->data[p].g,
				    img->data[p].b );
		
		/* store it in the crossreference palette */
		xp[ img->data[p].a ] = t;
		
		/* shove it into the image */
		img->data[p].a = t;
		printf("%d\n", t);

	    } else {
		/* we already figured it out, just set it... */
		img->data[p].a = t;
	    }
	}
    }
}


/* Color_Find_Integrated
 * 
 *  finds a color with an a value of less than 'ncolors', containing the 
 *  same RGB value as the index passed in.  If none were found, '0' is 
 *  returned.  This returns an index into the passed in image for the
 *  values to reassign.
 */
int
Color_Find_Integrated ( IMAGE * img, int ncolors, int refpixel, int * xp )
{
    int p;
    int r,g,b;

    if( !img )  return( 0 );

    r = img->data[refpixel].r;
    g = img->data[refpixel].g;
    b = img->data[refpixel].b;

    for( p=0 ; p<ncolors ; p++ )
    {
	if(    ( img->data[ xp[p] ].r == img->data[refpixel].r ) 
	    && ( img->data[ xp[p] ].g == img->data[refpixel].g )
	    && ( img->data[ xp[p] ].b == img->data[refpixel].b ))
	{
	    return( xp[p] );
	}
    }

    return( 0 );
}


/* Color_ReduceIntegrated
 *
 *   reduce colors in the image to ncolors
 */
void
Color_ReduceIntegrated (
        IMAGE * img,
        int ncolors
)
{
    int xp[255];
    int maxfound = -1;
    int p;
    int t;

    if( !img )  return;

/*

	We know at this point that the rgb values and the a value
	are both set.  We can look at the 'a' value and check to
	see if it's greater than [[ncolors]].

	XP will be filled with references to the first color in the
	passed in image.  It will include numbers => ncolors.  We 
	can then check to see if any were greater than ncolors,
	using [[maxfound]].  If there were, reassign those to 
	proper other colors in the palette.
*/

    /* fill the crossreference list with -1's */
    for( p=0 ; p<255 ; p++ )   xp[ p ] = -1;

    /* cache the "palette" cross-references used for the image */
    for( p=0 ; p < img->height * img->width ; p++ )
    {
	if( xp[ img->data[p].a ] == -1 )
	    xp[ img->data[p].a ] = p;

	if( img->data[p].a > maxfound )
	    maxfound = img->data[p].a;
    }


    /* if we found a reference greater than ncolors, reassign them. */
    if( maxfound >= ncolors )
    {
	/* do this for all pixels */
	for( p=0 ; p < img->height * img->width ; p++ )
	{
	    if( img->data[p].a >= ncolors )
	    {
		/* reassign the color for that pixel */
		t = Color_Find_Integrated( img, ncolors, p, xp );
		img->data[p].r = img->data[t].r;
		img->data[p].g = img->data[t].g;
		img->data[p].b = img->data[t].b;
		img->data[p].a = img->data[t].a;
	    }
	}
    }
}

