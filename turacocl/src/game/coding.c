/*
 * coding.c
 *
 * based heavily on "CODING.C" from the Turaco project
 *
 * CODING.C originally written by Ivan Mackintosh, 1998 November
 *          sped up and partially re-written by Jerry, 1998 December
 *
 * $Id: coding.c,v 1.14 2003/05/15 02:43:26 jerry Exp $
 */

#include "coding.h"
#include "driver.h"


/*
 * the roms are basically encoded like so:
 *  (ref: inidriv.txt, driver.h )

 **********
 *      I pretty much wrote this to jog my memory, although the side
 *      effeect is that the code gets explained well because of this.
 *      The original Turaco source had no commenting, and after the
 *      second pass of optimizations, was nearly impossible to figure
 *      out. I'm glad I kept around an early (non-optimized) version.
 *      heh.  
 **********

	For each graphics decode, the bank starts at the [[start]]
	address, which is a reference into the Rom[n] space.

	Each sprite is [[width]] pixels wide, and [[height]] pixels
	tall in this bank.

	There are [[nsprites]] sprites in the bank total.

	[[orientation]] defines how the characters need to be rotated
	for them to appear "right side up"

	There are [[planes]] bitplanes, meaning that there are 2^planes
	colors used.  The rom data is stored as seperate bitplanes,
	where when the bitplanes are combined, the completed byte is a
	reference into the palette. (This is much like Amiga IFF files,
	except that there is no RLE compression, or really, any
	compression at all.)

	Each sprite starts [[charincrement]] bits off from the previous
	sprite.

	The following example is for sprite number [N], using
	inidriv.txt's example driver.

	Each sprite starts at (start + (N * charincrement)) bits in the
	romspace.

	    sprite_N_Start = start + (N * charincrement)

			   = 0 + (N * 128) = N * 128  bits

	From there, each plane starts at the respective offset as
	defined in the [[planeoffsets]] array. (this is also in bits,
	NOT BYTES)

	    plane_0_Start = spriteStart + planeoffsets[0]

			  = (N * 128) + 0


	    plane_1_Start = spriteStart + planeoffsets[1]

			  = (N * 128) + 4

	Okay, for each plane, we reference the [[xoffsets]] and
	[[yoffsets]] arrays to determine where each pixel on each
	bitplane is in the romspace.

	    value( x, y ) = (plane_start + xoffset[x] + yoffset[y])

	So basically, if [[value(x, y)]] is set, then you set the image
	buffer like so:

	    buffer[x][y] = (value(x, y)) ? 1 << whichPlaneThisIsFor : 0 

	During this last step, we can do the rotations necessary to 
	rotate the sprite according to the [[orientation]].

	    No change:      buffer[ x          ][ y          ]

	    Flip X:         buffer[ width-1-x  ][ y          ]

	    Flip Y:         buffer[ x          ][ height-1-y ]

	    Rotate CW 90:   buffer[ height-1-y ][ x          ]

	    Rotate CW 180:  buffer[ width-1-x  ][ height-1-y ]

	    Rotate CW 270:  buffer[ height-1-y ][ width-1-x  ]


	That's pretty much it.  Reverse the process to encode the data
	back into graphics roms.

 */

#include "driver.h"
#include "gfxlib.h"


/* coding_GetBit
 * 
 *   get a bit from the buffer
 */
int
coding_GetBit( char * buffer, long bitnumber, long bufsize )
{
    long nbyte = bitnumber >> 3;
    int nbit = bitnumber & 7;    /* using an &7 is quicker than an %8 */
    int mask = 0x80 >> nbit;

    /* bounds check */
    if( nbyte < 0 || nbyte > bufsize )
	    return( 0 );

    return ((buffer[nbyte] & mask) == mask);
}


/* coding_SetBit
 *
 *  set a bit in the buffer
 */
void
coding_SetBit( char * buffer, long bitnumber, int value, long bufsize )
{
    long nbyte = bitnumber >> 3;
    int nbit = bitnumber & 7;
    int mask = 0x80 >> nbit;

    /* bounds check */
    if( nbyte < 0 || nbyte > bufsize )
	return;

    buffer[nbyte] &= ~mask;

    if (value)
	buffer[nbyte] |= mask;
}


/* coding_EncodeSprite
 *
 * takes in a [gfxDecode], and Converts sprite number
 * [spriteNumber] from the [image] into the [romBuffer]
 *
 * NOTE: This loads the image data into the alpha channel of the IMAGE.
 *       It needs to be converted externally to save it properly
 */
void
coding_EncodeSprite(
        GfxDecode * gd,		int spriteNumber,
        IMAGE * image,          int xpos, int ypos,
        char * romBuffer,       long bufsize )
{
    int x,y,p;
    int pmask;
    long poffs;
    int xoffs, yoffs;
    int usex, usey;
    int imagepos;
    long startOffsetBits;
    int bit;

    /* make sure the parameters aren't bogus. */
    if( !gd || !image || !romBuffer )  return;

    /* determine the start offset of the sprite */
    startOffsetBits = ( gd->startaddress * 8 ) +
                       (gd->charincrement * spriteNumber );


    for( p=0 ; p < gd->planes ; p++ )
    {
	pmask = 1<<p;

	poffs = gd->planeoffsets[p];	/* the offset for the plane */
        for( x=0 ; x < gd->width ; x++ )
        {
            xoffs = gd->xoffset[x];     /* the offset for x */       

            for( y=0 ; y < gd->height ; y++ )
            {
                yoffs = gd->yoffset[y]; /* the offset for y */


                    /* adjust based on the orientation */
                    switch( gd->orientation )
                    {
                        case( ORIENTATION_FLIP_X ):
                            usex = gd->width - 1 - x;
                            usey = y;
                            break;       

                        case( ORIENTATION_FLIP_Y ):
                            usex = x;
                            usey = gd->height - 1 - y;
                            break;

                        case( ORIENTATION_SWAP_XY ):
                            usey = x;
                            usex = y;
			    break;

                        case( ORIENTATION_ROTATE_90 ):
                            usex = gd->height - 1 - y;
                            usey = x;
                            break;

                        case( ORIENTATION_ROTATE_180 ):
                            usex = gd->width - 1 - x;
                            usey = gd->height - 1 - y;
                            break;

                        case( ORIENTATION_ROTATE_270 ):
                            usex = gd->height - 1 - y;
                            usey = gd->width - 1 - x;
                            break;

                        case( ORIENTATION_DEFAULT ):
                        default:
                            usex = x;
                            usey = y;
                            break;
                    }
                    imagepos = (((ypos * gd->height) + usey) * image->width) +
                                ((xpos * gd->height) + usex);

                    if( image->data[ imagepos ].a & pmask )
                        bit = 1;
                    else
                        bit = 0;

                    coding_SetBit( romBuffer,
                            startOffsetBits + poffs + xoffs + yoffs, bit,
                            bufsize );       
	    }
	}
    }
}
 


/* coding_DecodeSprite
 *
 * takes in a [gfxDecode], and fabricates an [image] from the
 * sprite specified as [spriteNumber] from the [romBuffer]
 *
 * Range checking is assumed to have happened before this, therefore
 * there is no return value.
 *
 * NOTE: This looks for the image data in the alpha channel of the IMAGE.
 *       It needs to be converted externally to load it properly
 */
void
coding_DecodeSprite(
        GfxDecode * gd,		int spriteNumber,
        IMAGE * image,          int xpos, int ypos,
        char * romBuffer,       long bufsize )
{
    long startOffsetBits = 0l;
    int x,y,p, usex,usey;
    int bit=0, imagepos;
    long poffs, xoffs, yoffs;

    /* make sure the parameters aren't bogus. */
    if( !gd || !image || !romBuffer )  return;

    /* determine the start offset of the sprite */
    startOffsetBits = ( gd->startaddress * 8 ) +
                       (gd->charincrement * spriteNumber );

    for( p=0 ; p < gd->planes ; p++ )
    {
	poffs = gd->planeoffsets[p];	/* the offset for the plane */

	for( x=0 ; x < gd->width ; x++ )
	{
	    xoffs = gd->xoffset[x];	/* the offset for x */

	    for( y=0 ; y < gd->height ; y++ )
	    {
		yoffs = gd->yoffset[y];	/* the offset for y */

		bit = coding_GetBit( romBuffer,
				    startOffsetBits + poffs + xoffs + yoffs, 
				    bufsize );

		/* adjust based on the orientation */
		switch( gd->orientation )
		{
                        case( ORIENTATION_FLIP_X ):
                            usex = gd->width - 1 - x;
                            usey = y;
                            break;

                        case( ORIENTATION_FLIP_Y ):
                            usex = x;
                            usey = gd->height - 1 - y;
                            break;

                        case( ORIENTATION_SWAP_XY ):
                            usey = x;
                            usex = y;
                            break;    

                        case( ORIENTATION_ROTATE_90 ):
                            usex = gd->height - 1 - y; 
                            usey = x;
                            break;

                        case( ORIENTATION_ROTATE_180 ):
                            usex = gd->width - 1 - x;
                            usey = gd->height - 1 - y;
                            break;      

                        case( ORIENTATION_ROTATE_270 ):
                            usex = gd->height - 1 - y;
                            usey = gd->width - 1 - x;
                            break;

                        case( ORIENTATION_DEFAULT ):
                        default:
                            usex = x;
                            usey = y;      
                            break;
		}

		/* generate the offset */
		imagepos = (((ypos * gd->height) + usey) * image->width) +
                                ((xpos * gd->height) + usex);

		if( bit )
		{
		    image->data[ imagepos ].a |= 1 << p;
		}
	    }
	}
    }
}



/* codingGenHerringbone
 *    
 *  generates a sprite-sized herringbone at (xpos,ypos) in image
 */
void
coding_GenHerringbone( GfxDecode * gd,
                        IMAGE * image,
                        int xpos, int ypos)
{
    int x,y;
    int c=0;
    int d=0;
    int imagepos;

    for( y=0 ; y < gd->height ; y++ )
    {
	d ^= 1;
	for( x=0 ; x < gd->width ; x++ )
	{
	    imagepos = (((ypos * gd->height) + y) * image->width) 
			+ (xpos * gd->width) + x;

	    c ^= 1;
	    image->data[ imagepos ].a = c+1 + d;
	}
    }
}

