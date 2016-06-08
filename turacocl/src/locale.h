/*
 *	locale.h
 *
 *	locale lookup guides
 */

/* locale_SetCode
**	set the current two-character language code
*/
void locale_SetCode( char * code );

/* locale_
**	return the correct string (or the key if not found)
*/
char * locale_( char * key );
