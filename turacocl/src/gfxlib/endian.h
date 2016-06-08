/*
 *  endian.h
 *
 *   little and big endian file io routines 
 *
 * $Id: endian.h,v 1.1 2003/04/02 21:24:56 jerry Exp $
 *
 * $Log: endian.h,v $
 * Revision 1.1  2003/04/02 21:24:56  jerry
 * Added endian.[ch] for use with the PCX encoding and decoding.
 *
 *
 */

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


#include <stdio.h> /* for "FILE" */
 

/************************************************************/
/* BIG ENDIAN READ */


/* endian_BigRead8
 *
 *   read in a byte (8 bits), Big endian style
 */
int endian_BigRead8( FILE * fp );


/* endian_BigRead16
 *
 *   read in a word (2 bytes, 16 bits), Big endian style
 */
int endian_BigRead16( FILE * fp );


/* endian_BigRead32
 *
 *   read in a long (4 bytes, 32 bits), Big endian style
 */
long endian_BigRead32( FILE * fp );



/************************************************************/
/* BIG ENDIAN WRITE */


/* endian_BigWrite8
 *
 *   write out a byte (8 bits), Big endian style
 */
void endian_BigWrite8( int data, FILE * fp );


/* endian_BigWrite16
 *
 *   write out a word (2 bytes, 16 bits), Big endian style
 */
void endian_BigWrite16( int data, FILE * fp );


/* endian_BigWrite32
 *
 *   write out a long (4 bytes, 32 bits), Big endian style
 */
void endian_BigWrite32( long data, FILE * fp );


 
/************************************************************/
/* LITTLE ENDIAN READ */


/* endian_LittleRead8
 *
 *   read in a byte (8 bits), Little endian style
 */
int endian_LittleRead8( FILE * fp );


/* endian_LittleRead16
 *
 *   read in a word (2 bytes, 16 bits), Little endian style
 */
int endian_LittleRead16( FILE * fp );


/* endian_LittleRead32
 *
 *   read in a long (4 bytes, 32 bits), Little endian style
 */
long endian_LittleRead32( FILE * fp );



/************************************************************/
/* LITTLE ENDIAN WRITE */


/* endian_LittleWrite8
 *
 *   write out a byte (8 bits), Little endian style
 */
void endian_LittleWrite8( int data, FILE * fp );


/* endian_LittleWrite16
 *
 *   write out a word (2 bytes, 16 bits), Little endian style
 */
void endian_LittleWrite16( int data, FILE * fp );


/* endian_LittleWrite32
 *
 *   write out a long (4 bytes, 32 bits), Little endian style
 */
void endian_LittleWrite32( long data, FILE * fp );
