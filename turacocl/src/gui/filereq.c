/* Turaco CL GUI Experiment - gui - file requestor
**
**	filereq.c
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
#include <stdlib.h> /* for exit() */
#include <signal.h> /* for x windows resize signal */
#include <string.h> /* for strncpy() */
#include <strings.h> 	/* for strcasecmp() for insertion sort */
#include <sys/types.h>
#include <sys/dirent.h>
#include <dirent.h> /* for dirent, etc */
#include <unistd.h>	/* for getcwd */
#include <sys/param.h>	/* MAXPATHLEN */

#include "tmgui.h"  /* textmode gui */
#include "filereq.h"



/* filereq_add
**	creates and adds a new item onto the filereq list passed in
**	does an insertion sort
*/
void filereq_add( gui_filereq * req, struct dirent * de )
{
	filereq_item * t;
	filereq_item * ni;

	/* parameter check */
	if( !req || !de ) return;


	/* check to make sure that "de" is the type of item
	   that we can add, according to req->flags */

	/* dotfiles */
	if( (de->d_name[0] == '.') && !(req->flags & FILEREQ_DOTS ))
		return;

	/* directories */
	if( (de->d_type & DT_DIR) && !(req->flags & FILEREQ_DIRS ))
		return;

	/* files */
	if( (de->d_type & ~DT_DIR) && !(req->flags & FILEREQ_FILES ))
		return;

	/* check the file extension... */
	if( strlen( req->extensions ))
	{
		/*
		** what behaviour do we want here?
		** if the file has no extension, should we accept
		** it or fail it?  For now, I'll accept it.
		*/
		char * ext = strrchr( de->d_name, '.' );

		if( ext )
		{
			char * idx = strcasestr( req->extensions, ext );
			if( !idx ) 
			{
				return;
			}
		}

		/*
		** note; this is not perfect, but it'll work for now.
		** for example. if req->extensions contained ".csv .png"
		** then "filename.c" will be allowed
		*/
	}


	/* allocate space for the new node */
	ni = (filereq_item *)calloc( 1, sizeof( filereq_item ));
	if( !ni ) return;

	/* fill the new node */
	ni->filename = strdup( de->d_name );
	if( de->d_type & DT_DIR ) ni->isdir = 1;

	/* put it on the list in the correct location, alphabetically */
	if( !req->list ) {
		/* no list, start a new one */
		req->list = ni;
		return;
	}

	/* find the right location on the list */
	t = req->list;
	
	while( t ) {
		if(strcasecmp( ni->filename, t->filename ) < 0 )
		{
			ni->next = t;
			if( t->previous ) {
				/* weave it in */
				ni->previous = t->previous;
			} else {
				/* Make it the head of the list */
				req->list = ni;
			}
			t->previous = ni;
			return;
		}
		t=t->next;
	}

	/* append it to the end */
	t = req->list;
	while( t->next ) t=t->next;
	t->next = ni;
	ni->previous = t;
}


/* filereq_add_parent
**	adds the parent entry onto the current list
*/
void filereq_add_parent( gui_filereq * req )
{
	filereq_item * ni = (filereq_item *) calloc( 1, sizeof( filereq_item ));

	if( !ni ) return;

	ni->next = req->list;
	if( req->list ) req->list->previous = ni;
	req->list = ni;

	ni->filename = strdup( ".." );
	ni->isdir = 1;
}


/* filereq_build
**	builds a new filereq_item list from the directory
*/
void filereq_build( gui_filereq * frq )
{
	struct dirent *dp;
	DIR * dirp;

	if( !frq ) return;

	/* assume we've been pre-wiped */
	frq->list = frq->curr = NULL;

	/* attempt to open the directory */
	dirp = opendir( frq->path );
	if( !dirp ) return;

	/* attempt to add each item to the list */
	while( (dp = readdir( dirp )) != NULL )
	{
		filereq_add( frq, dp );
	}

	/* close it down */
	closedir( dirp );

	/* reset the current */
	frq->curr = frq->list;
}


/* filereq_free
**	frees an entire filereq_item list 
*/
void filereq_free( filereq_item * list )
{
	filereq_item * t = list;

	/* just a simple iterator to walk down the list and free everything */
	while( t )
	{
		list = t->next;
		free( t->filename );
		free( t );
		t = list;
	}
}


/* filereq_dump
**	dumps out an entire item list for debugging
*/
void filereq_dump( filereq_item * list )
{
	int i = 0;
	filereq_item * t = list;
	FILE * out = fopen( "/tmp/log.txt", "a" );

	if( !out ) return;

	/* just a simple iterator to walk down the list and dump everything */
	while( t )
	{
		fprintf( out, "%3d: %s%s\n",
				i++, t->filename, 
				(t->isdir)?" (DIR)" : "" );
		t = t->next;
	}
	fclose( out );
}


/* frq_path
**	display the path element at the top of the requestor
*/
static void frq_path( gui_handle * gui, WINDOW * wnd, gui_filereq * frq )
{
	char cwd[MAXPATHLEN];
	char full[MAXPATHLEN];
	
	/* cheat to get the full pathname */
	getcwd( cwd, MAXPATHLEN );
	chdir( frq->path );
	getcwd( full, MAXPATHLEN );
	chdir( cwd );

	/* and store it in frq */
	strcpy( frq->path, full );

	wattron( wnd, COLOR_PAIR( COLOR_KEYSTROKE ));
	wmove( wnd, 1,1 );
	wclrtoeol( wnd );

	mvwprintw( wnd, 1, (gui->w/2)-(strlen( full )/2), " %s ", full );

	wattroff( wnd, COLOR_PAIR( COLOR_KEYSTROKE ));
}


/* frq_buttons
**	display the buttons at the bottom of the requestor
*/
static void frq_buttons( gui_handle * gui, WINDOW * wnd, gui_filereq * frq )
{
	int col;
	wattron( wnd, COLOR_PAIR( COLOR_BORDER_SELECTED ));
	mvwaddch( wnd, gui->h-3, 0, ACS_LTEE );
	for( col=1 ; col<gui->w-1 ; col ++ )
		mvwaddch( wnd, gui->h-3, col, ACS_HLINE );

	mvwaddch( wnd, gui->h-3, gui->w/2, ACS_TTEE );
	mvwaddch( wnd, gui->h-2, gui->w/2, ACS_VLINE );
	mvwaddch( wnd, gui->h-1, gui->w/2, ACS_BTEE );
	wattroff( wnd, COLOR_PAIR( COLOR_BORDER_SELECTED ));

	if( frq->focus == FILEREQ_CANCEL ) 
		wattron( wnd, COLOR_PAIR( COLOR_SELECTED ));
	else
		wattron( wnd, COLOR_PAIR( COLOR_UNSELECTED ));
	mvwprintw( wnd, gui->h-2, (gui->w/4)-(strlen( frq->cancel )/2),
			" %s ", frq->cancel );
	if( frq->focus == FILEREQ_CANCEL ) 
		wattroff( wnd, COLOR_PAIR( COLOR_SELECTED ));
	else
		wattroff( wnd, COLOR_PAIR( COLOR_UNSELECTED ));

	if( frq->focus == FILEREQ_ACCEPT ) 
		wattron( wnd, COLOR_PAIR( COLOR_SELECTED ));
	else
		wattron( wnd, COLOR_PAIR( COLOR_UNSELECTED ));
	mvwprintw( wnd, gui->h-2, (gui->w*3/4)-(strlen( frq->accept )/2),
			" %s ", frq->accept );
	if( frq->focus == FILEREQ_ACCEPT ) 
		wattroff( wnd, COLOR_PAIR( COLOR_SELECTED ));
	else
		wattroff( wnd, COLOR_PAIR( COLOR_UNSELECTED ));

	wattron( wnd, COLOR_PAIR( COLOR_BORDER_SELECTED ));
	mvwaddch( wnd, gui->h-3, gui->w-1, ACS_RTEE );
	wattron( wnd, COLOR_PAIR( COLOR_BORDER_SELECTED ));
}


/* mvwprintw_item
**	display a filereq_item
*/
void mvwprintw_item( WINDOW * wnd, int line, filereq_item *itm, int c )
{
	wmove( wnd, line, 1 );
	wclrtoeol( wnd );
	if( !itm ) return;
	mvwprintw( wnd, line, 1, "   %s%c%s", 
			itm->filename,
			(itm->isdir)? '/' : ' ',
			((itm->filename[0] == '.') && (itm->filename[1] == '.'))?
				" (Parent Directory)":"" );

	if( c ) mvwaddch( wnd, line, 2, ACS_LARROW );
}


/* frq_listing
**	display the file listing section of the file requestor
*/
static void frq_listing( gui_handle * gui, WINDOW * wnd, gui_filereq * frq )
{
	filereq_item * itm;
	int cline = 2+((gui->h-4-1)/2);
	int line;

	wattron( wnd, COLOR_PAIR( COLOR_SELECTED ));
	mvwprintw_item( wnd, cline, frq->curr, 1 );
	wattroff( wnd, COLOR_PAIR( COLOR_SELECTED ));

	wattron( wnd, COLOR_PAIR( COLOR_UNSELECTED ));
	itm = frq->curr->previous;
	for( line = cline-1 ; line > 1 ; line-- )
	{
		mvwprintw_item( wnd, line, itm, 0 );
		if( itm ) itm=itm->previous;
	}

	itm = frq->curr->next;
	for( line = cline+1 ; line < (gui->h-3) ; line++ )
	{
		mvwprintw_item( wnd, line, itm, 0 );
		if( itm ) itm=itm->next;
	}
	wattroff( wnd, COLOR_PAIR( COLOR_UNSELECTED ));
}

/* gui_file_requestor
**	application
**	display a file requestor. - MODAL
**	pass in a pointer to a gui_filereq struct
**	returns 1 on "accept", 0 on "cancel"
*/
int gui_file_requestor( gui_handle * gui, gui_filereq * frq )
{
	int done = 0;
	int c;
	WINDOW * req = NULL;
	
	/* make sure we have the current size */
	getmaxyx( stdscr, gui->h, gui->w );

	/* create our window */
	req = newwin( gui->h, gui->w, 0, 0 );

	/* DEBUG: force the flags for now */
	frq->flags = FILEREQ_DIRS | FILEREQ_FILES;

	/* reset focus to the file listing */
	frq->focus = FILEREQ_LISTING;

	/* scan the directory - kickstart it */
	filereq_build( frq );
	filereq_add_parent( frq );

	/* adjust 'curr' if empty */
	if( frq->curr == NULL ) {
		frq->curr = frq->list;
	}

	if( frq->curr == NULL ) return 0;
	
	/* dump it out */
	/*filereq_dump( frq->list );*/

	while( !done )
	{
		/* first, redraw the screen */
		frq_path( gui, req, frq );
		frq_listing( gui, req, frq );

		gui_colorborder_titled( req, COLOR_BORDER_SELECTED, frq->title, NULL );
		frq_buttons( gui, req, frq );

		wrefresh( req );
		refresh();

		/* next, handle some user input */
		c = getch();
		switch( c )
		{
		case( KEY_UP ):
			if( frq->curr->previous ) 
				frq->curr = frq->curr->previous;
			break;

		case( KEY_DOWN ):
			if( frq->curr->next ) 
				frq->curr = frq->curr->next;
			break;

		case( 9 ): /* tab, change focus */
			frq->focus++;
			if( frq->focus > FILEREQ_MAX ) frq->focus = 0;
			break;

		case( 27 ):
			done = -1;
			break;

		case( ' ' ):
		case( KEY_ENTER ):
		case( 10 ):
		case( 13 ):
			if( frq->focus == FILEREQ_ACCEPT ) {
				done = 1;
			}
			if( frq->focus == FILEREQ_CANCEL ) {
				done = -1;
			}
			if( frq->focus == FILEREQ_LISTING && frq->curr ) {
				if( frq->curr->isdir ) {
					/* setup path */
					strcat( frq->path, "/" );
					strcat( frq->path, frq->curr->filename );
					/* rescan */
					filereq_free( frq->list );
					frq->list = frq->curr = NULL;
					filereq_build( frq );
					filereq_add_parent( frq );

					/* adjust 'curr' if empty */
					if( frq->curr == NULL ) {
						frq->curr = frq->list;
					}
				}
			}
			break;
		}
	}
	
	/* get rid of the window */
	delwin( req );
	clear();
	refresh();

	/* free our ram */
	filereq_free( frq->list );
	frq->list = frq->curr = NULL;

	if( done == -1 ) return( 0 );
	return( 1 );
}


/* gui_file_requestor_p
**	same as above, but takes the new FRQ parameters when called
*/
int gui_file_requestor_p( gui_handle * gui, gui_filereq * frq,
			char * title, char * path,
			char * accept, char * cancel,
			char * ext )
{
	if( !gui || !frq ) return( -1 );

	snprintf( frq->title, LEN_TITLE, "%s", (title)?title:
				(frq->title)?frq->title:"Select file..." );
	snprintf( frq->path, LEN_PATH, "%s", (path)?path: 
				(frq->path)?frq->path:"." );
	snprintf( frq->accept, LEN_BUTTON, "%s", (accept)?accept:
				(frq->accept)?frq->accept:"Accept" );
	snprintf( frq->cancel, LEN_BUTTON, "%s", (cancel)?cancel:
				(frq->cancel)?frq->cancel:"Cancel" );
	snprintf( frq->extensions, LEN_EXT, "%s", (ext)?ext:
				(frq->extensions)?frq->extensions:"" );
	return( gui_file_requestor( gui, frq ) );
}
