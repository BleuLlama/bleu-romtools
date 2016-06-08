/* params.c
 *
 * $Id: params.c,v 1.21 2003/05/29 02:50:00 jerry Exp $
 */

#include <stdio.h>
#include <stdlib.h>  /* malloc */
#include <string.h>  /* strcmp */
#include <sys/types.h> /* opendir */
#include <dirent.h>    /* opendir */
#include <ctype.h>    /* tolower */
#include "params.h"
#include "locale.h"
#include "errors.h"
#include "gfxlib.h"

/* _params_NewUP
 *
 *  generates a new user params structure
 */ 
UserParams *
_params_NewUP( void )
{
    UserParams * pup = (UserParams *)malloc( sizeof( UserParams ));

    if( pup )
    {
	/* zero it all */

	/* common */
	pup->drv = pup->ini =
	pup->inf = pup->key = pup->rom = NULL;
	pup->bnk = pup->pal = 1;

	/* imageFormat */
	pup->ffs  = NULL;
	pup->ff   = ff_PCX;
	pup->trn  = 0;

	/* tilemap set */
	pup->tms  = NULL;
	pup->tmsn = 0;

	/* IMG to ROM */
	pup->rod = NULL;

	/* ROM to IMG */
	pup->wid = 32;
	pup->dbf = pup->chk = NULL;

	/* oneshot */
	pup->v = pup->h = 0;

	/* env vars */
	pup->tdrivers = pup->troms = NULL;

	/* misc */
	pup->gui = 0;

	/* debug stuff */
	pup->dmp = 0;
    }

    return( pup );
}


/* params_ParseArgv  
 * 
 * parse the arg vector, generating a UserParams structure.
 * Need to free it later.
 */
UserParams *
params_ParseArgv( UserParams * pup, int argc, char ** argv )
{
    int ac;

    /* if no params were passed in, create one */
    if (!pup)
	pup = _params_NewUP();

    if (pup)
    {
	/* we need to move over the arg vector to find stuff */
	for( ac=1 ; ac<argc ; ac++ )
	{
	    /* common arguments */
	         if (!strcmp( argv[ac], "-drv" ))  pup->drv = argv[++ac];
	    else if (!strcmp( argv[ac], "-ini" ))  pup->ini = argv[++ac];
	    else if (!strcmp( argv[ac], "-inf" ))  pup->inf = argv[++ac];
	    else if (!strcmp( argv[ac], "-bnk" ))  pup->bnk = atoi(argv[++ac]);
	    else if (!strcmp( argv[ac], "-pal" ))  pup->pal = atoi(argv[++ac]);
	    else if (!strcmp( argv[ac], "-key" ))  pup->key = argv[++ac];
	    else if (!strcmp( argv[ac], "-rom" ))  pup->rom = argv[++ac];
	    else if (!strcmp( argv[ac], "-trom"))  pup->rom = pup->troms;

	    else if (!strcmp( argv[ac], "-ff" ))   pup->ffs = argv[++ac];

	    else if (!strcmp( argv[ac], "-tms" ))  pup->tms = argv[++ac];

	    /* added for PNG exportation, might be useful in the future */
	    else if (!strcmp( argv[ac], "-trn" ))  pup->trn = 1;

	    /* misc one-shot */
	    else if (!strcmp( argv[ac], "-h"   ))  pup->h = 1;
	    else if (!strcmp( argv[ac], "-v"   ))  pup->v = 1;

	    /* debug stuff */
	    else if (!strcmp( argv[ac], "-dmp" ))  pup->dmp = 1;

	    /* IMG to ROM specific*/
	    else if (!strcmp( argv[ac], "-rod" ))  pup->rod = argv[++ac];

	    /* ROM to IMG specific*/
	    else if (!strcmp( argv[ac], "-dbf" ))  pup->dbf = argv[++ac];
	    else if (!strcmp( argv[ac], "-chk" ))  pup->chk = argv[++ac];
	    else if (!strcmp( argv[ac], "-wid" ))  pup->wid = atoi(argv[++ac]);

	    else if (!strcmp( argv[ac], "-lang" )) locale_SetCode( argv[++ac]);
#ifdef USE_CURSES
	    else if (!strcmp( argv[ac], "-gui" ))  pup->gui = 1;
#endif
	}
    }

    return( pup );
}


/* params_IsDir
 * 
 *  returns 0 if the passed in path is not a dir or not writable
 */
int
params_IsDir( char * path )
{
    DIR * td = opendir( path );

    if( td )
    {
	closedir( td );
	return( 1 );
    }
    return( 0 );
}


/* params_ParseEnvVars
 *
 *  checks the environment variables and sets the appropriate
 *  parameter options as such with cheese and pickles
 */
UserParams *
params_ParseEnvVars( UserParams * pup )
{
    char * r;

    /* if no params were passed in, create one */
    if (!pup)
	pup = _params_NewUP();

    if (pup)
    {
	r = getenv("TDRIVERS");   if (r)  pup->tdrivers = r;
	r = getenv("TROMS");      if (r)  pup->troms = r; 
    }

    return( pup );
}


/* params_Parse
 *
 *  checks the environment variables and command line and 
 *  sets the appropriate parameter options as such with 
 *  cheese and pickles
 */
UserParams *
params_Parse( UserParams * pup, int argc, char **argv )
{
    pup = params_ParseEnvVars( pup );
    pup = params_ParseArgv( pup, argc, argv );

    return( pup );
}



/* params_Verify
 *
 * check validity of parameters
 * returns 0 on OK.
 */
int
params_Verify( UserParams * pup )
{
    char tbuff[4];
    int x;

    if( !pup ) return( ERR_NO_PARAMS_STRUCT );

    if( !pup->tdrivers )
    {
	fprintf( stderr, "Error: TDRIVERS environment variable not defined!\n");
	return( ERR_NO_TDRIVERS );
    }

    if( (!pup->inf) || (strcmp(pup->inf, "ROM") && strcmp(pup->inf, "IMG") ) )
    {
	fprintf( stderr, "%s: %s\n",
		locale_( "ERROR" ),
		locale_( "You must specify either ROM or IMG with -inf" ));
	return( ERR_CONVERSION_UNKNOWN );
    }

    /* adjust the file format selector */
    if( pup->ffs )
    {
	strncpy( tbuff, pup->ffs, 3 );
	tbuff[3] = '\0';
	for( x=0 ; x < strlen( tbuff ) ; x++ ) tbuff[x] = tolower( tbuff[x] );

	if( !strcmp( pup->ffs, "ppm" ) )
	{
	    pup->ff  = ff_PPM;
#ifdef USE_MAGICK
	} else if( !strcmp( pup->ffs, "png" )) {
	    pup->ff  = ff_PNG;
#endif
	} else {
	    fprintf( stderr, "Unrecognized format selector: %s.  Using PCX", pup->ffs );
	    pup->ff  = ff_PCX;
	}
    }

    if( pup->ff == ff_PPM )
    {
	pup->ffs = "pnm";
#ifdef USE_MAGICK
    } else if( pup->ff == ff_PNG ) {
	pup->ffs = "png";
#endif
    } else {
	pup->ffs = "pcx";
    }

    /* just in case */
    if( pup->wid < 1 )
	pup->wid = 1;

    return( ERR_NONE );
}



/* params_Usage
 *
 * dumps out the parameter usages to the passed in file
 */
void params_Usage( FILE * fp )
{
    fprintf( fp, 
	    "    == %s ==\n"
	    "\t-drv DriverName          %s\n"
	    "\t-ini AbsoluteDriverPath  %s\n"
	    "\t-inf (IMG|ROM)           %s\n" 
	    "\t-bnk BankNumber          %s\n"
	    "\t-pal PaletteNumber       %s\n" 
	    "\n",
		locale_( "COMMON" ),
		locale_( "Select the driver to use" ),
		locale_( "Use this specific driver" ),
		locale_( "Select the input format" ),
		locale_( "Which bank to use" ),
		locale_( "Which palette to use" )
		);

    fprintf( fp,
	    "\t-key KeyFileName         %s\n"
	    "\t-rom InputDirectory      %s\n"
#ifdef USE_MAGICK
	    "\t-ff  (PPM|PCX|PNG)       %s\n"
#else
	    "\t-ff  (PPM|PCX)           %s\n"
#endif
	    "\t-trn                     %s\n"
	    "\n"
	    "\t-tms TilemapSetName      %s\n"
	    "\n"
	    "\t-dmp                     %s\n"
	    "\n",
		locale_( "The palette key filename" ),
		locale_( "Absolute ROM input directory" ),
		locale_( "Image format to use (PCX is default)" ),
		locale_( "Make color 0 transparent (where applicable, PNG)" ),
		locale_( "Import/Export tilemaps along with banks" ),
		locale_( "Dump out the driver structure" )
		);


    fprintf( fp, 
	    "    == %s ==   (-inf IMG)\n"
	    "\t-rod OutputDirectory     %s\n"
	    "\n",
		locale_( "IMG to ROM" ),
		locale_( "Absolute ROM output directory" )
		);

    fprintf( fp, 
	    "    == %s ==   (-inf ROM)\n"
	    "\t-dbf BankImageFile       %s\n"
	    "\t-chk CheckerboardFile    %s\n"
	    "\t-wid NumberSpritesWide   %s\n"
	    "\n",
		locale_( "ROM to IMG" ),
		locale_( "Force this filename for the bank image" ),
		locale_( "Force this filename for the checkerboard" ),
		locale_( "How many sprites across" )
		);

    fprintf( fp, 
	    "    == %s ==\n"
	    "\t-lang en                 %s\n",
		locale_( "MISC" ),
		locale_( "Select locale code" )
		 );
#ifdef USE_CURSES
    fprintf( fp, 
	    "\t-gui                     %s\n",
		locale_( "Enable Curses-GUI Mode" )
		 );
#endif
    fprintf( fp, "\n" );
	}
