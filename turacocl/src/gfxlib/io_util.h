 /*
 * io_util.h
 *
 *  my additional IO functions
 *
 *  $Id: io_util.h,v 1.3 2003/03/11 19:32:34 jerry Exp $
 *
 *  $Log: io_util.h,v $
 *  Revision 1.3  2003/03/11 19:32:34  jerry
 *  Changed my_whitespace() to use #defines.  not that it's used, but whatever.
 *
 *  Revision 1.2  2003/03/10 21:00:04  jerry
 *  Changed INI to pass around a pointer to the list
 *  moved the parser into game.c (new)
 *  updated the makefile to add game.c
 *
 *  Revision 1.1  2003/03/07 20:23:43  jerry
 *  Brought over the PPM library to use here for loading and saving images.
 *  We'll also use the PPM image structure for our images.
 *
 * Revision 1.2  1999/12/23  22:24:18  sdlpci
 * added bits to the structure
 * implemented interpolation/etrapolation routine
 * pulled out some functions to io_util.[ch]
 *
 * Revision 1.1  1999/12/23  19:21:47  sdlpci
 * Initial revision
 *
 */

/* NOTE: All of this code was ripped from my MS-DOS PLFI project, 
 *       dated 1996 January 01 (Most of which in-turn was taken from
 *       my Flatware 3-D Object editor on my Amiga. Whee!
 */

#include <stdio.h>
 

/*
** fpeek
**
**  look at the next character without getting it.... sort of
*/
char fpeek(FILE * fp);


/*
** my_getc
**
**  there was a reason for this. damned if i can remember why.
*/
int my_getc(FILE * fp);


/*
** my_whitespace
**
**  returns IOU_SPACE on is white space:
**    spaces, \t \n \r ( ) [ ]
**  returns IOU_EOF on EOF
**  returns IOU_NONSPACE on non-whitespace
**  returns IOU_POUND on #
**  returns IOU_CURLYBRACE on { or }
*/
int my_whitespace(int c);

#define IOU_NONSPACE	(0)
#define IOU_SPACE	(1)
#define IOU_EOF		(2)
#define IOU_POUND	(3)
#define IOU_CURLYBRACE	(4)


/*
**  dump_to_eol
**
**   go to the character after the next \n, \r, or EOF
*/
void dump_to_eol(FILE * fp);


/*
** read_char
**
**  read the next char from fp, skipping whitespace
*/
int read_char(FILE * fp);


/*
** read_int
**
**  read the next number (negative or non) from fp, skipping whitespace
*/
int read_int(FILE * fp);


/*
** read_string
**
**  read the next string in from fp, returning it through *string
*/
char * read_string(FILE * fp);


/*
** read_quoted
**
**  read the next quoted string in from fp, returning it through *string
*/
char * read_quoted(FILE * fp);
