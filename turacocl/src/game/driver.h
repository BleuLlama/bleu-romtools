/*
 * driver.h
 *
 * $Id: driver.h,v 1.12 2003/05/15 19:50:33 jerry Exp $
 */

#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "image.h" /* for PIXEL */
#include "params.h"

/* for internal use */
#define MAXSTRING (512)


/* RomFileDesc structure
 * 
 *  describes a ROM image file. 
 */
typedef struct
{
    int offset;
    int size;
    char filename[MAXSTRING];
    int needed;
} RomFileDesc;



/* GfxDecode structure
 *
 *  describes how the graphics data is stored in the rom image file
 */
typedef struct
{
    int startaddress;   /* offset into the rom for start of the bank */
    int width;          /* pixels wide */
    int height;         /* pixels tall */
    int nsprites;       /* number of sprites in the bank */
    int orientation;	/* how the images are stored in the rom image */

    int planes;             /* num of bitplanes */
    /* these are all in bits, not bytes */
    int planeoffsets[64];   /* start of every bitplane */
    int xoffset[MAXSTRING]; /*|coords of the bit corresponding to the pixel */
    int yoffset[MAXSTRING]; /*|  of the given coordinates */
    int charincrement;      /* bits between two consecutive characters */
} GfxDecode;



/* orientation value #defines
 * 
 * these are used in XXXXX
 * 
 * (Originally taken from mame src\driver.h
 */
#define ORIENTATION_DEFAULT  (0x00)	/* no rotations */
#define ORIENTATION_FLIP_X   (0x01)	/* mirror in the X direction */
#define ORIENTATION_FLIP_Y   (0x02)	/* mirror in the Y direction */

#define ORIENTATION_SWAP_XY  (0x04)	/* mirror along the NW/SE diagonal */

#define ORIENTATION_ROTATE_90   (ORIENTATION_SWAP_XY|ORIENTATION_FLIP_X)
					/* rotate CW 90 degrees */
#define ORIENTATION_ROTATE_180  (ORIENTATION_FLIP_X|ORIENTATION_FLIP_Y)
					/* rotate 180 degrees */
#define ORIENTATION_ROTATE_270  (ORIENTATION_SWAP_XY|ORIENTATION_FLIP_Y)
					/* rotate CCW 90 degrees */



/* steal the 'PIXEL' from ppm/image.h and use it as our color element */
#define ColorElement PIXEL



/* GamePalette
 *
 *  palette for our games.
 */
typedef struct
{
    int ncolors;
    ColorElement p[256];
} GamePalette;


/* TileMap
 *
 *  tilempas for decode layouts
 */
typedef struct
{
    IMAGE * decoded;

    char fname[MAXSTRING];
    char name[MAXSTRING];
    char credit[MAXSTRING];
    char description[MAXSTRING];
    int bank;
    int height;
    int width;
    int ** row;
} TileMap;


/* TileSet
 *
 *  sets of tilemaps to be loaded/saved
 */
typedef struct
{
    char name[MAXSTRING];
    int setnos[MAXSTRING];
} TileSet;


/* GameDriver structure
 *
 *  The complete driver structure.  This gets filled in by the driver loader
 */
typedef struct
{
    /* from command line */
    char * driverName;

    /* from ini file */
    char * description;

    /* roms */
    int nroms;
    RomFileDesc * romFileDescs;  /* dynamic array */

    /* graphics */
    int ndecodes;
    GfxDecode * gfxDecodes;      /* dynamic array */

    /* palettes */
    int npalettes;
    GamePalette * gamePalettes;  /* dynamic array */

    /* tilemaps */
    int ntilemaps;
    TileMap * tileMaps; 	/* dynamic array */
    int nsets;
    TileSet * tileSets;		/* dynamic array */
} GameDriver;



/******************************************************************************/

/*
 * game_FindDriver
 *
 *   This recurses down $(DRIVERS) to find the driver named "drivername".
 *   it puts the proper filename and path down in the buffer 'buf'
 *   if it was not found, buf[0] = NULL;
 */
char *
game_FindDriver( UserParams * userParams, char * buf, int bufsize );


/* game_ExtractName
 *
 *  extracts out the game name from the passed in filepath
 *   ie:  /users/jerry/driver.ini  becomes  driver
 *        XXXXXXXXXXXX/driver.XXX  becomes  driver
 *        driver.XXX  becomes  driver              
 */
char *
game_ExtractName( char * inputText );


/* game_LoadDriver
 *
 *  loads in the driver defined with [[filename]].  You may need to find
 *   the complete path first using the above method.
 */
GameDriver *
game_LoadDriver( UserParams * userParams, char * filename );


/* game_DestroyDriver
 *
 *  frees memory taken up by a driver
 */
void
game_DestroyDriver( GameDriver * gd );


/* game_Dump
 *
 * dump out the game driver information (debug use)
 */
void game_Dump( GameDriver * gd );


#endif
