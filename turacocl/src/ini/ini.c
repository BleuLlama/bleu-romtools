/* ini.c
 *
 * $Id: ini.c,v 1.11 2003/04/12 21:32:58 jerry Exp $
 */

/******

 This module turns an INI file into more of a DOM (SAX?) model.  You
 can load files in to the internal structure then query them  This only
 currently supports reading in INI files, since that's all we need to
 do at this point.

 It will also return entries into a whitespace-seperated array as
 strings or integers.  ie:
	key = 1 2 3 4 5
	assert( key[2] = "3" )

 ******/

#include <stdio.h>
#include <stdlib.h> /* malloc, free, atoi */
#include <string.h> /* memset, strlen, strdup */
#ifdef SunOS
#include <strings.h> /* strlen, strdup */
#endif
#include "jstr.h"   /* for my string manipulation functions */

#include "misc.h"
#include "ini.h"

/*
 * ini_Destroy
 *
 * clears the internal structure
 */
void 
ini_Destroy( INIStruct * i )
{
    INIStruct * next;

    while( i )
    {
	next = i->next;

	if ( i->group )  free( i->group );
	if ( i->key )    free( i->key );
	if ( i->value )  free( i->value );

	free( i );
	i = next;
    }
}


/*
 * ini_Query
 * 
 *  finds data from the internal strucutre.
 *  the returned data is a char string allocated internally.
 *  do not free it externally!   
 */
char * 
ini_Query( INIStruct * i, char * group, char * key )
{
    INIStruct * curr = i;

    if( !group || !key || !i)
	return( NULL );

    while( curr )
    {
	if (    (!strcasecmp( curr->group, group ))
	     && (!strcasecmp( curr->key, key ))
	    )
	    return( curr->value );

	curr = curr->next;
    }
    return( NULL );
}


/*
 * ini_QueryInt
 *
 *  finds data from the internal strucutre.
 *  the returned data is an int
 */
int
ini_QueryInt( INIStruct * i, char * group, char * key )
{
    char * r = ini_Query( i, group, key );
    if( r )
	return( atoi( r ));
    return( 0 );
}



/*
 * ini_QueryArrayString
 * 
 * finds the n'th element in the array found via an INI query
 *  do not free it externally!   
 */
char *
ini_QueryArrayString( INIStruct * i, char * group, char * key, int index )
{
    char * r = ini_Query( i, group, key );
    if (r)
    {
	return( jstr_ArrayYoink( r, index ) );
    }
    return( NULL );
}


/*
 * ini_QueryArrayInt
 * 
 * finds the n'th element in the array found via an INI query
 *  do not free it externally!   
 */
int 
ini_QueryArrayInt( INIStruct * i, char * group, char * key, int index )
{
    char * r = ini_Query( i, group, key );
    if (r)
    {
	return( atoi( jstr_ArrayYoink( r, index )) );
    }
    return( 0 );
}


/* 
 * ini_Dump
 *
 *  dump out the internal header list. (for debug use)
 */
void 
ini_Dump( INIStruct * i )
{
    INIStruct * curr = i;

    printf("Dump:\n");
    while( curr )
    {
	printf( "  [%s] %s \t->%s\n", curr->group, curr->key, curr->value );

	curr = curr->next;
    }
}



#define BUFSIZE (1024)

/*
 * ini_AddEntry
 *
 *  adds an entry into the ini list
 */
INIStruct * 
ini_AddEntry( INIStruct * i, char * group, char * key, char * value )
{
    INIStruct * n;

    n = (INIStruct *) malloc( sizeof( INIStruct ));
    if( n )
    {
	n->group = strdup( group );
	n->key   = strdup( key );
	n->value = strdup( value );

	n->next = i;
	i = n;
    }
    return( i );
}


/*
** __fpeek
**
**  look at the next character without getting it.... sort of
*/
char __fpeek(FILE * fp)
{
    char c = getc(fp);
    ungetc(c, fp);
    return c;
}



/*
 * ini_ParseFile
 *
 *   will parse in the file "filename" into the internal structures
 */
INIStruct * 
ini_ParseFile( char * filename )
{
    int x;
    char buf[BUFSIZE];
    char * header = NULL;
    char * key    = NULL;
    char * value  = NULL;
    FILE * ifp    = NULL;
    INIStruct * i = NULL;

    if (!filename)
	return( NULL );

    ifp = fopen( filename, "rb" );
    if (!ifp)
	return( NULL );

    while (__fpeek(ifp) != EOF)
    {
	/* clear the buffer*/
	memset( buf, '\0', BUFSIZE );
	/* read in a line*/
	(void) fgets( buf, BUFSIZE, ifp );

	/* check for a # sign in it, set it to a NULL*/
	(void) jstr_NullFirstC( buf, BUFSIZE, '#' );

	/* zot leading and trailing space*/
	(void) jstr_ZotLeadTrailSpace( buf, BUFSIZE );

	/* skip if there's nothing left*/
	if (0 == strlen( buf )) continue;

	/* check for square brackets.  if so, */
	if ( buf[0] == '[' && buf[strlen(buf)-1] == ']' )
	{
	    /*   set first and last character to ' ' */
	    buf[0] = buf[strlen(buf)-1] = ' ';

	    /*   zot leading and trailing space */
	    (void) jstr_ZotLeadTrailSpace( buf, BUFSIZE );

	    /*   that is the header */
	    if (header)  free( header );
	    header = strdup( buf );
	} else {
	    /* find '=' */
	    x = jstr_FindC( buf, BUFSIZE, '=', 0 );

	    /* if not found, go to the next line */
	    if (x == -1) continue;

	    key = value = NULL;

	    /*   char * value = buf[that address+1] */
	    value = buf+x+1;

	    /*   set buf[that address] = '\0' */
	    buf[x] = '\0';
	    
	    /*   char * key = buf[0] */
	    key = buf;

	    /*   zot leading and trailing space (buf) (value) */
	    (void) jstr_ZotLeadTrailSpace( key, BUFSIZE );
	    (void) jstr_ZotLeadTrailSpace( value, BUFSIZE );

	    /*   CB key-value */
	    i = ini_AddEntry( i, header, key, value );
	}

    }
    fclose(ifp);

    /* free any allocated memory */
    if (header)
	free( header );

    return( i );
}
