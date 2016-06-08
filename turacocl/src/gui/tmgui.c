/* Turaco CL GUI Experiment - textmode gui
**
**	tmgui.c
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
#include "menu.h"   /* gui menu system */
#include "status.h" /* gui status bar */
#include "bits.h"   /* bitmap editing tools */


#define GUI_TEXT_READY	" Ready.   '/' for menu   [tab] for focus"


/******************************************************************************/
/* handle manipulations */


/* gui_new_handle
**	internalish
**	initialize the "gui" structure to have appropriate starting values
*/
gui_handle * gui_new_handle( void )
{
	gui_handle * gui = (gui_handle *)calloc( 1, sizeof( gui_handle ));
	if( !gui ) {
		return( NULL );
	}

	/* starting menu: 100 */
	gui->cmenu = 100;
	gui->cmenuitem = 100;
	gui->show_menu = 0;
	gui->quit = 0;

	/* initial screen size */
	getmaxyx( stdscr, gui->h, gui->w );

	/* initial status */
	gui->sb.color = COLOR_STATUS_GOOD;
	gui->sb.text[0] = '\0';

	/* and user mode */
	gui->user_mode = USER_MODE_IDLE;

	return( gui );
}


/* gui_delete_handle
**	internalish
**	free the passed-in gui handle
**	it will be invalid after this call
*/
void gui_delete_handle( gui_handle * gui )
{
	if( gui ) {
		if( gui->pal.win )    delwin( gui->pal.win );
		if( gui->bited.win )  delwin( gui->bited.win );
		if( gui->second.win ) delwin( gui->second.win );
		
		bits_free( gui->main );
		bits_free( gui->swap );
		bits_free( gui->undo );
		bits_free( gui->temp );
		
		free( gui );
	}
}


/******************************************************************************/
/* screen helpers */


/* gui_colorborder
**	application
**	draw a border in the specified color
*/
void gui_colorborder( WINDOW * w, int color )
{
	if( !w ) return;

	wborder( w,
		COLOR_PAIR( color ) | ACS_VLINE,
		COLOR_PAIR( color ) | ACS_VLINE,
		COLOR_PAIR( color ) | ACS_HLINE,
		COLOR_PAIR( color ) | ACS_HLINE,
		COLOR_PAIR( color ) | ACS_ULCORNER,
		COLOR_PAIR( color ) | ACS_URCORNER,
		COLOR_PAIR( color ) | ACS_LLCORNER,
		COLOR_PAIR( color ) | ACS_LRCORNER );
}


/* gui_colorborder_titled
**	application
**	draw a titled colorborder
*/
void gui_colorborder_titled( WINDOW * w, int color, char * title, char * legend )
{
	int cols = getmaxx( w );
	int rows = getmaxy( w );
	int tlen_top = title?strlen(title):0;
	int tlen_bot = legend?strlen(legend):0;

	/* first, draw the regular border */
	gui_colorborder( w, color );

	/* display the title, if we have one */
	if( tlen_top )
	{
		/* change to the color, and plop it down, centered */
		/* if thise doesn't look acceptable, 
			replace ACS_RTEE, LTEE with [ ] */
		wattron( w, COLOR_PAIR( color ));
		mvwaddch( w, 0, cols/2-(tlen_top/2)-2, ACS_RTEE );
		mvwprintw( w, 0, cols/2-(tlen_top/2)-1, " %s ", title );
		mvwaddch( w, 0, cols/2+(tlen_top/2)+1, ACS_LTEE );
		wattroff( w, COLOR_PAIR( color ));
	}

	/* draw the legend if we can */
	if( tlen_bot )
	{
		wattron( w, COLOR_PAIR( color ));
		mvwaddch( w, rows-1, cols/2-(tlen_bot/2)-2, ACS_RTEE );
		mvwprintw( w, rows-1, cols/2-(tlen_bot/2)-1, " %s ", legend );
		mvwaddch( w, rows-1, cols/2+(tlen_bot/2)+1, ACS_LTEE );
		wattroff( w, COLOR_PAIR( color ));
	} 
}


/******************************************************************************/
/* screen display */


/* gui_palette_display
**	internal
**	display the palette editor/widget, and update it appropriately
*/
void gui_palette_display( gui_handle * gui )
{
	int c;
	int focus = (gui->user_mode == USER_MODE_PALETTE)?1:0;
	int bc = (focus) ? COLOR_BORDER_SELECTED : COLOR_BORDER;

	/* draw the outline first */
	gui_colorborder( gui->pal.win, bc );

	/* draw out the palette colors */
	for( c=0 ; c<gui->pal.num ; c++ )
	{
		wattron( gui->pal.win, COLOR_PAIR(gui->pal.colors[c].local) );
		wmove( gui->pal.win, c+1, 1 );
		wprintw( gui->pal.win, "%2x", c );
		wattroff( gui->pal.win, COLOR_PAIR(gui->pal.colors[c].local) );
	}

	/* and draw the pointer or cursor position */
	mvwaddch( gui->pal.win, gui->pal.cursor+1, 0, 
		(focus? ACS_RARROW:'[') | COLOR_PAIR( COLOR_BORDER_SELECTED ));
	mvwaddch( gui->pal.win, gui->pal.cursor+1, 3, 
		(focus? ACS_LARROW:']') | COLOR_PAIR( COLOR_BORDER_SELECTED ));
/*
	mvwaddch( gui->pal.win, gui->pal.cursor+1, 0, 
		(focus? ACS_RARROW:ACS_LTEE) | COLOR_PAIR( bc ));
	mvwaddch( gui->pal.win, gui->pal.cursor+1, 3, 
		(focus? ACS_LARROW:ACS_RTEE) | COLOR_PAIR( bc ));
*/

	/* finally, the up/down pointers */
	if( focus )
	{
		/* up */
		mvwaddch( gui->pal.win, 0, 1, ACS_UARROW | COLOR_PAIR( bc ));
		mvwaddch( gui->pal.win, 0, 2, ACS_UARROW | COLOR_PAIR( bc ));

		/* down */
		mvwaddch( gui->pal.win, gui->pal.num+1, 1,
				'V' | COLOR_PAIR( bc ));
		mvwaddch( gui->pal.win, gui->pal.num+1, 2,
				'V' | COLOR_PAIR( bc ));
	}

	/* and refresh the output */
	wrefresh( gui->pal.win );
}


#define BITSW_FLAGS_NONE	(0x00)
#define BITSW_FLAGS_GRID	(0x01) /* display grid */
#define BITSW_FLAGS_NUMBERS	(0x02) /* display numbers */
#define BITSW_FLAGS_CURSOR	(0x04) /* display cursor */

/* gui_bitsw
**	display a bits array to a window
*/
void gui_bitsw( WINDOW * wnd, int xp, int yp,
		bits * bmp, palette_editor * pal,
		int flags, int cx, int cy )
{
	int x,y,c,ch;

	if( !bmp || !pal || !wnd ) return;

	/* now the bitmap content */
	for( y=0 ; y<bmp->h ; y++ )
	{
		/* we're assuming characters are twice tall than wide */
		wmove( wnd, y+yp, xp );

		for( x=0 ; x<bmp->w ; x++ )
		{
			if( flags & BITSW_FLAGS_NUMBERS )
			{
				/* display the palette numbers */

				/* get the color */
				c = bits_getp( bmp, x, y );

				/* set our gui color */
				wattron( wnd, COLOR_PAIR(pal->colors[c].local));

				/* make it bold if it's the cursor */
				if( (flags & BITSW_FLAGS_CURSOR )
				    && (x == cx) && (y == cy) )
					wattron( wnd, A_BOLD );

				/* display the number */
				wprintw( wnd, "%2x", c );

				/* restore attributes */
				if( (flags & BITSW_FLAGS_CURSOR )
				    && (x == cx) && (y == cy) )
					wattroff( wnd, A_BOLD );
				wattroff(wnd, COLOR_PAIR(pal->colors[c].local));
			} else {
				/* if "grid" is on, put dots in each spot. */
				if( flags & BITSW_FLAGS_GRID )
					ch = ':';
				else
					ch = ' ';

				/* something special for cursor pos */
				/* if grid is on, do the opposite */
				if( (flags & BITSW_FLAGS_CURSOR )
				    && (x == cx) && (y == cy) )
				{
					if( flags & BITSW_FLAGS_GRID )
						ch = ' ';
					else
						ch = ':';
				}

				/* get the color from the main bitmap */
				c = bits_getp( bmp, x, y );

				/* and now draw out the uberpixel */
				waddch( wnd,
				    ch | COLOR_PAIR(pal->colors[c].local) );
				waddch( wnd,
				    ch | COLOR_PAIR(pal->colors[c].local) );
			}
		}
	}
}


/* gui_second_display
**	internal
**	display the second window
*/
void gui_second_display( gui_handle * gui )
{
	char *title = "Unknown";
	char legend[32];

	legend[0] = '\0';

	switch( gui->second.which ) {
	case( SECOND_SWAP ): 
		title = "Swap";
		break;
	case( SECOND_UNDO ): 
		title = "Undo";
		break;
	case( SECOND_BANK ):
		title = "Bank";
		snprintf( legend, 32, "B %02x  S %02x",
			gui->second.bank, gui->second.sprite );
		break;
	}

	/* render the border */
	gui_colorborder_titled( gui->second.win,
		(gui->user_mode == USER_MODE_SECOND)?
		    COLOR_BORDER_SELECTED : COLOR_BORDER,
		title, legend );

	/* draw it out */
	gui_bitsw( gui->second.win, 1, 1, 
		(gui->second.which == SECOND_SWAP)? gui->swap : 
		(gui->second.which == SECOND_UNDO)? gui->undo :
		NULL,
		&gui->pal, 0, 0, 0 );

	/* and refresh it to the screen */
	wrefresh( gui->second.win );
}


/* gui_text_display
**	internal
**	display various texty bits
*/
void gui_text_display( gui_handle * gui )
{
	/*
	attron( COLOR_PAIR( COLOR_BORDER ));
	mvprintw( 2, (gui->w/2) -(gui->main->w)-6, "Main" );
	mvprintw( 2, (gui->w/2) +(gui->main->w)+2, "Swap" );
	attroff( COLOR_PAIR( COLOR_BORDER ));
	*/
}


/* gui_bited_display
**	internal
**	display the bitmap editor/widget, and update it appropriately
*/
void gui_bited_display( gui_handle * gui )
{
	char legend[32];

	int focus = (gui->user_mode == USER_MODE_BITED)?1:0;
	int bc = (focus) ? COLOR_BORDER_SELECTED : COLOR_BORDER;

	/* render the bitmap */
	gui_bitsw( gui->bited.win, 1, 1, gui->main, &gui->pal,
		  ( (gui->bited.grid) ? BITSW_FLAGS_GRID    : 0 )
		| ( (gui->bited.num)  ? BITSW_FLAGS_NUMBERS : 0 )
		| ( (focus)           ? BITSW_FLAGS_CURSOR  : 0 ),
		gui->bited.cx, gui->bited.cy );

	/* render the border */
	snprintf( legend, 32, "B %02x  S %02x",
			gui->bited.bank, gui->bited.sprite );

	gui_colorborder_titled ( gui->bited.win, 
			(gui->user_mode == USER_MODE_BITED)?
			COLOR_BORDER_SELECTED : COLOR_BORDER,
			"Editor", legend );


	/* if we have focus, draw cursor diamond dots on the border */
	if( focus )
	{
		/* column indicator */
		wmove( gui->bited.win, 0, 1+(2*gui->bited.cx) );
		waddch( gui->bited.win, ACS_DIAMOND | COLOR_PAIR( bc ));
		waddch( gui->bited.win, ACS_DIAMOND | COLOR_PAIR( bc ));

		wmove( gui->bited.win, gui->main->h+1, 1+(2*gui->bited.cx) );
		waddch( gui->bited.win, ACS_DIAMOND | COLOR_PAIR( bc ));
		waddch( gui->bited.win, ACS_DIAMOND | COLOR_PAIR( bc ));

		/* row indicator */
		wmove( gui->bited.win, 1+gui->bited.cy, 0 );
		waddch( gui->bited.win, ACS_DIAMOND | COLOR_PAIR( bc ));

		wmove( gui->bited.win, 1+gui->bited.cy, (gui->main->w*2)+1 );
		waddch( gui->bited.win, ACS_DIAMOND | COLOR_PAIR( bc ));
	}


	/* and refresh it to the screen */
	wrefresh( gui->bited.win );
}


/* gui_cursor_hide
**	internal
**	a quick hack in an attempt to hide the cursor; move it to bottom right
*/
void gui_cursor_hide( gui_handle * gui )
{
	move( gui->h-1, gui->w-1 );
}


/* gui_redraw
**	internal
**	redraw all screen elements, where appropriate
*/
void gui_redraw( gui_handle * gui )
{
	gui_menu_display( gui, 0, 0, gui->cmenu, gui->cmenuitem );
	gui_status_display( gui );
	gui_palette_display( gui );
	gui_bited_display( gui );
	gui_second_display( gui );

	gui_text_display( gui );

	gui_cursor_hide( gui );
	refresh();
}

 
/******************************************************************************/
/* signal handlers */


/* gui_handle_window_resize
**	internal
**	this gets called on an X11 terminal resize, hopefully
*/
#ifdef SIGWINCH
void gui_handle_window_resize( int sig )
{
	if( sig == SIGWINCH ) {
/*
		getmaxyx( stdscr, gui->h, gui->w );
		gui_redraw( gui );
*/
		refresh();
	}
}
#endif



/******************************************************************************/
/* application level interactions - core functionality */


/* gui_startup
**	application
**	start up the gui/Curses interface
**	the menu structure array MUST be passed in
**	the menu callback MUST also be passed in
**	returns 1 on succes, 0 on failure
*/
gui_handle * gui_startup( gui_menuitem * mi, gui_menu_fcn cb )
{
	gui_handle * gui = NULL;

	/* check to make sure we have menu items... */
	if( !mi )
	{
		fprintf( stderr, "No menu items.  That's kinda important.\n" );
		return( NULL );
	}

	/* start up the curses system, and make sure we can map colors */
	initscr();
	if( has_colors() == FALSE )
	{
		endwin();
		fprintf( stderr, "No color support in terminal.  ZING!\n" );
		return( NULL );
	}
/*
	if( can_change_color() == FALSE )
	{
		endwin();
		fprintf( stderr, "Unable to remap colors.  D'oh!\n" );
		return( NULL );
	}
*/

	/* first, alloc and set the initial values in the structure */
	gui = gui_new_handle();
	if( !gui ) {
		endwin();
		fprintf( stderr, "Memory allocation issue.  Bummer.\n" );
		return( NULL );
	}
	gui->menuitems = mi;
	gui->cb = cb;

	/* and initialize the various settings in the curses system */
	raw();
	keypad( stdscr, TRUE );
	noecho();
	cbreak();
	curs_set( 0 );		/* turn off the cursor */
	start_color();
	timeout( 1000 );
#ifdef SIGWINCH
	signal( SIGWINCH, &gui_handle_window_resize );
#endif

	/* these are colors we can draw from...
	COLOR_BLACK	COLOR_RED	COLOR_GREEN	COLOR_YELLOW
	COLOR_BLUE	COLOR_MAGENTA	COLOR_CYAN	COLOR_WHITE
	*/

	/* set up our color palette */
	/*         COLOR_id_code		Foreground	Background */
	/* gui colors */
	init_pair( COLOR_UNSELECTED,		COLOR_WHITE,	COLOR_BLACK );
	init_pair( COLOR_KEYSTROKE,		COLOR_YELLOW,	COLOR_BLACK );
	init_pair( COLOR_SELECTED,		COLOR_BLACK,	COLOR_CYAN );
	init_pair( COLOR_INFORMATION,		COLOR_CYAN,	COLOR_BLACK );

	/* gui window colors */
	init_pair( COLOR_BORDER,		COLOR_BLUE,	COLOR_BLACK );
	init_pair( COLOR_BORDER_SELECTED,	COLOR_CYAN,	COLOR_BLACK );

	/* status colors */
	init_pair( COLOR_STATUS_GOOD,    	COLOR_BLACK,	COLOR_GREEN );
	init_pair( COLOR_STATUS_WARNING, 	COLOR_BLACK,	COLOR_YELLOW );
	init_pair( COLOR_STATUS_BAD,     	COLOR_BLACK,	COLOR_RED );

	/* bitmap palette colors */
	init_pair( COLOR_PAL_BLACK,		COLOR_WHITE,	COLOR_BLACK );
	init_pair( COLOR_PAL_RED,		COLOR_BLACK,	COLOR_RED );
	init_pair( COLOR_PAL_GREEN,		COLOR_BLACK,	COLOR_GREEN );
	init_pair( COLOR_PAL_YELLOW,		COLOR_BLACK,	COLOR_YELLOW );
	init_pair( COLOR_PAL_BLUE,		COLOR_WHITE,	COLOR_BLUE );
	init_pair( COLOR_PAL_MAGENTA,		COLOR_BLACK,	COLOR_MAGENTA );
	init_pair( COLOR_PAL_CYAN,		COLOR_BLACK,	COLOR_CYAN );
	init_pair( COLOR_PAL_WHITE,		COLOR_BLACK,	COLOR_WHITE );

	/* display something sane for the status */
	gui_set_status( gui, COLOR_STATUS_GOOD, GUI_TEXT_READY );

	/* and... we're outta here */
	return( gui );
}


/* gui_initialize_palette
**	application
**	(re)setup the palette data
*/
void gui_initialize_palette( gui_handle * gui, int ncolors )
{
	int c;

	/* set up the control variables */
	gui->pal.num = ncolors;
	gui->pal.cursor = 0;

	/* reallocate the window structure */
	if( gui->pal.win ) delwin( gui->pal.win );
	gui->pal.win = newwin( gui->pal.num+2, 2+2,   3, gui->w/2-2  );

	/* populate it */
	for( c=0 ; c<gui->pal.num ; c++ )
	{
		gui->pal.colors[c].rom = c;
		gui->pal.colors[c].local = COLOR_PAL_FIRST + (c&0x07);
	}
}


/* gui_initialize_bited
**	application
**	(re)setup the bitmap editor
*/
void gui_initialize_bited( gui_handle * gui, int w, int h )
{
	/* reallocate the window structure */
	if( gui->bited.win ) delwin( gui->bited.win );

	/* allocate ram for the sprite buffers */
	gui->main = bits_resize( gui->main, w, h );
	gui->swap = bits_resize( gui->swap, w, h );
	gui->undo = bits_resize( gui->undo, w, h );
	gui->temp = bits_resize( gui->temp, w, h );

	/* curses buffer */
	gui->bited.win = newwin( h+2, (w*2)+2, 3, (gui->w/2) - 3 - (w*2) - 2 );

	/* positional */
	gui->bited.cx = gui->bited.cy = 0;

	/* populate it - for now... */
	bits_fill( gui->main );
	bits_fill( gui->swap );
	bits_fill( gui->undo );

	/* sprite stuff */
	gui->bited.bank = 0;
	gui->bited.sprite = 0;
}


/* gui_initialize_second
**	setup the secondary display
*/
void gui_initialize_second( gui_handle * gui, int w, int h )
{
	/* reallocate the window structure */
	if( gui->second.win ) delwin( gui->second.win );

	/* curses buffer */
	gui->second.win = newwin( h+2, (w*2)+2, 3, (gui->w/2) + 3 );

	/* various navigational settings */
	gui->second.which = 0;
	gui->second.bank = 0;
	gui->second.sprite = 0;
}


/* gui_keyhandler_palette
**	internal
**	handle all of the palette related key navigation
*/
int gui_keyhandler_palette( gui_handle * gui, int c )
{
	int used = 0;

	switch( c )
	{
	case( KEY_UP ):
		gui->pal.cursor--;
		if( gui->pal.cursor<0 ) gui->pal.cursor = gui->pal.num-1;
		used = 1;
		break;

	case( KEY_DOWN ):
		gui->pal.cursor++;
		if( gui->pal.cursor>=gui->pal.num ) gui->pal.cursor = 0;
		used = 1;
		break;

	case( KEY_LEFT ):
		gui->pal.colors[gui->pal.cursor].local--;
		if( gui->pal.colors[gui->pal.cursor].local < COLOR_PAL_FIRST )
			gui->pal.colors[gui->pal.cursor].local = COLOR_PAL_LAST;
		used = 1;
		break;

	case( KEY_RIGHT ):
		gui->pal.colors[gui->pal.cursor].local++;
		if( gui->pal.colors[gui->pal.cursor].local > COLOR_PAL_LAST )
			gui->pal.colors[gui->pal.cursor].local = COLOR_PAL_FIRST;
		used = 1;
		break;
	}
/*
	if( used ) gui_redraw( gui );
*/

	return( used );
}


/* gui_keyhandler_bited
**	internal
**	handle all of the bitmap editor navigation stuff
*/
int gui_keyhandler_bited( gui_handle * gui, int c )
{
	int used = 0;
	int ch = -1;
	int shift = 1;

	/* check for the regular palette keys */
	if( c>='0' && c<='9' ) ch = c-'0';
	if( c>='a' && c<='z' ) ch = c-'a'+0x0a;
	
	if( ch != -1 ) shift = 0;
	

	/* check for shifted palette keys */
	/* these will select the color without putting it on the canvas */
	if( c==')' ) ch=0;
	if( c=='!' ) ch=1;
	if( c=='@' ) ch=2;
	if( c=='#' ) ch=3;
	if( c=='$' ) ch=4;
	if( c=='%' ) ch=5;
	if( c=='^' ) ch=6;
	if( c=='&' ) ch=7;
	if( c=='*' ) ch=8;
	if( c=='(' ) ch=9;

	if( (ch != -1) && (ch < gui->pal.num) )
	{
		bits_undo_checkpoint( gui );
		gui->pal.cursor = ch;
		if( !shift )
		    bits_setp( gui->main, gui->bited.cx, gui->bited.cy, ch );
		used = 1;
		return( used );
	}

	switch( c )
	{
	/* display related things */
	case( 'g' ): gui->bited.grid = ~gui->bited.grid; break;
	case( 'n' ): gui->bited.num = ~gui->bited.num; break;

	/* bitmap modifiers */
	case( 'x' ): bits_flip_horiz( gui ); break;
	case( 'y' ): bits_flip_vert( gui ); break;
	case( 'm' ): bits_mirror_horiz( gui ); break;
	case( 'M' ): bits_mirror_vert( gui ); break;
	case( 'z' ): bits_rotate_90( gui ); break;

	/* extra-bitmap image changes */
	case( 'u' ): bits_swap_undo( gui ); break;
	case( 'j' ): bits_swap( gui ); break;
	case( 'J' ): bits_copy_to_swap( gui ); break;

	/* image sliding */
	case( 'a' ): bits_shift( gui, -1, 0 ); break;
	case( 'd' ): bits_shift( gui,  1, 0 ); break;
	case( 'w' ): bits_shift( gui, 0, -1 ); break;
	case( 's' ): bits_shift( gui, 0,  1 ); break;

	/* stupid stuff */
	case( 'c' ): bits_conway( gui ); break;

	/* fill */
	case( 'K' ): bits_clear( gui ); break;
	case( 'S' ): bits_swap_colors( gui ); break;
	case( 'F' ): bits_fill_all( gui ); break;

	/* single pixel */
	case( ',' ): /* get */
		gui->pal.cursor = bits_getp( gui->main, 
				gui->bited.cx, gui->bited.cy );
		used = 1;
		break;

	case( ' ' ): /* put */
		bits_undo_checkpoint( gui );
		bits_setp( gui->main, gui->bited.cx, gui->bited.cy,
				gui->pal.cursor );
		used = 1;
		break;

	/* positioning */
	case( KEY_UP ):
		gui->bited.cy--;
		if( gui->bited.cy < 0 ) gui->bited.cy=gui->main->h-1;
		used = 1;
		break;
		
	case( KEY_DOWN ):
		gui->bited.cy++;
		if( gui->bited.cy >= gui->main->h ) gui->bited.cy=0;
		used = 1;
		break;
		
	case( KEY_LEFT ):
		gui->bited.cx--;
		if( gui->bited.cx < 0 ) gui->bited.cx=gui->main->w-1;
		used = 1;
		break;
		
	case( KEY_RIGHT ):
		gui->bited.cx++;
		if( gui->bited.cx >= gui->main->w ) gui->bited.cx=0;
		used = 1;
		break;
	}

/*
	if( used ) gui_redraw( gui );
*/
	return( used );
}


/* gui_keyhandler_second
**	internal
**	handle all of the second display input
*/
int gui_keyhandler_second( gui_handle * gui, int c )
{
	int used = 0;

	if( gui->second.which == SECOND_BANK ) { 
		switch( c ) {
		case( '0' ):		/* chr = 0  */
			gui->second.sprite = 0; used = 1; break;

		case( KEY_UP ): 	/* chr -= 8 */
			gui->second.sprite -= 8; used = 1; break;

		case( KEY_DOWN ):	/* chr += 8 */
			gui->second.sprite += 8; used = 1; break;

		case( KEY_LEFT ):	/* chr -- */
			gui->second.sprite--; used = 1; break;

		case( KEY_RIGHT ):	/* chr ++ */
			gui->second.sprite++; used = 1; break;

		case( '[' ):		/* previous same-sized bank */
			gui->second.bank -= 4; used = 1; break;

		case( ']' ):		/* next same-sized bank */
			gui->second.bank += 4; used = 1; break;

		case( '{' ):		/* previous bank */
			gui->second.bank--; used = 1; break;

		case( '}' ):		/* next bank */
			gui->second.bank++; used = 1; break;

		case( 'g' ):		/* get this sprite to the editor */ 
		case( 'p' ):		/* put editor sprite here */ 
			break;
		}

		/* XXX some common bounds checking */
		if( gui->second.sprite < 0 )   gui->second.sprite = 0;
		if( gui->second.sprite > 255 ) gui->second.sprite = 255;

		if( gui->second.bank < 0 ) gui->second.bank = 0;
		if( gui->second.bank >15 ) gui->second.bank = 15; /* for now */
	}

	switch( c ) {
	case( ' ' ):	/* spacebar advances to the next display */
		gui->second.which++;
		if( gui->second.which>SECOND_MAX ) gui->second.which = 0;
		used = 1;
		break;
/*
		gui->second.which--;
		if( gui->second.which<0 ) gui->second.which = SECOND_MAX;
		used = 1;
		break;
*/
	}

	return( used );
}


/* gui_keyhandler_main
**	internal
**	handle the main mode switching and triggering
**	returns 1 if the character (c) was used
*/
int gui_keyhandler_main( gui_handle * gui, int c )
{
	switch( c )
	{
	case( 9 ): /* tab */
		/* change focus */
		gui_menu_hide( gui );
		gui->user_mode++;
		if( gui->user_mode > USER_MODE_MAX ) {
			gui->user_mode = 0;
		}

		switch( gui->user_mode ) {
		case( USER_MODE_IDLE ):
			gui_set_status( gui, COLOR_STATUS_GOOD,
				GUI_TEXT_READY );
			break;
		case( USER_MODE_PALETTE ):
			gui_set_status( gui, COLOR_STATUS_GOOD,
				"Palette editor" );
			break;
		case( USER_MODE_BITED ):
			gui_set_status( gui, COLOR_STATUS_GOOD,
				"Bitmap editor" );
			break;
		}
		return( 1 );
		break;

	case( '/' ):
	case( ':' ):
		/* bring up the menu */
		gui_menu_start( gui );
		return( 1 );
		break;

	case( 27 ): /* escape */
		/* exit out of the menu */
		gui_menu_hide( gui );
		return( 1 );
		break;

		
	}
	return( 0 );
}


/* gui_runloop
**	application
**	this is the main loop that checks for user input and does stuff with it
*/
void gui_runloop( gui_handle * gui )
{
	int c;
	int used;

	while( !gui->quit )
	{
		/* first, redraw the screen */
		gui_redraw( gui );

		/* next, handle some user input */
		c = getch();
		used = 0;

		/* check for global usage */
		used = gui_keyhandler_main( gui, c );

		/* if not, check for subfunction/transient usage */
		if( !used && gui->show_menu ) 
		{
			/* these need to be mutually exclusive so that
			   we eat all keys while the menu is up, and don't
			   accidently take a dump on the bitmap or palette
			*/
			used = gui_keyhandler_menu( gui, c );
		} else {
			/* distribute the keystroke to the right handler */
			if( !used && (gui->user_mode == USER_MODE_BITED) )
				used = gui_keyhandler_bited( gui, c );
			if( !used && (gui->user_mode == USER_MODE_PALETTE) )
				used = gui_keyhandler_palette( gui, c );
			if( !used && (gui->user_mode == USER_MODE_SECOND) )
				used = gui_keyhandler_second( gui, c );
		}
	}
}


/* gui_shutdown
**	application
**	called when we are ending the gui session - does not trigger shutdown
**	after this is called, the handle passed in is invalid
*/
void gui_shutdown( gui_handle * gui )
{
	gui_delete_handle( gui );
	endwin();
}
