 /*
 * io_util.h
 *
 *  my additional IO functions
 *
 *  $Id: io_util.h,v 1.1.1.1 2002/07/08 20:42:31 jerry Exp $
 *
 *  $Log: io_util.h,v $
 *  Revision 1.1.1.1  2002/07/08 20:42:31  jerry
 *  Initial Directory
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
**  returns 1 on is white space:
**    spaces, \t \n \r ( ) [ ]
**  returns 2 on EOF
**  0 on non-whitespace
*/
int my_whitespace(int c);


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
