/* Turaco CL GUI Experiment - textmode gui - popup menu
**
**	popup.c
**
**  2008-02-08+  Scott Lawrence
**  http://umlautllama.com
*/

/* This code is considered to be freeware.  Do with it as you wish.
** I claim no responsibility for anything bad that might be a direct
** or indirect result of the usage of this code.
*/

#include <curses.h>
#include <stdio.h>
#include <string.h> /* for strlen() */
#include "tmgui.h"  /* textmode gui */
#include "bits.h"   /* bitmap editing tools */


/* quickie macro to print a centered text string */
#define wprcent( W, L, S ) \
	mvwprintw( (W), (L), cols/2-(strlen(S)/2), "%s", (S) );

/* gui_popup_requestor
**	application
**	display a popup menu selector thingy (Modal)
**	returns the item number selected [0..N] or -1 if cancelled
**	use the arrow keys to select the item
**	space/return to select, escape to cancel
*/
int gui_popup_requestor( gui_handle * gui, char * info, char ** items, 
			int allowCancel )
{
	WINDOW * req = NULL;
	int done = 0;
	int ret = -1;
	int ci = 0;
	int p, c;
	int nitems;
	int cols, rows;
	int startx, starty;
	int ch;

	/* first. count the items, but also find the widest blob */
	cols = strlen( info );
	for( nitems=0 ; items[nitems] ; nitems++ )
	{
		if( cols < strlen( items[nitems] ))
			cols = strlen( items[nitems] );
	}

	/* adjust the sizes for the border and padding */
	cols += 6;
	rows = nitems + 4;

	/* now figure out the start[x|y] to center the window */
	startx = (gui->w/2)-(cols/2);
	starty = (gui->h/2)-(rows/2);

	/* create the new window */
	req = newwin( rows, cols, starty, startx );

	/* print the info header */
	wattron( req, COLOR_PAIR( COLOR_KEYSTROKE ));
	wprcent( req, 1, info );
	wattroff( req, COLOR_PAIR( COLOR_KEYSTROKE ));

	/* main input loop */
	while( !done )
	{
		/* draw the border */
		gui_colorborder( req, COLOR_BORDER_SELECTED );

		/* draw the items, highlighting the one with focus */

		for( p=0 ; p<nitems ; p++ )
		{
			/* unselected, unless selected. */
			wattron( req, COLOR_PAIR(COLOR_UNSELECTED) );
			if( ci == p ) 
				wattron( req, COLOR_PAIR(COLOR_SELECTED) );

			/* backfill the line */
			for( c=2 ; c<cols-2 ; c++ )
			{
				mvwaddch( req, 3+p, c, ' ' );
			}

			/* print out the text, centered */
			wprcent( req, 3+p, items[p] );

			/* position display */
			mvwaddch( req, 3+p,      1, (ci==p)?ACS_LARROW:' ' );
			mvwaddch( req, 3+p, cols-2, (ci==p)?ACS_RARROW:' ' );

			/* unselected, unless selected. */
			if( ci == p ) 
				wattroff( req, COLOR_PAIR(COLOR_SELECTED) );
			wattroff( req, COLOR_PAIR(COLOR_UNSELECTED) );

		}

		/* refresh it to the screen */
		wrefresh( req );
		refresh();

		/* get input */
		ch = getch();
		switch( ch )
		{
		case( KEY_DOWN ): /* next item */
			ci++;
			if ( ci >= nitems ) ci = 0;
			break;

		case( KEY_UP ): /* previous item */
			ci--;
			if ( ci < 0 ) ci = nitems-1;
			break;

		case( KEY_ENTER ): /* select item */
		case( 10 ):
		case( ' ' ):
			ret = ci;
			done = 1;
			break;

		case( 27 ): /* escape */ /* cancel out */
			if( allowCancel )
			{
				done = 1;
				ret = -1;
			}
			break;
	
		}
	}

	/* clear the memory */
	delwin( req );

	/* clear the screen */
	clear();
	refresh();

	/* and return the value */
	return( ret );
}
