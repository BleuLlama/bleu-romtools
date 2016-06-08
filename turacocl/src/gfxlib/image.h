/*
 *  image.h
 *
 *   simple image functions
 *
 * $Id: image.h,v 1.7 2003/05/30 21:54:54 jerry Exp $
 *
 * $Log: image.h,v $
 * Revision 1.7  2003/05/30 21:54:54  jerry
 * Adding the color reducer for BUG 15 (gfxlib/color.[ch])
 *
 * Revision 1.6  2003/05/29 20:44:35  jerry
 * Added Image_Blit
 *
 * Revision 1.5  2003/03/31 17:19:56  jerry
 * Updated everything to read "Jerry Carpenter"  ;)
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
#ifndef __IMAGE_H__
#define __IMAGE_H__

typedef struct pixel
{
    int r;
    int g;
    int b;
    int a;
} PIXEL;

typedef struct image
{
    int width;
    int height;
    int bits;
    PIXEL * data;
} IMAGE;


IMAGE * Image_Create(int width, int height, int bits_per_color);
void    Image_Destroy(IMAGE * img);

IMAGE * Image_Duplicate(IMAGE * src);
#endif
