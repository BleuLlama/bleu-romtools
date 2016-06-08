/* Turaco CL GUI Experiment - textmode gui - status bar
**
**	status.c
**
**  2008-02-08+  Scott Lawrence
**  http://umlautllama.com
*/

/* This code is considered to be freeware.  Do with it as you wish.
** I claim no responsibility for anything bad that might be a direct
** or indirect result of the usage of this code.
*/

/* something to note:
**	curses/ncurses addresses the screen as line, column;  y,x
**	our bitmaps address it as column, line: x,y
*/

#include <curses.h>
#include <stdio.h>
#include <stdlib.h> /* for exit() */
#include <signal.h> /* for x windows resize signal */
#include <string.h> /* for strncpy() */
#include "tmgui.h"  /* textmode gui */
#include "bits.h"   /* bitmap editing tools */


/******************************************************************************/
/* status routines */

/* gui_set_status
**	application
**	set the status bar
*/
void gui_set_status( gui_handle * gui, int color, char * text )
{
	gui->sb.color = color;
	strncpy( gui->sb.text, text, 80 );
}


/* gui_status_display
**	internal
**	display the statusbar on the bottom of the screen
*/
void gui_status_display( gui_handle * gui )
{
	int c;
	int len = strlen( gui->sb.text );
	move( gui->h-1, 0 );
	attron( COLOR_PAIR( gui->sb.color ));
	for( c=0 ; c<gui->w ; c++ )
	{
		printw( "%c", (c<len)?gui->sb.text[c] : ' ' );
	}
	attroff( COLOR_PAIR( gui->sb.color ));
}

