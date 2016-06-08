/*
 * io_util.c
 *
 *  my additional IO functions
 *
 *  $Id: io_util.c,v 1.2 2003/03/11 19:32:34 jerry Exp $
 *
 *  $Log: io_util.c,v $
 *  Revision 1.2  2003/03/11 19:32:34  jerry
 *  Changed my_whitespace() to use #defines.  not that it's used, but whatever.
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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "io_util.h"
 

/*
** fpeek
**
**  look at the next character without getting it.... sort of
*/
char fpeek(FILE * fp)
{
    char c = getc(fp);
    ungetc(c, fp);
    return c;
}


/*
** my_getc
**
**  there was a reason for this. damned if i can remember why.
*/
int my_getc(FILE * fp)
{
    int c=getc(fp);
    return (c);
}


/*
** my_whitespace
**
**  returns IOU_SPACE on is white space:
**    spaces, \t \n \r ( ) [ ]
**  returns IOU_EOF on EOF
**  returns IOU_NONSPACE on non-whitespace
*/
int my_whitespace(int c)
{
    switch (c)
    {
        case('\n'):
        case(' '):
        case('\t'): case('\r'):
        case('('): case(')'):
        case('['): case(']'):
            return( IOU_SPACE );
        case(EOF):
            return( IOU_EOF );
        case('#'):
            return( IOU_POUND );
        case('{'): case('}'):
            return( IOU_CURLYBRACE );
        default:
            return( IOU_NONSPACE );
    }
}


/*
**  dump_to_eol
**
**   go to the character after the next \n, \r, or EOF
*/
void dump_to_eol(FILE * fp)
{
    int c=my_getc(fp);

    while (c != '\r' && c != '\n')
        c=my_getc(fp);

    if (my_whitespace(c) != IOU_EOF)
        ungetc(c, fp);
}


/*
** read_char
**
**  read the next char from fp, skipping whitespace
*/
int read_char(FILE * fp)
{
    int c;

    while (1)
    {
        c = my_getc(fp);
        switch(c)
        {
            case -1:
                return -1;
            case '\n':
            case '\f':
            case '\r':
            case '\t':
            case ' ':
                /* um... white space... skip it */
                break;
            case '#':
                /* comment... read till end of line */
                dump_to_eol(fp);
                break;
            default:
                return (int) c;
        }
    }
    return(0);
}


/*
** read_int
**
**  read the next number (negative or non) from fp, skipping whitespace
*/
int read_int(FILE * fp)
{
    int c = read_char(fp);
    int number = 0;

    while (1)
    {
        if (c == '-')
            return -1 * read_int(fp);  /* Recursion! */
        else if (c >= '0' && c <= '9')
        {
            number = (number * 10) + (c - '0');
            c = my_getc(fp);
        } else
            return number;
    }
}


/*
** read_string
**
**  read the next string in from fp, returning it through *string
*/
char * read_string(FILE * fp)
{
    int c = read_char(fp);
    int len = 0;
    char *newstr;
    char temp[100];

    do
    {
        temp[len++] = c ;
    }
    while ((c=my_getc(fp)) != EOF && !isspace(c));

    temp[len] = '\0';

    newstr = (char *) malloc (len+1);
    strcpy(newstr, temp);

    return (newstr);
}


/*
** read_quoted
**
**  read the next quoted string in from fp, returning it through *string
*/
char * read_quoted(FILE * fp)
{
    int c;
    int len = 0;
    char *newstr;
    char temp[100];

    while ((c = read_char(fp)) != '\"');

    while ((c = read_char(fp)) != '\"')  temp[len++] = c;
    temp[len] = '\0';

    newstr = (char *) malloc (len+1);
    strcpy(newstr, temp);

    return (newstr);
}

