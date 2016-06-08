/* jstr.h
 *
 * $Id: jstr.h,v 1.1 2003/03/10 06:37:23 jerry Exp $
 */


/*
 * jstr_NullFirstC
 *
 *  nulls out the first occurrance of 'c'
 */
char * jstr_NullFirstC( char * buf, int max, char c );

/* 
 * jstr_ZotLeadTrailSpace
 *
 *  converts something like "   hello there   " into "hello there"
 */
char * jstr_ZotLeadTrailSpace( char * buf, int max );

/*
 * jstr_FindC
 *
 *  finds the first occurrance of 'C' after index 'start'
 */
int jstr_FindC( char * buf, int max, char c, int start );

/*
 * jstr_ArrayYoink
 *
 *  gets the [n]th element from the string passed in.
 *  string returned should not be free'd.  if the index was invalid,
 *  it returns a NULL.
 */
char * jstr_ArrayYoink( char * buf, int index );
