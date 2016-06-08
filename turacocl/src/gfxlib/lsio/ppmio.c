/*
 *  ppmio
 *
 *   simple save/loader for ppm data
 *
 * $Id: ppm.c,v 1.10 2003/04/08 14:57:44 jerry Exp $
 *
 * $Log: ppm.c,v $
 * Revision 1.10  2003/04/08 14:57:44  jerry
 * Added the 'Query' operation.
 *
 * Revision 1.9  2003/04/02 21:53:20  jerry
 * Updated a lot of stuff.
 * changed 'xxx_ValidFile' to xxx_SupportedFile
 *
 * Revision 1.8  2003/03/31 17:38:21  jerry
 * updated the structure
 * changed ppmio to ppm
 * added pcx
 *
 * Revision 1.7  2003/03/31 17:31:40  jerry
 * Added pcx headers
 *
 * Revision 1.6  2003/03/31 17:19:56  jerry
 * Updated everything to read "Jerry Carpenter"  ;)
 *
 * Revision 1.5  2003/03/31 17:07:54  jerry
 * Moved out the load/save methods from image.c into ls.c
 * moved the io_util back up to gfxlib/
 *
 * Revision 1.4  2003/03/17 02:07:25  jerry
 * did something inconsequential
 *
 * Revision 1.3  2003/03/17 01:48:25  jerry
 * Added in the autoscale, then promptly #ifdef'ed it out.
 *
 * Revision 1.2  2003/03/13 01:40:57  jerry
 * added in the check for bits during save
 *
 * Revision 1.1  2003/03/07 20:23:43  jerry
 * Brought over the PPM library to use here for loading and saving images.
 * We'll also use the PPM image structure for our images.
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
#include "ppmio.h"
#include "ls.h"
#include "io_util.h"


IMAGE *
PPM_Load (
	char * filename
)
{
    FILE * in_fp;
    char * tempstr;
    int bits;
    int w, h;
    int c;
    int binary_file = 0;
    IMAGE * newimg;

    if (filename == NULL)
    {
	printf("Nothing to save.\n");
	return NULL;
    }

    in_fp = fopen(filename, "r");
    if (in_fp == NULL)
    {
	printf ("%s: Unable to open file\n", filename);
	return NULL;
    }
    
    /* insert loader code here. */
    tempstr = read_string(in_fp);

    if (strcmp(tempstr, "P3"))
    {
	if (strcmp(tempstr, "P6"))
	{
	    printf ("%s: Not a PPM file!  Expecting \"P3\" or \"P6\" but read \"%s\".\n",
			filename, tempstr);
	    fclose(in_fp);
	    free(tempstr);
	    return NULL;
	} else {
	    binary_file = 1;
	}
    }
    free(tempstr);

    w    = read_int(in_fp);
    h    = read_int(in_fp);
    bits = read_int(in_fp);

    if (w == 0 || h == 0)
    {
	printf ("%s: Unsupported PPM file: %d x %d  %d bits per color\n", 
		    filename, w,h,bits);
    }

    newimg = Image_Create(w,h,bits);
    if (newimg == NULL)
    {
	printf ("%s: Unable to load file.  Memory error\n", filename);
	return NULL;
    }

    newimg->width = w;
    newimg->height = h;
    newimg->bits = bits;

    if (!binary_file)
    {
	for (c=0 ; c<(w*h) ; c++)
	{
	    /* the format is: r g b r g b ... */
	    newimg->data[c].r = read_int(in_fp);
	    newimg->data[c].g = read_int(in_fp);
	    newimg->data[c].b = read_int(in_fp);
	}
    } else {
	for (c=0 ; c<(w*h) ; c++)
	{
	    /* the format is: r g b r g b ... */
	    newimg->data[c].r = fgetc(in_fp);
	    newimg->data[c].g = fgetc(in_fp);
	    newimg->data[c].b = fgetc(in_fp);
	}
    }

    fclose (in_fp);
    return newimg;
}



/*
P3
# CREATOR: XV Version 3.10a  Rev: 12/29/94
227 149
255
 48  47  45  48  47  45  49  48  46  50  49  47  53  49  46
 ...
*/

void
PPM_Save (
	char * filename, 
	IMAGE * tosave,
	int asbinary
)
{
    FILE * out_fp;
    int pix;
    int nperline;

    if (tosave == NULL || filename == NULL)
    {
	printf("Nothing to save.\n");
	return;
    }

    if (tosave->width == 0 || tosave->height == 0)
    {
	printf("%dx%d: Illegal image dimensions\n", 
		    tosave->width, tosave->height);
	return;
    }

    out_fp = fopen(filename, "w");
    if (!out_fp)
    {
	printf ("%s: Unable to open file\n", filename);
	return;
    }

    fprintf(out_fp, "P%d\n# CREATOR: Turaco CL  turacocl@umlautllama.com\n", 
		(asbinary == SAVE_BINARY)?6:3);
    fprintf(out_fp, "%d %d\n", tosave->width, tosave->height);
    fprintf(out_fp, "%d\n", 1<< tosave->bits);  /* levels per color per pixel */

    if (asbinary == SAVE_ASCII)
    {
	/* output an ascii ppm file.. */

	nperline = 0;
	for (pix = 0 ; pix < (tosave->width * tosave->height) ; pix++)
	{
	    fprintf(out_fp, "%3d %3d %3d ",    tosave->data[pix].r,
		    tosave->data[pix].g, tosave->data[pix].b
		    );
	    nperline++;
	    if (nperline > 5)
	    {
		nperline = 0;
		fprintf(out_fp, "\n");
	    }
	}
    } else {
	/* output a binary ppm file.. */
	for (pix = 0 ; pix < (tosave->width * tosave->height) ; pix++)
	{
	    fputc(tosave->data[pix].r, out_fp);
	    fputc(tosave->data[pix].g, out_fp);
	    fputc(tosave->data[pix].b, out_fp);
	}
    }
    fclose(out_fp);    
}


/*////////////////////////////////////////////////////////////////////////////*/

FileFormat
PPM_SupportedFile(
	char * filename
)
{
    /* eventually check the first value to see if it's a magic cookie */
    /* but for now, always assume it's correct. */
    filename = filename;
    return( ff_PPM ); 
}


/*////////////////////////////////////////////////////////////////////////////*/

int
PPM_Query(
        void
)
{
    return( QUERY_TRUECOLOR );
}


