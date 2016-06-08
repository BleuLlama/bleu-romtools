/* Turaco CL GUI Experiment - bitmap manipulations
**
**	bits.h
**
**  2008-02-08+  Scott Lawrence
**  http://umlautllama.com
*/

/* This code is considered to be freeware.  Do with it as you wish.
** I claim no responsibility for anything bad that might be a direct
** or indirect result of the usage of this code.
*/


#ifndef __BITS_H__
#define __BITS_H__

/******************************************************************************/
/* bitmap manipulations */


/* bits_new
**	allocate a new bitmap
*/
bits * bits_new( int w, int h );


/* bits_free
**	free all memory associated with a bitmap
*/
void bits_free( bits * bmp );


/* bits_resize
**	pass in a bits; if the new one is smaller or equal, just return it
**	if no bits passed in, just create a new one
*/
bits * bits_resize( bits * bmp, int w, int h );



/* bits_setp
**	set a color in a bits
*/
void bits_setp( bits * bmp, int x, int y, int color );


/* bits_getp
**	get a color in a bits
*/
int bits_getp( bits * bmp, int x, int y );


/* bits_copy
**	copy one buffer to another
*/
void bits_copy( bits * dst, bits * src );


/* bits_fill
**	fill with a test pattern
*/
void bits_fill( bits * bmp );


/******************************************************************************/
/* basic manipulations */


/* bits_clear
**	clear the main bits to the current color
*/
void bits_clear( gui_handle * gui );


/* bits_swap
**	swap the main bits with the spare
*/
void bits_swap( gui_handle * gui );


/* bits_copy_to_swap
**	copy main to swap
*/
void bits_copy_to_swap( gui_handle * gui );


/* bits_flip_horiz
**	flip the image horizontally
*/
void bits_flip_horiz( gui_handle * gui );


/* bits_flip_vert
**	flip the image vertically
*/
void bits_flip_vert( gui_handle * gui );


/* bits_mirror_horiz
**	mirror the image horizontally
*/
void bits_mirror_horiz( gui_handle * gui );


/* bits_mirror_vert
**	mirror the image vertically
*/
void bits_mirror_vert( gui_handle * gui );


/* bits_rotate_90
**	rotate the image 90 degrees CW
**	this does not work if the image is non-square.  snoogins.
*/
void bits_rotate_90( gui_handle * gui );


/* bits_wrap
**	simple helper to constrain within ranges
*/
int bits_wrap( int v, int max );


/* bits_shift
**	shift the image to the left/right/up/down
*/
void bits_shift( gui_handle * gui, int xx, int yy );


/* bits_swap_colors
**	swaps the color under the bits cursor with the palette current
*/
void bits_swap_colors( gui_handle * gui );


/* bits_fill_all
**	sets all occurrences of the color under the cursor with the palette 
*/
void bits_fill_all( gui_handle * gui );


/* bits_undo_checkpoint
**	copies main to undo
*/
void bits_undo_checkpoint( gui_handle * gui );


/* bits_undo_swap
**	copies undo to main and vice versa
*/
void bits_swap_undo( gui_handle * gui );


/******************************************************************************/
/* misc useless stuff */


/* bits_conway
**	apply conway's game of life to the main window
*/
void bits_conway( gui_handle * gui );

#endif
