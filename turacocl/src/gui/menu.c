/* Turaco CL GUI Experiment - textmode gui - menus
**
**	menu.c
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
/* menu routines */


/* gui_menu_cls
**	internal
**	clear the screen underneath the menu display
*/
void gui_menu_cls( int y, int x )
{
	move( y, x );
	clrtoeol();
	move( y+1, x );
	clrtoeol();
}


/* gui_menu_indexOf
**	internal
**	return the array index of a specific id, maxarray on failure
*/
int gui_menu_indexOf( gui_handle * gui, unsigned int id )
{
	int c = 0;
	while( gui->menuitems[c].id != 999 && gui->menuitems[c].id != id ) c++;
	return( c );
}


/* gui_menu_start
**	internal
**	user selected '/', this brings up the menu on the next refresh
*/
void gui_menu_start( gui_handle * gui )
{
	gui->cmenu = gui->cmenuitem = 100;
	gui->show_menu = 1;
}


/* gui_menu_hide
**	internal
**	disables display and interaction with the menu
*/
void gui_menu_hide( gui_handle * gui )
{
	gui_menu_cls( 0, 0 );
	gui->show_menu = 0;
}


/* gui_menu_left
**	internal
**	user pressed the left arrow key
*/
void gui_menu_left( gui_handle * gui )
{
	int x = gui_menu_indexOf( gui, gui->cmenuitem );
	if( x <= 0 ) return;
	if( gui->menuitems[x-1].keystroke != '\0' ) {
		gui->cmenuitem = gui->menuitems[x-1].id;
	}
}


/* gui_menu_right
**	internal
**	user pressed the right arrow key
*/
void gui_menu_right( gui_handle * gui )
{
	int x = gui_menu_indexOf( gui, gui->cmenuitem );
	if( gui->menuitems[x+1].keystroke != '\0' ) {
		gui->cmenuitem = gui->menuitems[x+1].id;
	}
}


/* gui_menu_enter
**	internal
**	user pressed enter or spacebar and the item gets activated
**	a submenu will be displayed or an item put on the todo microstack
*/
void gui_menu_enter( gui_handle * gui )
{
	int x = gui_menu_indexOf( gui, gui->cmenuitem );
	int y;

	if( gui->menuitems[x].submenu ) {
		y = gui_menu_indexOf( gui, gui->menuitems[x].submenu );
		gui->cmenu = gui->cmenuitem = gui->menuitems[y].id;
	} else {
		gui_menu_hide( gui );
		gui->cb( gui, gui->menuitems[x].id );
	}
}


/* gui_menu_kick
**	application
**	lets application code or callbacks bring up a menu-based requestor
*/
void gui_menu_kick( gui_handle * gui, int id )
{
	gui->cmenu = gui->cmenuitem = id;
	gui->show_menu = 1;
}


/******************************************************************************/
/* keystroke handlers */


/* gui_menu_keystroke_check
**	intenral
**	user pressed an arbitrary key
**	search the current menu for a matching keyboard shortcut, do _enter
*/
int gui_menu_keystroke_check( gui_handle * gui, int c )
{
	int x = gui_menu_indexOf( gui, gui->cmenu );

	while( gui->menuitems[x].keystroke != '\0' ) 
	{
		if( gui->menuitems[x].keystroke == c ) {
			gui->cmenuitem = gui->menuitems[x].id;
			gui_menu_enter( gui );
			return( 1 );
		}
		x++;
	}
	return( 0 );
}


/* gui_menu_display
**	internal
**	display the menu to the screen with highlighting, shortcuts, etc
*/
void gui_menu_display( gui_handle * gui,
			int y, int x,  
			int start_item, int highlight_item )
{
	int p = 0;
	int keystroke_trigger = 0;
	int i = gui_menu_indexOf( gui, start_item );

	if( !gui->show_menu ) return;

	gui_menu_cls( y, x );

	move( y, x );
	while( gui->menuitems[i].keystroke != '\0' )
	{
		keystroke_trigger = 0;
		for( p=0 ; gui->menuitems[i].title[p] != '\0' ; p++ )
		{
			if( gui->menuitems[i].id == highlight_item ) {
				attron( COLOR_PAIR(COLOR_SELECTED) );
			} else {
				attron( COLOR_PAIR(COLOR_UNSELECTED) );
			}
			if( gui->menuitems[i].title[p] == '_' ) {
				keystroke_trigger = 1;
			} else {
				if( keystroke_trigger == 1 ) {
					attron( A_UNDERLINE );
					if( gui->menuitems[i].id != highlight_item )
					 attron( COLOR_PAIR(COLOR_KEYSTROKE) );
				}
				printw( "%c", gui->menuitems[i].title[p] );
				if( keystroke_trigger == 1 ) {
					if( gui->menuitems[i].id != highlight_item )
					 attroff( COLOR_PAIR(COLOR_KEYSTROKE) );
					attroff( A_UNDERLINE );
				}
				keystroke_trigger = 0;
			}
			if( gui->menuitems[i].id == highlight_item ) {
				attroff( COLOR_PAIR(COLOR_SELECTED) );
			} else {
				attroff( COLOR_PAIR(COLOR_UNSELECTED) );
			}
		}
		printw( "  " );
		i++;
	}

	i = gui_menu_indexOf( gui, gui->cmenuitem );
	move( y+1, x+3 );
	attron( COLOR_PAIR(COLOR_INFORMATION) );
	printw( "%s", gui->menuitems[i].info  );
	attroff( COLOR_PAIR(COLOR_INFORMATION) );
}


/* gui_keyhandler_menu
**	internal
**	handle all of the menu related key navigation
**	returns 1 if the character was used
*/
int gui_keyhandler_menu( gui_handle * gui, int c )
{
	switch( c )
	{
	case( KEY_BACKSPACE ):
		gui->show_menu = 0;
		break;

	case( KEY_LEFT ):
		gui_menu_left( gui );
		break;

	case( KEY_RIGHT ):
		gui_menu_right( gui );
		break;

	case( KEY_DOWN ):
	case( KEY_ENTER ):
	case( 10 ):
	case( ' ' ):
		gui_menu_enter( gui );
		break;

	default:
		/* fall back to menu shortcut */
		return( gui_menu_keystroke_check( gui, c ) );
	}
	return( 1 );
}
