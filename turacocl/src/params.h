/* params.h
 *
 * $Id: params.h,v 1.19 2003/05/29 02:50:00 jerry Exp $
 */


#ifndef __PARAMS_H__
#define __PARAMS_H__

#include <stdio.h>	/* for FILE */
#include "gfxlib.h"	/* for "FileFormat" */


typedef struct {
    /* common */
    char * drv;	/* driver name */
    char * ini;	/* driver absolute name */
    char * inf; /* input file style IMG or ROM */
    int    bnk;	/* bank number */
    int    pal;	/* palette number */
    char * key;	/* key file name */
    char * rom;	/* rom input directory */

    FileFormat ff; /* which image format to use */
    char * ffs;    /* temporary location for the above, and extension */
    int    trn;    /* should color 0 be transparent? (PNG only) */

    /* IMG to ROM */
    char * rod;	/* rom output directory */

    /* ROM to IMG */
    int eightbit; /* user palette range override for 8 bit color */
    char * dbf;	/* bank output IMG filename */
    char * chk;	/* checkerboard filename */
    int    wid;	/* number of sprites wide we're doing */

    /* tilemap */
    char * tms;  /* tilemap set name */
    int    tmsn; /* number of tilemap set: 0 for none, -1 for error */
    int    line; /* use a linear tilemap 0,1,2,3,4....*/

    /* oneshot */
    int h;	/* help */
    int v;	/* version */

    /* env vars */
    char * tdrivers;  /* TDRIVERS environment variable */
    char * troms;     /* TROMS    environment variable */

    /* misc */
    int gui;	/* enable GUI mode? 0=no, !0=yes */

    /* debug stuff */
    int dmp;	/* dump out the driver */
} UserParams;


int params_IsDir( char * path );



/* params_Parse
 *
 *  checks the environment variables and command line and
 *  sets the appropriate parameter options as such 
 *  This needs to be freed later.
 */
UserParams *
params_Parse( UserParams * pup, int argc, char **argv );



/* params_Verify
 *
 * check validity of parameters
 * returns 0 on OK.
 */
int params_Verify( UserParams * pup );



/* params_Usage
 *
 * dumps out the parameter usages to the passed in file
 */
void params_Usage( FILE * fp );

#endif

