/*
	Typedefs for various models and compiler sizes, written by Mark Incley.
*/

#ifndef __MITYPES__
#define	__MITYPES__


#define	FALSE	0
#define	TRUE	1

/* Maximum length of a filename (inc. path, filename and ext.) */

#ifndef _MAX_PATH
#define	_MAX_PATH	64			/* Max length of filename, inc. path. */
#endif

/* 16-bit DOS (Using Borland C++) */

#ifdef	__SMALL__

#define	HUGE	huge

typedef	unsigned char	BYTE;
typedef	unsigned int   	WORD;
typedef	unsigned long	DWORD;


/* 32-bit DOS (Using DJGPP) */

#else

#define	HUGE

typedef	unsigned char	BYTE;
typedef	unsigned short 	WORD;
typedef	unsigned long	DWORD;


#endif


#endif		/* __MITYPES__ */
