/* Turaco CL GUI Experiment - gui - file requestor
**
**	filereq.h
**
**  2008-02-08+  Scott Lawrence
**  http://umlautllama.com
*/

/* This code is considered to be freeware.  Do with it as you wish.
** I claim no responsibility for anything bad that might be a direct
** or indirect result of the usage of this code.
*/


#ifndef __FILEREQ_H__
#define __FILEREQ_H__

#include <sys/types.h>
#include <sys/dirent.h>
#include <dirent.h> /* for dirent, etc */
#include "tmgui.h"  /* textmode gui */


/* filereq_item
**	an item in a directory listing 
*/
typedef struct filereq_item
{
	char * filename;
	int isdir;
	struct filereq_item * next;
	struct filereq_item * previous;
} filereq_item;


#define LEN_TITLE	(80)
#define LEN_PATH	(512)
#define LEN_BUTTON	(32)
#define LEN_EXT		(32)

/* struct gui_filereq
**	encompasses all parameters and persistant values for a file requestor
*/
typedef struct gui_filereq
{
	char title[LEN_TITLE];	  /* title */
	char path[LEN_PATH];	  /* start path */
	char accept[LEN_BUTTON];  /* "Open", "Save As" etc... */
	char cancel[LEN_BUTTON];  /* "Cancel", "Forget it" etc... */
	char extensions[LEN_EXT]; /* ".pcx .png" file exts, "" for any */
	int flags;
	int focus;

	struct filereq_item * list;
	struct filereq_item * curr;
} gui_filereq;


/* the flags:  (these determine what gets added to the list) */
#define FILEREQ_DIRS	(0x01)	/* allow directory selection */
#define FILEREQ_FILES	(0x02)	/* allow file selection */
#define FILEREQ_DOTS	(0x04)	/* show dot files */

/* gui focus - internal use */
#define FILEREQ_LISTING	(0x00)
#define FILEREQ_ACCEPT  (0x01)
#define FILEREQ_CANCEL	(0x02)
#define FILEREQ_MAX	(0x02)


/* filereq_add
**	creates and adds a new item onto the filereq list passed in
*/
void filereq_add( gui_filereq * req, struct dirent * de );


/* filereq_build
**	builds a new filereq_item list from the directory
*/
void filereq_build( gui_filereq * req );


/* filereq_free
**	frees an entire filereq_item list 
*/
void filereq_free( filereq_item * list );


/* filereq_dump
**	dumps out an entire item list for debugging
*/
void filereq_dump( filereq_item * list );


/* gui_file_requestor
**	application
**	display a file requestor.
**	pass in a pointer to a gui_filereq struct
**	returns 1 on "accept", 0 on "failure"
*/
int gui_file_requestor( gui_handle * gui, gui_filereq * frq );


/* gui_file_requestor_p
**	same as above, but takes the new FRQ parameters when called
*/
int gui_file_requestor_p( gui_handle * gui, gui_filereq * frq,
			char * title, char * path,
			char * accept, char * cancel,
			char * ext );

#endif
