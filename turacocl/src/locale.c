/*
 *	locale.c
 *
 *	locale lookup guides
 */

#include <string.h>

/* our hash structure */
typedef struct locale_hash {
	char * key;
	char * value;
} locale_hash;


locale_hash lhash_en[] = {
	/* main.c */
	{ "Usage", "Usage" },
	{ "options", "options" },
	{ "Options", "Options" },

	{ "ERROR", "ERROR" },

	/* params.c */
	{ "You must specify either ROM or IMG with -inf",
		"You must specify either ROM or IMG with -inf" },

	{ "COMMON", "COMMON" },
	{ "Select the driver to use",
		"Select the driver to use" },
	{ "Use this specific driver",
		"Use this specific driver" },
	{ "Select the input format",
		"Select the input format" },
	{ "Which bank to use",
		"Which bank to use" },
	{ "Which palette to use",
		"Which palette to use" },

	{ "IMG to ROM", "IMG to ROM" },
	{ "The palette key filename",
		"The palette key filename" },
	{ "Absolute ROM input directory",
		"Absolute ROM input directory" },
	{ "Image format to use (PCX is default)",
		"Image format to use (PCX is default)" },
	{ "Import/Export tilemaps along with banks",
		"Import/Export tilemaps along with banks" },
	{ "Make color 0 transparent (where applicable, PNG)",
		"Make color 0 transparent (where applicable, PNG)" },
	{ "Dump out the driver structure",
		"Dump out the driver structure" },
	{ "Absolute ROM output directory",
		"Absolute ROM output directory" },

	{ "ROM to IMG", "ROM to IMG" },
	{ "Force this filename for the bank image",
		"Force this filename for the bank image" },
	{ "Force this filename for the checkerboard",
		"Force this filename for the checkerboard" },
	{ "How many sprites across",
		"How many sprites across" },

	{ "MISC", "MISC" },
	{ "Enable Curses-GUI Mode",
		"Enable Curses-GUI Mode" },
	{ "Select locale code",
		"Select locale code" },


	/* errors.c */
	{ "None",
		"None" },
	{ "TROMS environment variable not set",
		"TROMS environment variable not set" },
	{ "TDRIVERS environment variable not set",
		"TDRIVERS environment variable not set" },
	{ "Memory error",
		"Memory error" },
	{ "Turaco/params is null",
		"Turaco/params is null" },
	{ "Turaco/driver is null",
		"Turaco/driver is null" },

	{ "Turaco is null",
		"Turaco is null" },
	{ "Couldn't find the driver",
		"Couldn't find the driver" },
	{ "Couldn't load the driver",
		"Couldn't load the driver" },
	{ "Couldn't find a required ROM",
		"Couldn't find a required ROM" },
	{ "Couldn't find the color key file",
		"Couldn't find the color key file" },

	{ "Input image file not specified",
		"Input image file not specified" },
	{ "No image to save",
		"No image to save" },
	{ "No TuracoInstance",
		"No TuracoInstance" },
	{ "No ROM file desc",
		"No ROM file desc" },
	{ "Bank number selected was out of range",
		"Bank number selected was out of range" },

	{ "Palette number selected was out of range",
		"Palette number selected was out of range" },
	{ "ROM or PNM were not selected",
		"ROM or PNM were not selected" },
	{ "Output filename was not specified",
		"Output filename was not specified" },
	{ "Could not stat the file",
		"Could not stat the file" },
	{ "ROM file was of unexpected size",
		"ROM file was of unexpected size" },

	{ "Source directory was bad",
		"Source directory was bad" },
	{ "Destination directory was bad",
		"Destination directory was bad" },
	{ "Requested set was not found",
		"Requested set was not found" },
	{ "Missing filename",
		"Missing filename" },
	{ "Tilemap selected was out of range",
		"Tilemap selected was out of range" },
	{ "Tilemap bank was inconsistant",
		"Tilemap bank was inconsistant" },

	{ "Unknown error",
		"Unknown error" },

	{ "", "" }
};


/* the current locale */
locale_hash *current_locale = lhash_en;

/* locale_SetCode
**	set the current language via 2 or 3 letter code
**	ref: http://www.loc.gov/standards/iso639-2/php/code_list.php
*/
void locale_SetCode( char * code )
{
	/* fallback to english, just in case */
	current_locale = lhash_en;

	if( !strcmp( code, "en" ))  current_locale = lhash_en; /* ISO 639-1 */
	if( !strcmp( code, "eng" )) current_locale = lhash_en; /* ISO 639-2 */
}


/* locale_
**	return the correct string (or the key if not found)
*/
char * locale_( char * key ) {
	int item;
	if( !current_locale ) return( key );

	/* look for it */
	for( item=0 ; current_locale[item].key ; item++ )
	{
		if( !strcmp( current_locale[item].key, key ))
			return( current_locale[item].value );
	}

	/* not found, attempt to look in the english one for it */
	for( item=0 ; lhash_en[item].key ; item++ )
	{
		if( !strcmp( lhash_en[item].key, key ))
			return( lhash_en[item].value );
	}

	/* nope.  just return the key */
	return( key );

}
