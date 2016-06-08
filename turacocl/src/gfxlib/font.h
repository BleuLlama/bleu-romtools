/*
 * font.h
 *
 *  an internal font and some routines for using it
 */

#include "image.h"

#ifndef __FONT_H_
#define __FONT_H_

typedef struct __font_Definition{
    int width;			/* width of each characters */
    int height;			/* height of each character */
    int min_ascii;		/* minimum ascii value */
    int max_ascii;		/* maximum ascii value */
    unsigned char * data;	/* pointer to the data */
} FONT_DEFINITION;


/*
 * internal fonts... 
 */
extern FONT_DEFINITION * font_Internal_88;
extern FONT_DEFINITION * font_Internal_36;

/* 
 * some nice colors
 */
extern PIXEL font_White;
extern PIXEL font_Gray50;
extern PIXEL font_Red;
extern PIXEL font_Red2;



/* font_RenderChar
 *
 *  draw out a character
 */
void
font_RenderChar(
        IMAGE * dest,
        PIXEL * color,
        FONT_DEFINITION * font,
        int x,
        int y,
        int c
);


/* font_RenderText
 *
 *  draw out some left-justified text
 */
void
font_RenderText(
        IMAGE * dest,
        PIXEL * color,
        FONT_DEFINITION * font,  
        int x, int y,
        char * text
);


/* font_RenderTextCentered
 *
 *   draw out some center-justified text, centered at x,y
 */
void
font_RenderTextCentered(
        IMAGE * dest,
        PIXEL * color,
        FONT_DEFINITION * font,
        int x, int y,
        char * text
);


/* font_RenderTextRight
 *
 *  draw out some right-justified text  
 */
void
font_RenderTextRight(
        IMAGE * dest,
        PIXEL * color,
        FONT_DEFINITION * font,
        int x, int y,
        char * text
);

#endif
