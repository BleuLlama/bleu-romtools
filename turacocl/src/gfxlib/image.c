/*
 *  image.c
 *
 *   simple image functions
 *
 * $Id: image.c,v 1.6 2003/04/03 19:46:28 jerry Exp $
 *
 * $Log: image.c,v $
 * Revision 1.6  2003/04/03 19:46:28  jerry
 * 8bit (paletted) and 24bit (truecolor/multiplanar) image loading complete
 *
 * Revision 1.5  2003/03/31 17:38:21  jerry
 * updated the structure
 * changed ppmio to ppm
 * added pcx
 *
 * Revision 1.4  2003/03/31 17:07:54  jerry
 * Moved out the load/save methods from image.c into ls.c
 * moved the io_util back up to gfxlib/
 *
 * Revision 1.3  2003/03/31 04:24:43  jerry
 * Finished the update to seperate everything around.
 *
 * Revision 1.2  2003/03/31 03:28:18  jerry
 * Moved stuff around. changed things.
 *
 * Revision 1.1  2003/03/07 20:23:43  jerry
 * Brought over the PPM library to use here for loading and saving images.
 * We'll also use the PPM image structure for our images.
 *
 * Revision 1.3  2000/05/30 18:33:18  sdlpci
 * Added Image_Duplicate()
 * Added a memset to Image_Create()
 *
 * Revision 1.2  1999/12/23  22:24:18  sdlpci
 * added bits to the structure
 * implemented interpolation/etrapolation routine
 * pulled out some functions to io_util.[ch]
 *
 * Revision 1.1  1999/12/23  19:02:51  sdlpci
 * Initial revision
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"


IMAGE * 
Image_Create (
	int width, 
	int height, 
	int bits
)
{
    IMAGE * temp = (IMAGE *) malloc(sizeof(IMAGE));
    
    if (temp)
    {
	temp->width  = width;
	temp->height = height;
	temp->bits   = bits;
	temp->data   = (PIXEL *)malloc(width * height * sizeof(PIXEL));
    }

    if (temp->data)
	memset(temp->data, 0, (width * height * sizeof(PIXEL)));

    return temp;
}


void
Image_Destroy (
	IMAGE * img
)
{
    if (img != NULL)
    {
	if (img->data != NULL)
	    free(img->data);
	free(img);
    }
}


IMAGE *
Image_Duplicate (
	IMAGE * src
)
{
    IMAGE * new = (IMAGE *) malloc(sizeof(IMAGE));
    
    if (new)
    {
	new->width  = src->width;
	new->height = src->height;
	new->bits   = src->bits;
	new->data   = (PIXEL *)malloc(src->width * src->height * sizeof(PIXEL));
    }

    if (new->data)
	memcpy(new->data, 
	       src->data, 
	       (src->width * src->height * sizeof(PIXEL))
	      );

    return new;
}
