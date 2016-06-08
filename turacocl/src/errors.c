/*
 *	errors.c
 *
 *	error messages
 */

#include "errors.h"
#include "locale.h"

char *errors[] = {
	"None",
	"TROMS environment variable not set",
	"TDRIVERS environment variable not set",
	"Memory error",
	"Turaco/params is null",
	"Turaco/driver is null",

	"Turaco is null",
	"Couldn't find the driver",
	"Couldn't load the driver",
	"Couldn't find a required ROM",
	"Couldn't find the color key file",

	"Input image file not specified",
	"No image to save",
	"No TuracoInstance",
	"No ROM file desc",
	"Bank number selected was out of range",

	"Palette number selected was out of range",
	"ROM or PNM were not selected",
	"Output filename was not specified",
	"Could not stat the file",
	"ROM file was of unexpected size",

	"Source directory was bad",
	"Destination directory was bad",
	"Requested set was not found",
	"Missing filename",
	"Tilemap selected was out of range",
	"Tilemap bank was inconsistant",

	"Unknown error",
	""
};

char * error_toString( int errno )
{
	if( (errno > 0) || (errno < ERR_MAX) ) errno = ERR_UNKNOWN;
	return( locale_( errors[ -errno ]) );
}
