/*
 * turaco.h
 *
 * $Id: turaco.h,v 1.15 2003/05/29 21:21:33 jerry Exp $
 */

#include "driver.h"
#include "params.h"

#ifndef __TURACOH__
#define __TURACOH__

#define FN_MAX  (512)

/* TuracoInstance structure
 *
 *  When a game's graphics are loaded in or to be saved out, we use
 *  this structure to hold all of the related information
 */
typedef struct
{
    UserParams * up;    /* the user's selected parameters */
    GameDriver * gd;    /* the driver for the game */

    int paletteNumber;  /* which palette we should use */
    int bankNumber;     /* which bank number it is */

    IMAGE * loadedGraphics;     /* the image loaded in */
    IMAGE * colorKey;           /* the color key for the image */

    int romstart;
    char * romBuffer;   /* internal buffer which simulates the rom image */
    int romBufferSize;  /* the size of said rom buffer */

    char fn_bank[ FN_MAX ];  /* bank data filename */
    char fn_key[ FN_MAX ];   /* color key filename */
    char fn_check[ FN_MAX ]; /* checkerboard filename */

} TuracoInstance;


/* turaco_Create
 * 
 *  set up an initial instance
 */
TuracoInstance *
turaco_Create( void );


/* turaco_Destroy
 * 
 *  get rid of an instance
 */
void
turaco_Destroy( TuracoInstance * ti );


/* turaco_LoadDriver
 * 
 *  load the driver selected by the userParams
 *  returns [[0]] on success
 */
int
turaco_LoadDriver( TuracoInstance * ti );


/* turaco_VerifyDriverParams
 *
 *  checks the validity of driver-based parameters (range checking, etc)
 */
int
turaco_VerifyDriver( TuracoInstance * ti );



/* turaco_ConfigureFilenames
 *
 *  sets up all of the internal filename strings apropriately
 */
int
turaco_ConfigureFilenames( TuracoInstance * ti );



/* turaco_LoadImages
 *
 *  loads the Image data as selected in the user params 
 */
int
turaco_LoadImages( TuracoInstance * ti );


/* turaco_OverlayTilemapImages
 *
 *  loads the proper tilemaps and decodes them into the ROM buffer
 */
void turaco_OverlayTilemapImages( TuracoInstance * ti );



/* turaco_SaveImages
 *
 *  saves out the internal images
 */
int
turaco_SaveImages( TuracoInstance * ti );



/* turaco_GenerateColorKeyFromDriver
 *
 *  generates the color key image from the driver parameters
 */
int
turaco_GenerateColorKeyFromDriver( TuracoInstance * ti );



/* turaco_EncodeRom
 *
 * takes in a [gfxDecode], and Converts the [image] into the [romBuffer]
 *
 * Range checking is assumed to have happened before this, therefore
 * there is no return value.
 *
 * NOTE: This loads the image data into the alpha channel of the IMAGE.
 *       It needs to be converted externally to save it properly
 */
void
turaco_EncodeRom(
	TuracoInstance * ti,
        GfxDecode * gfxDecode,
        IMAGE * image,
        char * romBuffer,
        long bufsize );


/* turaco_EncodeRomBufferFromImage
 *
 *  generates the internal RomBuffer by encoding the internal IMAGE 
 */
int
turaco_EncodeRomBufferFromImage( TuracoInstance * ti );



/* turaco_DecodeRom
 *
 * takes in a [gfxDecode], and fabricates an [image] from the [romBuffer]
 *
 * Range checking is assumed to have happened before this, therefore
 * there is no return value.
 *
 * NOTE: This looks for the image data in the alpha channel of the IMAGE.
 *       It needs to be converted externally to load it properly
 */
void
turaco_DecodeRom(
	TuracoInstance * ti,
        GfxDecode * gfxDecode,
        IMAGE * image,
        char * romBuffer,
        long bufsize );



/* turaco_DecodeImageFromRomBuffer
 *
 *  generates the internal IMAGE by decoding the RomBuffer
 */
int
turaco_DecodeImageFromRomBuffer( TuracoInstance * ti );


#endif
