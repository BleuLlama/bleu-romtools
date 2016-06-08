/* Turaco CL GUI Experiment - textmode gui - status bar
**
**	status.h
**
**  2008-02-08+  Scott Lawrence
**  http://umlautllama.com
*/

/* This code is considered to be freeware.  Do with it as you wish.
** I claim no responsibility for anything bad that might be a direct
** or indirect result of the usage of this code.
*/


#ifndef __STATUS_H__
#define __STATUS_H__

/******************************************************************************/
/* status routines */

/* gui_set_status
**	application
**	set the status bar
*/
void gui_set_status( gui_handle * gui, int color, char * text );


/* gui_status_display
**	internal
**	display the statusbar on the bottom of the screen
*/
void gui_status_display( gui_handle * gui );

#endif
