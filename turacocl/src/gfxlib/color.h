/*
 *  color.h
 *
 *   color manipulations functions
 *
 * $Id: color.h,v 1.2 2003/05/31 03:16:03 jerry Exp $
 *
 * $Log: color.h,v $
 * Revision 1.2  2003/05/31 03:16:03  jerry
 * Fixed Bug 15. (Photoshop 5 color images)
 *
 * Revision 1.1  2003/05/30 21:54:54  jerry
 * Adding the color reducer for BUG 15 (gfxlib/color.[ch])
 *
 *
 */
#ifndef __COLOR_H__
#define __COLOR_H__

/* Color_Find24
 *
 *  finds (r g b) in the palette, with a max of [[ncolors]] colors
 */
int       
Color_Find24 (
        IMAGE * palette,
        int ncolors,
        int r, int g, int b
);


/* Color_Reduce24
 *
 *   reduce colors in the 'a' channel based on the palette image passed in
 */
void
Color_Reduce24 (
        IMAGE * img,
        IMAGE * palette,
        int ncolors
);


/* Color_ReduceIntegrated
 *
 *   reduce colors in the image to ncolors
 */
void
Color_ReduceIntegrated (
        IMAGE * img,
        int ncolors
);
#endif
