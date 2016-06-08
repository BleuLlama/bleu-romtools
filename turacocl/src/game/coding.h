/*
 * coding.h
 *
 * $Id: coding.h,v 1.7 2003/05/15 02:43:26 jerry Exp $
 */
#ifndef __CODING_H__
#define __CODING_H__

#include "driver.h"
#include "gfxlib.h"

/* coding_EncodeSprite
 *
 * takes in a [gfxDecode], and Converts sprite number 
 * [spriteNumber] from the [image] into the [romBuffer]
 *
 * NOTE: This loads the image data into the alpha channel of the IMAGE.
 *       It needs to be converted externally to save it properly
 */
void
coding_EncodeSprite(
	GfxDecode * gfxDecode,	int spriteNumber,
	IMAGE * image,		int xpos, int ypos,
	char * romBuffer,	long bufsize );


/* coding_DecodeSprite
 *
 * takes in a [gfxDecode], and fabricates an [image] from the 
 * sprite specified as [spriteNumber] from the [romBuffer]
 *
 * Range checking is assumed to have happened before this, therefore
 * there is no return value.
 *
 * NOTE: This looks for the image data in the alpha channel of the IMAGE.
 *       It needs to be converted externally to load it properly
 */
void
coding_DecodeSprite( 
	GfxDecode * gfxDecode,	int spriteNumber,
	IMAGE * image, 		int xpos, int ypos,
	char * romBuffer,	long bufsize );
 

/* codingGenHerringbone
 *
 *  generates a sprite-sized herringbone at (xpos,ypos) in image
 */
void
coding_GenHerringbone( GfxDecode * gfxDecode,	
			IMAGE * image,
			int xpos, int ypos);

#endif
