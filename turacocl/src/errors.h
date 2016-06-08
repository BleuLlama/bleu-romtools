/*
 * errors.h
 *
 *  various error messages
 *
 * $Id: errors.h,v 1.12 2003/05/15 19:50:33 jerry Exp $
 */

#ifndef __ERRRORS_H__
#define __ERRRORS_H__

/* to look up an error, and return it in english... */
char * error_toString( int errno );

#define ERR_NONE		(0)	/* no error */

#define ERR_NO_TROMS		(-1)	/* TROMS env variable not set */
#define ERR_NO_TDRIVERS		(-2)	/* TDRIVERS env variable not set */

#define ERR_NO_MEMORY		(-3) 	/* malloc failed somewhere */

#define ERR_NO_PARAMS_STRUCT	(-4)	/* Turaco/params is null */
#define ERR_NO_DRIVER_STRUCT	(-5)	/* Turaco/driver is null */
#define ERR_NO_TURACO_STRUCT	(-6)	/* Turaco is null */

#define ERR_FIND_DRIVER_FAILED	(-7)	/* could not find the driver */
#define ERR_LOAD_DRIVER_FAILED	(-8)	/* could not load the found driver */

#define ERR_ROM_MISSING		(-9)	/* could not find a required rom */
#define ERR_KEY_MISSING		(-10)	/* could not find the color key file */
#define ERR_IMAGE_MISSING	(-11)   /* input image file not specified */

#define ERR_NO_IMAGE		(-12)	/* there is no image to save */
#define ERR_NO_INSTANCE		(-13)	/* there was no TuracoInstance */
#define ERR_NO_ROMFILE_DESCS	(-14)	/* there was no Rom File Desc's */

#define ERR_BANK_OUT_OF_RANGE	(-15)	/* user selected a bad bank number */
#define ERR_PAL_OUT_OF_RANGE	(-16)	/* user selected a bad palette number */
#define ERR_CONVERSION_UNKNOWN  (-17)	/* user didn't select ROM or PNM */

#define ERR_NO_FILENAME  	(-18)	/* output filename not specified */

#define ERR_BAD_FILE		(-19)	/* could not stat file */
#define ERR_BAD_FILE_SIZE	(-20)	/* the ROM Was the wrong size */

#define ERR_SRC_DIR_BAD		(-21)	/* the source dir was bad */
#define ERR_DST_DIR_BAD		(-22)	/* the destination dir was bad */

#define ERR_SET_NOT_FOUND	(-23)	/* the requested set was not found */
#define ERR_NAME_NOT_DEFINED	(-24)	/* a filename was missing */

#define ERR_TM_OUT_OF_RANGE	(-25)	/* tilemap selected was out of range */
#define ERR_TM_BANK_INCONSISTANT (-26)	/* selected tilemap didn't jive */

#define ERR_UNKNOWN		(-27)	/* unknown error */

#define ERR_MAX (-27)

#endif
