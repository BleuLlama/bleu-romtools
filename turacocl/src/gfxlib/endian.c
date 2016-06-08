/*
 *  endian.c
 *
 *   little and big endian file io routines 
 *
 * $Id: endian.c,v 1.1 2003/04/02 21:24:56 jerry Exp $
 *
 * $Log: endian.c,v $
 * Revision 1.1  2003/04/02 21:24:56  jerry
 * Added endian.[ch] for use with the PCX encoding and decoding.
 *
 *
 */

#include <stdio.h>
#include "endian.h"

/*
 *  routines to read and write to and from little and big endian 
 *  binary blob files from any machine.
 *
 *   The value "480"  is:
 *		0x01 0xe0	on big endian machines (network order)
 *		0xe0 0x01	on little endian machines
 *
 *      Big endian:  68k, PPC, Sparc	IFF, PNG, network
 *   Little endian:  x86, Mips		PCX
 */



/************************************************************/
/* BIG ENDIAN READ */


/* endian_BigRead8
 *
 *   read in a byte (8 bits), Big endian style
 */
int endian_BigRead8(
	FILE * fp
)
{
    return( fgetc( fp ));
}


/* endian_BigRead16
 *
 *   read in a word (2 bytes, 16 bits), Big endian style
 */
int endian_BigRead16(
	FILE * fp
)
{
    int a,b,z;

    a = endian_BigRead8(fp) & 0x00ff;
    b = endian_BigRead8(fp) & 0x00ff;

    z = (a<<8) + (b);

    return( z );

}


/* endian_BigRead32
 *
 *   read in a long (4 bytes, 32 bits), Big endian style
 */
long endian_BigRead32(
	FILE * fp
)
{
    int a,b;
    long z;

    a = endian_BigRead16(fp) & 0x00ffff;
    b = endian_BigRead16(fp) & 0x00ffff;

    z = (a<<16) + (b);

    return( z );
}



/************************************************************/
/* BIG ENDIAN WRITE */


/* endian_BigWrite8
 *
 *   write out a byte (8 bits), Big endian style
 */
void endian_BigWrite8(
	int data,
	FILE * fp
)
{
    fputc( data, fp );
}


/* endian_BigWrite16
 *
 *   write out a word (2 bytes, 16 bits), Big endian style
 */
void endian_BigWrite16(
	int data,
	FILE * fp
)
{
    endian_BigWrite8( (data >> 8) & 0x00ff, fp );
    endian_BigWrite8( (data     ) & 0x00ff, fp );
}


/* endian_BigWrite32
 *
 *   write out a long (4 bytes, 32 bits), Big endian style
 */
void endian_BigWrite32(
	long data,
	FILE * fp
)
{
    endian_BigWrite16( (data >> 16) & 0x0000ffff, fp );
    endian_BigWrite16( (data      ) & 0x0000ffff, fp );
}


 
/************************************************************/
/* LITTLE ENDIAN READ */


/* endian_LittleRead8
 *
 *   read in a byte (8 bits), Little endian style
 */
int endian_LittleRead8(
	FILE * fp
)
{
    return( fgetc( fp ));
}


/* endian_LittleRead16
 *
 *   read in a word (2 bytes, 16 bits), Little endian style
 */
int endian_LittleRead16(
	FILE * fp
)
{
    int a,b,z;

    a = endian_LittleRead8(fp) & 0x00ff;
    b = endian_LittleRead8(fp) & 0x00ff;

    z = (b<<8) + (a);

    return( z );

}


/* endian_LittleRead32
 *
 *   read in a long (4 bytes, 32 bits), Little endian style
 */
long endian_LittleRead32(
	FILE * fp
)
{
    int a,b;
    long z;

    a = endian_BigRead16(fp) & 0x00ffff;
    b = endian_BigRead16(fp) & 0x00ffff;

    z = (b<<16) + (a);

    return( z );
}



/************************************************************/
/* LITTLE ENDIAN WRITE */


/* endian_LittleWrite8
 *
 *   write out a byte (8 bits), Little endian style
 */
void endian_LittleWrite8(
	int data,
	FILE * fp
)
{
    fputc( data, fp );
}


/* endian_LittleWrite16
 *
 *   write out a word (2 bytes, 16 bits), Little endian style
 */
void endian_LittleWrite16(
	int data,
	FILE * fp
)
{
    endian_LittleWrite8( (data     ) & 0x00ff, fp );
    endian_LittleWrite8( (data >> 8) & 0x00ff, fp );
}


/* endian_LittleWrite32
 *
 *   write out a long (4 bytes, 32 bits), Little endian style
 */
void endian_LittleWrite32(
	long data, 
	FILE * fp
)
{
    endian_LittleWrite16( (data      ) & 0x0000ffff, fp );
    endian_LittleWrite16( (data >> 16) & 0x0000ffff, fp );
}
