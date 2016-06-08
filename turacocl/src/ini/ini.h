/* ini.h
 *
 * $Id: ini.h,v 1.4 2003/03/11 05:17:13 jerry Exp $
 */

typedef struct inistruct{
    char * group;               /* group name */
    char * key;                 /* the key name. */
    char * value;               /* the value */
    struct inistruct * next;    /* the next one in the list */
} INIStruct;


/*
 * ini_Destroy
 *
 * clears the internal structure
 */
void
ini_Destroy( INIStruct * i );


/*
 * ini_ParseFile
 *
 *   will parse in the file "filename" into the internal structures
 */
INIStruct *
ini_ParseFile( char * filename );


/*
 * ini_Query
 * 
 *  finds data from the internal strucutre.
 *  the returned data is a char string allocated internally.       
 *  do not free it externally!
 */
char *
ini_Query( INIStruct * i, char * group, char * key );


/*
 * ini_QueryInt
 * 
 *  finds data from the internal strucutre.
 *  the returned data is an int.
 */
int
ini_QueryInt( INIStruct * i, char * group, char * key );


/*
 * ini_QueryArrayString
 *
 * finds the n'th element in the array found via an INI query
 *  do not free it externally!
 */
char *
ini_QueryArrayString( INIStruct * i, char * group, char * key, int index );



/*
 * ini_QueryArrayInt
 *
 * finds the n'th element in the array found via an INI query
 *  do not free it externally!
 */
int
ini_QueryArrayInt( INIStruct * i, char * group, char * key, int index );


/*
 * ini_Dump
 *
 *  dump out the internal header list. (for debug use )
 */
void ini_Dump( INIStruct * i );
