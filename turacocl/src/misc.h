/*
 * misc.h
 *
 *  various helpful #defines and macros.
 *
 * $Id: misc.h,v 1.2 2003/04/11 21:27:45 jerry Exp $
 */

/* MACROS */

/* for the max brightness detector for the palette scaler */
#define MAX( A, B )    ( (A) > (B) ) ? (A) : (B)
#define MIN( A, B )    ( (A) > (B) ) ? (B) : (A)


/* OS-Specific stuff */

#ifdef MSDOS
    /* *** MS-DOS *** */

/* directory slashes are backslashes in MS-DOS */
#define DIRSLASH  "\\"
#define DIRSLASHC '\\'

#else
    /* *** _NIX *** */

/* directory slashes are correct */
#define DIRSLASH  "/"
#define DIRSLASHC '/'

#endif



