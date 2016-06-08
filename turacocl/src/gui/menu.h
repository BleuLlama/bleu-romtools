#ifndef __MENU_H__
#define __MENU_H__

/******************************************************************************/
/* menu routines */


/* gui_menu_cls
**	internal
**	clear the screen underneath the menu display
*/
void gui_menu_cls( int y, int x );


/* gui_menu_indexOf
**	internal
**	return the array index of a specific id, maxarray on failure
*/
int gui_menu_indexOf( gui_handle * gui, unsigned int id );


/* gui_menu_start
**	internal
**	user selected '/', this brings up the menu on the next refresh
*/
void gui_menu_start( gui_handle * gui );


/* gui_menu_hide
**	internal
**	disables display and interaction with the menu
*/
void gui_menu_hide( gui_handle * gui );


/* gui_menu_enter
**	internal
**	user pressed enter or spacebar and the item gets activated
**	a submenu will be displayed or an item put on the todo microstack
*/
void gui_menu_enter( gui_handle * gui );


/* gui_menu_kick
**	application
**	lets application code or callbacks bring up a menu-based requestor
*/
void gui_menu_kick( gui_handle * gui, int id );


/******************************************************************************/
/* keystroke handlers */


/* gui_menu_keystroke_check
**	intenral
**	user pressed an arbitrary key
**	search the current menu for a matching keyboard shortcut, do _enter
*/
int gui_menu_keystroke_check( gui_handle * gui, int c );


/* gui_menu_display
**	internal
**	display the menu to the screen with highlighting, shortcuts, etc
*/
void gui_menu_display( gui_handle * gui,
			int y, int x,  
			int start_item, int highlight_item );


/* gui_keyhandler_menu
**	internal
**	handle all of the menu related key navigation
**	returns 1 if the character was used
*/
int gui_keyhandler_menu( gui_handle * gui, int c );

#endif
