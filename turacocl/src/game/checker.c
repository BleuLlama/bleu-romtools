/*
 * checker.h
 *
 *  checkerboard generation
 *
 * $Id: checker.c,v 1.2 2003/05/20 03:41:17 jerry Exp $
 *
 * $Log: checker.c,v $
 * Revision 1.2  2003/05/20 03:41:17  jerry
 * First jab at tilemap generation
 *
 * Revision 1.1  2003/05/19 20:44:58  jerry
 * Moved the checkerboard generation out of "turaco.c" and into "checker.c"
 * (some changes are going to happen to it for tilemap stuff.  It needs to be more modular.)
 *
 *
 */

#include <stdio.h>
#include "turaco.h"
#include "tilemap.h"
#include "params.h"
#include "coding.h"
#include "gfxlib.h"
#include "misc.h"
#include "errors.h"


/* checker_Generate
 *
 *  generate the chekcerboard image into the loadedGraphics IMAGE
 */
void
checker_Generate( TuracoInstance * ti )
{
    char lut[16] = "0123456789";
    int px, py;
    int x, y;
    int xoff = 0;
    int yoff = 0;
    int RGBcolor;
    int Icolor;
    char l1, l2, l3, l4;
    PIXEL p;

    /*      hundreds  0123456789 */
    char h1lut[16] = " < <= => ><<==>>";
    char h2lut[16] = "  << == >>=><><=";
    /* the pattern here is:
     * < [  .]
     * = [ ..]
     * > [...]
     *
     *                   12
     * <100 [   ][   ]  "  "
     *  100 [  .][   ]  "< "
     *  200 [   ][  .]  " <"
     *  300 [  .][  .]  "<<"
     *  400 [ ..][   ]  "= "
     *  500 [   ][ ..]  " ="
     *  600 [ ..][ ..]  "=="
     *  700 [...][   ]  "> "
     *  800 [   ][...]  " >"
     *  900 [...][...]  ">>"
     */

    if( !ti || !ti->loadedGraphics )  return;

    for ( px=0 ; px < ti->loadedGraphics->width ; px++ )
    {
	x = px / ti->gd->gfxDecodes[ti->up->bnk].width;

	for( py=0 ; py<ti->loadedGraphics->height ; py++ )
	{
	    y = py / ti->gd->gfxDecodes[ti->up->bnk].height;

	    if ( (!(x&1) && !(y&1)) || ((x&1) == (y&1)) )
	    {
		RGBcolor = (1<< (ti->loadedGraphics->bits)) -1;
		Icolor = 1;
	    } else {
		RGBcolor = 0;
		Icolor = 0;
	    }

	    ti->loadedGraphics->data[ py*ti->loadedGraphics->width + px].r =
	    ti->loadedGraphics->data[ py*ti->loadedGraphics->width + px].g =
	    ti->loadedGraphics->data[ py*ti->loadedGraphics->width + px].b =
		RGBcolor;

	    ti->loadedGraphics->data[ py*ti->loadedGraphics->width + px].a =
		Icolor;
	}
    }

    /* now throw the indexing in there */

    xoff = yoff = 0;

    /* determine the offsets for the centers of the numbers */
    if( ti->gd->gfxDecodes[ti->up->bnk].width > 8 )
	xoff = (ti->gd->gfxDecodes[ti->up->bnk].width - 8)/2;

    if( ti->gd->gfxDecodes[ti->up->bnk].height > 8 )
	yoff = (ti->gd->gfxDecodes[ti->up->bnk].height - 8)/2;

    x = 0;

    /* generate a custom color */
    p.r = font_Red.r;
    p.g = font_Red.g;
    p.b = font_Red.b;
    p.a = 3;


    for( py=0 ; 
	 py < ti->loadedGraphics->height ; 
	 py += ti->gd->gfxDecodes[ti->up->bnk].height )
    {
	for( px=0 ; 
	     px < ti->loadedGraphics->width ; 
	     px += ti->gd->gfxDecodes[ti->up->bnk].width )
	{
	    /* two-position, only for narrow (8x8) sprites */
	    if( ti->gd->gfxDecodes[ ti->up->bnk ].width < 16 )
	    {
		/* adjust color */
		if( x == 1000 )
		{
		    p.a = 2;
		    x -= 1000;
		}

		/* setup the two 'letters' */
		l1 = lut[(x/10)%10];
		l2 = lut[(x)%10];

		font_RenderChar( ti->loadedGraphics, &p,
			font_Internal_36, xoff+px, yoff+py, l1);

		font_RenderChar( ti->loadedGraphics, &p,
			font_Internal_36, xoff+px+4, yoff+py, l2);

		/* check for hundreds */
		if( x < 1000 )
		{
		    char h1 = h1lut[(x/100)%10];
		    char h2 = h2lut[(x/100)%10];

		    font_RenderChar( ti->loadedGraphics, &p,
			font_Internal_36, xoff+px, yoff+py, h1);

		    font_RenderChar( ti->loadedGraphics, &p,
			font_Internal_36, xoff+px+4, yoff+py, h2);
		}

	    } else {
		/* let's handle more than 9999 sprites... */
		if( x == 10000 )
		{
		    p.a = 2;
		    x -= 10000;
		}

		/* sprite is wide enough for up to 9999 value */

		l1 = lut[ (x/1000) % 10 ];
		l2 = lut[ (x/100) % 10 ];
		l3 = lut[ (x/10) % 10 ];
		l4 = lut[ (x) % 10 ];

                font_RenderChar( ti->loadedGraphics, &p,
                        font_Internal_36, xoff+px-4, yoff+py, l1);

                font_RenderChar( ti->loadedGraphics, &p,
                        font_Internal_36, xoff+px, yoff+py, l2);

                font_RenderChar( ti->loadedGraphics, &p,
                        font_Internal_36, xoff+px+4,   yoff+py, l3);

                font_RenderChar( ti->loadedGraphics, &p,
                        font_Internal_36, xoff+px+8, yoff+py, l4);

	    }

	    x++;
	}
    }
}
