/* Turaco CL GUI Experiment - textmode gui
**
**	tmgui.h
**
**  2008-02-08+  Scott Lawrence
**  http://umlautllama.com
*/

/* This code is considered to be freeware.  Do with it as you wish.
** I claim no responsibility for anything bad that might be a direct
** or indirect result of the usage of this code.
*/

#ifndef __TMGUI_H__
#define __TMGUI_H__

/* COLOR_xxxx
**	These are colors used by the GUI 
*/
#define COLOR_UNSELECTED	(1)
#define COLOR_KEYSTROKE		(2)
#define COLOR_SELECTED		(3)
#define COLOR_INFORMATION	(4)
#define COLOR_BORDER		(5)
#define COLOR_BORDER_SELECTED	(6)

/* these colors are used for status */
#define COLOR_STATUS_GOOD	(7)
#define COLOR_STATUS_WARNING	(8)
#define COLOR_STATUS_BAD	(9)

/* and these are for editing */
#define COLOR_PAL_FIRST		(10)
#define COLOR_PAL_BLACK		(10)
#define COLOR_PAL_RED		(11)
#define COLOR_PAL_GREEN		(12)
#define COLOR_PAL_YELLOW	(13)
#define COLOR_PAL_BLUE		(14)
#define COLOR_PAL_MAGENTA	(15)
#define COLOR_PAL_CYAN		(16)
#define COLOR_PAL_WHITE		(17)
#define COLOR_PAL_LAST		(17)


/* gui_pal_entry
**	these are used to map a rom color to a COLOR_PAL local entry
*/
typedef struct gui_pal_entry {
	int rom;
	int local;
} gui_pal_entry;

#define GUI_MAX_COLORS	(32)	/* number of LUT entries allowed */


/* status_bar
**	container for status bar information
*/
typedef struct status_bar {
	int color;
	char text[80];
} status_bar;


/* palette_editor
**	container for palette editor information
*/
typedef struct palette_editor {
	WINDOW * win;
	int num;	/* number of palette entries */
	int cursor; 	/* where the cursor currently is */
	gui_pal_entry colors[ GUI_MAX_COLORS ]; /* the entries */
} palette_editor;


/* bits
**	encompass an entire bitmap
*/
typedef struct bits {
	int w;		/* current bitmap width */
	int h;		/* current bitmap height */
	int * p;	/* bitmap pixels */
} bits;


/* bitmap_editor
**	container for bitmap editor information
*/
typedef struct bitmap_editor {
	WINDOW * win;
	int cx;		/* current x cursor position */
	int cy;		/* current y cursor position */
	int grid;	/* display a grid */
	int num;	/* display the palette numbers */

	/* for sprite editing */
	int bank;
	int sprite;
} bitmap_editor;


/* second_display
**	container for the second display
*/
typedef struct second_display {
	WINDOW * win;
	int which;	/* which buffer to display */

	/* for the sprite selector */
	int bank; 	/* current bank number */
	int sprite;	/* current sprite number */
} second_display;

#define SECOND_SWAP	(0)	/* display the swap bitmap */
#define SECOND_UNDO	(1)	/* display the undo bitmap */
#define SECOND_BANK	(2)	/* display the bank selector */
#define SECOND_MAX	(2)	/* for the keyboard handler */


/* perhaps abstract it out to this */
typedef struct gui_window {
	WINDOW * win;	/* the curses window to display to */
	int x, y;	/* thing's position on screen */
	int w, h;	/* data's width and height */
	int flags;	/* GUI flags */
} gui_window;

#define GUI_FLAGS_NONE	(0x00)	/* thing is normal */
#define GUI_FLAGS_FOCUS	(0x01)	/* thing has focus */
#define GUI_FLAGS_GRID	(0x02)	/* display a grid (where appropriate) */
#define GUI_FLAGS_HIDE 	(0x03)	/* thing should be hidden */


/* gui_menuitem
**	the menu structure (below) is a static array of these items
*/
typedef struct gui_menuitem {
	unsigned int id;	/* pseudo-unique identfier */
	unsigned int submenu;	/* CMD for a command, number for a submenu */
	char keystroke;		/* keystroke equivalent for this item */
	char * title;		/* title text _underscore is for the next char*/
	char * info;		/* informative text to appear underneath */
} gui_menuitem;


#define CMD 	(0)		/* the item is not a submenu, but a leaf node*/
#define END	0, '\0'		/* the item is the last item in a grouping */


/* predeclare the structure below */
typedef struct gui_handle_struct gui_handle;

#define USER_MODE_IDLE		(0)	/* idle mode */
#define USER_MODE_BITED		(1)	/* editing the bitmap */
#define USER_MODE_PALETTE	(2)	/* editing the palette */
#define USER_MODE_SECOND	(3)	/* changing the second display */
#define USER_MODE_MAX		(3)	/* needed for tabbing */

/* now create the typedef for the function */
typedef void (*gui_menu_fcn)( gui_handle * gui, int id );

/* gui_handle
**	this is the global state structure which maintains all of the 
**	runtime information about the menus and various screen
**	maintenance data for the entire gui system
*/
struct gui_handle_struct {
	/* general GUI */
	int quit;
	int w;
	int h;
	int show_menu;
	int user_mode;

	/* and some bitmaps */
	bits * main;	/* the main bitmap */
	bits * undo;	/* one level of undo for now */
	bits * swap;	/* the spare page */
	bits * temp;	/* the temp buffer for bits operations */

	/* menu */
	int cmenu;
	int cmenuitem;
	gui_menuitem *menuitems;
	gui_menu_fcn cb;

	/* gui elements */
	status_bar sb;		/* the status bar */
	palette_editor pal;	/* the palette editor */
	bitmap_editor bited;	/* the bitmap editor */
	second_display second;	/* the second display */
};



/* gui_set_status
**	application
**	set the status bar
*/
void gui_set_status( gui_handle * gui, int color, char * text );


/* gui_startup
**	application
**	start up the gui/Curses interface
**	the menu structure array MUST be passed in
**	returns 1 on succes, 0 on failure
*/
gui_handle * gui_startup( gui_menuitem * mi, gui_menu_fcn cb );


/* gui_initialize_palette
**      application
**      (re)setup the palette data
*/
void gui_initialize_palette( gui_handle * gui, int ncolors );


/* gui_initialize_bited
**      application
**      (re)setup the bitmap editor
*/
void gui_initialize_bited( gui_handle * gui, int w, int h );


/* gui_initialize_second
**      application
**      (re)setup the second display
*/
void gui_initialize_second( gui_handle * gui, int w, int h );


/* gui_menu_kick
**	application
**	lets application code or callbacks bring up a menu-based requestor
*/
void gui_menu_kick( gui_handle * gui, int id );


/* gui_runloop
**	application
**	this is the main loop that checks for user input and does stuff with it
*/
void gui_runloop( gui_handle * gui );


/* gui_shutdown
**	application
**	called when we are ending the gui session - does not trigger shutdown
**	after this is called, the handle passed in is invalid
*/
void gui_shutdown( gui_handle * gui );


/* gui_popup_requestor
**	application
**	display a popup menu selector thingy (Modal)
**	returns the item number selected [0..N] or -1 if cancelled
**	use the arrow keys to select the item
**	space/return to select, escape to cancel
*/
int gui_popup_requestor( gui_handle * gui, char * info, char ** items, 
			int allowCancel );


/* gui_colorborder
**	application
**	draw a border in the specified color
*/
void gui_colorborder( WINDOW * w, int color );


/* gui_colorborder_titled
**	application
**	draw a titled colorborder
*/
void gui_colorborder_titled( WINDOW * w, int color, char * top, char * bottom );

#endif
