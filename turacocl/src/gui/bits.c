/* Turaco CL GUI Experiment - bitmap manipulations
**
**	bits.c
**
**  2008-02-08+  Scott Lawrence
**  http://umlautllama.com
*/

/* This code is considered to be freeware.  Do with it as you wish.
** I claim no responsibility for anything bad that might be a direct
** or indirect result of the usage of this code.
*/

#include <stdlib.h> /* for calloc(), free() */
#include <string.h> /* memcpy() */
#include <curses.h> /* tmgui.h needs this. */
#include "tmgui.h"  /* textmode gui */
#include "bits.h"   /* bitmap manipulations */


/******************************************************************************/
/* bitmap manipulations */


/* bits_new
**	allocate a new bitmap
*/
bits * bits_new( int w, int h )
{
	bits * bmp = (bits *)calloc( 1, sizeof( bits ));
	if( !bmp ) return( NULL );

	bmp->w = w;
	bmp->h = h;
	bmp->p = (int *)calloc( 1, w * h * sizeof( int ));
	return( bmp );
}


/* bits_free
**	free all memory associated with a bitmap
*/
void bits_free( bits * bmp )
{
	if( bmp )
	{
		if( bmp->p ) free( bmp->p );
		free( bmp );
	}
}


/* bits_resize
**	pass in a bits; if the new one is smaller or equal, just return it
**	if no bits passed in, just create a new one
*/
bits * bits_resize( bits * bmp, int w, int h )
{
	if( !bmp )
	{
		return( bits_new( w, h ));
	} else {
		if( bmp->w >= w  &&  bmp->h >= h )  return( bmp );
		bits_free( bmp );
		return( bits_new( w, h ));
	}
}


/* bits_setp
**	set a color in a bits
*/
void bits_setp( bits * bmp, int x, int y, int color )
{
	if( !bmp || !bmp->p ) return;

	bmp->p[ x + (y * bmp->h) ] = color;
}


/* bits_getp
**	get a color in a bits
*/
int bits_getp( bits * bmp, int x, int y )
{
	if( !bmp || !bmp->p ) return 0;

	if( x<0 || y<0 ) return 0;
	if( x>=bmp->w || y>=bmp->h ) return 0;

	return( bmp->p[ x + (y * bmp->h) ] );
}


/* bits_copy
**	copy one buffer to another
*/
void bits_copy( bits * dst, bits * src )
{
	if( !dst || !dst->p || !src || !src->p ) return;
	if( dst->w != src->w || dst->h != src->h ) return;

	memcpy( dst->p, src->p, sizeof( int ) * dst->w * dst->h );
}


/*
The following isn't necessary, but it's fun. :)

            111111
  0123456789012345
 0
 1   #   #   #
 2  ### ##|  ####
 3   #   #  #####
 4          ##
 5         ###
 6         ###
 7  ######|###
 8 -------+---
 9 #######|###
10  ######|##
11  ##    |#
12  ##    |#
13  #      #
14  ##     ##
15
*/

/* this data is: startx, starty, color, horizontal pixels */
int test_fill_data[]= {
	/* umlauts */
	3, 1, 3, 1,	7, 1, 3, 1,
	2, 2, 3, 3,	6, 2, 3, 3, /* 6,2, color 3, 3 pixels horizontally */
	3, 3, 3, 1,	7, 3, 3, 1, 

	/* llama */
	11, 1, 7, 1,	11, 2, 7, 4,	10, 3, 7, 5,
	10, 4, 7, 2,	9, 5, 7, 3,	9, 6, 7, 3,
	2,  7, 7, 10,	1, 8, 7, 11,	1, 9, 7, 11,
	2, 10, 7, 9,	
	2, 11, 7, 2,	8, 11, 7, 2,
	2, 12, 7, 2,	8, 12, 7, 2,	
	2, 13, 7, 1,	9, 13, 7, 1,
	2, 14, 7, 2,	9, 14, 7, 2,
	
	-1, -1, -1, -1 /* end */
};

/* bits_fill
**	fill with a test pattern
*/
void bits_fill( bits * bmp )
{
	int x,y, c;
	if( !bmp || !bmp->p ) return;

	/* fill the background with sky and ground. */
	for( x=0 ; x<bmp->w ; x++ )
	{
		for( y=0 ; y<bmp->h ; y++ )
		{
			if( y<12 ) c=4; /* sky */
			else c=2; /* grass */

			bits_setp( bmp, x, y, c );
		}
	}

	for( x=0; test_fill_data[x] != -1 ; x+=4 ) {
		for( y=0; y<test_fill_data[x+3] ; y++ ) 
		{
			bits_setp( bmp,
			       test_fill_data[ x ]+y, test_fill_data[ x+1 ],
			       test_fill_data[ x+2 ] );
		}
	}
}


/******************************************************************************/
/* basic manipulations */


/* bits_clear
**	clear the main bitmap to the current color
*/
void bits_clear( gui_handle * gui )
{
	/* for some reason, i can't seem to get this to work with 
	   a memset() properly.  oh well */
	int x,y;

	bits_undo_checkpoint( gui );

	for( x=0 ; x<gui->main->w ; x++ )
	{
		for( y=0 ; y<gui->main->h ; y++ )
		{
			bits_setp( gui->main, x, y, gui->pal.cursor );
		}
	}
}


/* bits_undo_checkpoint
**	copies main to undo
*/
void bits_undo_checkpoint( gui_handle * gui )
{
	bits_copy( gui->undo, gui->main );
}

/* bits_undo_swap
**	copies undo to main and vice versa
*/
void bits_swap_undo( gui_handle * gui )
{
	bits_copy( gui->temp, gui->main );
	bits_copy( gui->main, gui->undo );
	bits_copy( gui->undo, gui->temp );
}


/* bits_swap
**	swap the main bits with the spare
*/
void bits_swap( gui_handle * gui )
{
	bits_copy( gui->temp, gui->main );
	bits_copy( gui->main, gui->swap );
	bits_copy( gui->swap, gui->temp );
}


/* bits_copy_to_swap
**	copy main to swap
*/
void bits_copy_to_swap( gui_handle * gui )
{
	bits_copy( gui->swap, gui->main );
}


/* bits_flip_horiz
**	flip the image horizontally
*/
void bits_flip_horiz( gui_handle * gui )
{
	int x,y;

	bits_undo_checkpoint( gui );

	for( x=0 ; x<gui->main->w ; x++ )
	{
		for( y=0 ; y<gui->main->h ; y++ )
		{
			bits_setp( gui->temp, gui->main->w-1-x, y,
				bits_getp( gui->main, x, y ));
		}
	}
	bits_copy( gui->main, gui->temp );
}


/* bits_flip_vert
**	flip the image vertically
*/
void bits_flip_vert( gui_handle * gui )
{
	int x,y;

	bits_undo_checkpoint( gui );

	for( x=0 ; x<gui->main->w ; x++ )
	{
		for( y=0 ; y<gui->main->h ; y++ )
		{
			bits_setp( gui->temp, x, gui->main->h-1-y,
				bits_getp( gui->main, x, y ));
		}
	}
	bits_copy( gui->main, gui->temp );
}


/* bits_mirror_horiz
**	mirror the image horizontally
*/
void bits_mirror_horiz( gui_handle * gui )
{
	int x,y;

	bits_undo_checkpoint( gui );

	for( x=0 ; x<gui->main->w/2 ; x++ )
	{
		for( y=0 ; y<gui->main->h ; y++ )
		{
			bits_setp( gui->main, gui->main->w-1-x, y,
				bits_getp( gui->main, x, y ));
		}
	}
}


/* bits_mirror_vert
**	mirror the image vertically
*/
void bits_mirror_vert( gui_handle * gui )
{
	int x,y;

	bits_undo_checkpoint( gui );

	for( x=0 ; x<gui->main->w ; x++ )
	{
		for( y=0 ; y<gui->main->h/2 ; y++ )
		{
			bits_setp( gui->main, x, gui->main->h-1-y,
				bits_getp( gui->main, x, y ));
		}
	}
}


/* bits_rotate_90
**	rotate the image 90 degrees CW
**	this does not work if the image is non-square.  snoogins.
*/
void bits_rotate_90( gui_handle * gui )
{
	int x,y;

	bits_flip_vert( gui );

	for( x=0 ; x<gui->main->w ; x++ )
	{
		for( y=0 ; y<gui->main->h ; y++ )
		{
			bits_setp( gui->temp, y, x,
				bits_getp( gui->main, x, y ));
		}
	}
	bits_copy( gui->main, gui->temp );
}


/* bits_wrap
**	simple helper to constrain within ranges
*/
int bits_wrap( int v, int max )
{
	if( v>=max ) return( 0 );
	if( v<0 )   return( max-1 );
	return( v );
}


/* bits_shift
**	shift the image to the left/right/up/down
*/
void bits_shift( gui_handle * gui, int xx, int yy )
{
	int x,y;
	int nx, ny;

	bits_undo_checkpoint( gui );

	for( x=0 ; x<gui->main->w ; x++ )
	{
		for( y=0 ; y<gui->main->h ; y++ )
		{
			nx = bits_wrap( x-xx, gui->main->w );
			ny = bits_wrap( y-yy, gui->main->h );

			bits_setp( gui->temp, x, y, 
				bits_getp( gui->main, nx, ny ));
		}
	}
	bits_copy( gui->main, gui->temp );
}


/* bits_swap_colors
**	swaps the color under the cursor with the palette color
*/
void bits_swap_colors( gui_handle * gui )
{
	int x,y;
	int color1, color2;
	int cs;

	color1 = bits_getp( gui->main, gui->bited.cx, gui->bited.cy );
	color2 = gui->pal.cursor;

	bits_undo_checkpoint( gui );

	for( x=0 ; x<gui->main->w ; x++ )
	{
		for( y=0 ; y<gui->main->h ; y++ )
		{
			cs = bits_getp( gui->main, x, y );
			if( cs == color1 ) cs = color2;
			else if( cs == color2 ) cs = color1;
			bits_setp( gui->main, x, y, cs );
		}
	}
}


/* bits_fill_all
**	sets all occurrences of the color under the cursor with the palette 
*/
void bits_fill_all( gui_handle * gui )
{
	int x,y;
	int color1, color2;
	int cs;

	color1 = bits_getp( gui->main, gui->bited.cx, gui->bited.cy );
	color2 = gui->pal.cursor;

	bits_undo_checkpoint( gui );

	for( x=0 ; x<gui->main->w ; x++ )
	{
		for( y=0 ; y<gui->main->h ; y++ )
		{
			cs = bits_getp( gui->main, x, y );
			if( cs == color1 ) cs = color2;
			bits_setp( gui->main, x, y, cs );
		}
	}
}


/******************************************************************************/
/* misc useless stuff */


/* bits_conway
**	apply conway's game of life to the main window
*/
void bits_conway( gui_handle * gui )
{
	int x,y,v;

	for( x=0 ; x<gui->main->w ; x++ )
	{
		for( y=0 ; y<gui->main->h ; y++ )
		{
			/* count up the number of cells aruond the current */
			v=0;
			v += (bits_getp( gui->main, x-1, y-1 )) ? 1 : 0;
			v += (bits_getp( gui->main, x-1, y   )) ? 1 : 0;
			v += (bits_getp( gui->main, x-1, y+1 )) ? 1 : 0;
			v += (bits_getp( gui->main, x  , y-1 )) ? 1 : 0;
			v += (bits_getp( gui->main, x  , y+1 )) ? 1 : 0;
			v += (bits_getp( gui->main, x+1, y-1 )) ? 1 : 0;
			v += (bits_getp( gui->main, x+1, y   )) ? 1 : 0;
			v += (bits_getp( gui->main, x+1, y+1 )) ? 1 : 0;

			/* play the game */
			if( bits_getp( gui->main, x, y ) != 0 ) {
				/* live cell */
				if( v<2 || v>3 ) 
					bits_setp( gui->temp, x, y, 0 );
				else 
					bits_setp( gui->temp, x, y, 1 );
			} else {
				/* dead cell */
				if( v==3 ) 
					bits_setp( gui->temp, x, y, 1 );
				else 
					bits_setp( gui->temp, x, y, 0 );
			}
		}
	}

	/* and copy the temp buffer back to the main buffer */
	bits_copy( gui->main, gui->temp );
}
