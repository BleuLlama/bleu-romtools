/*
 * main
 *
 *  2000 Dec 21
 *  yorgle@gmail.com
 */

/*
 * genroms - Generates ROM images from IHX files
 * Copyright (C) 2000-2003 Scott Lawrence yorgle@gmail.com
 *
 *  This is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "romreg.h"
#include "romsread.h"
#include "readihx.h"

void usage(
	void
)
{
    fprintf(stderr, "%s v%s, Scott Lawrence\n", J_APPNAME, J_VERSION);
    fprintf(stderr, "Usage:\n\t%s  filename.roms  ( filename.ihx [-patch dir] | -list | -listall )\n", J_APPNAME );
    fprintf(stderr, "\n");
    fprintf(stderr, "  %s file.roms file.ihx              generate roms from .ihx file\n", J_APPNAME);
    fprintf(stderr, "  %s file.roms file.ihx -patch dir   \", starting with the roms in (dir)\n", J_APPNAME);
    fprintf(stderr, "  %s file.roms -list                 list the program rom files\n", J_APPNAME);
    fprintf(stderr, "  %s file.roms -listall              list all of the rom files\n", J_APPNAME);
}


int file_exists(
	char * filename
)
{
    FILE * fp = fopen(filename, "r");
    if (fp)
    {
	fclose(fp);
	return 1;
    }
    return 0;
}


int main(
	int argc,
	char ** argv
)
{
    FILE * ihx_fp = NULL;
    char * romsfilename = NULL;
    char * ihxfilename = NULL;
    int ret;
    unsigned int pos;
    int nbytes;
    char * patchdir = NULL;

    if (argc == 5) //    a.out file.roms file.ihx -patch dir
    {
	if( !strcmp( argv[ 3 ], "-patch" ))
	{
	    patchdir = argv[ 4 ];
	}
    }

    if ((argc != 3) && (patchdir == NULL))
    {
	usage();
	return 1;
    }

    romsfilename = argv[1];
    ihxfilename = argv[2];

    if (!file_exists(romsfilename))
    {
	fprintf(stderr, "ROMS file '%s' is not readable\n", romsfilename);
	return -5;
    }

    if( !strcmp( ihxfilename, "-list" ))
    {
	// list out the 'program' set of roms instead of generating them

	if (!roms_loadfile(NULL, romsfilename, 1))
	{
	    if (!romlist)
	    {
		fprintf(stderr, "Unable to read in .ROMS\n");
		return( 0 );
	    }
	}
	return( 0 );
    }

    if( !strcmp( ihxfilename, "-listall" ))
    {
	// list out the 'program' set of roms instead of generating them

	if (!roms_loadfile(NULL, romsfilename, 2))
	{
	    if (!romlist)
	    {
		fprintf(stderr, "Unable to read in .ROMS\n");
		return( 0 );
	    }
	}
	return( 0 );
    }

    if (!file_exists(ihxfilename))
    {
	fprintf(stderr, "IHX file '%s' is not readable\n", ihxfilename);
	return -5;
    }

    nbytes = 0;
    printf("ROMS: %s\n IHX: %s\n", romsfilename, ihxfilename);

    if (!roms_loadfile(patchdir, romsfilename, 0))
    {
	if (!romlist)
	{
	    fprintf(stderr, "Unable to read in .ROMS\n");
	    return 0;
	}

	ihx_fp = fopen(ihxfilename, "r");

	if( !ihx_fp )
	{
	    fprintf( stderr, "%s: error opening IHX file!\n", ihxfilename );
	    return( -1 );
	}

	printf("Working.."); fflush(stdout);
	decoded.type = 0;
	while (decoded.type != 01)
	{
	    next_ihx(ihx_fp);

	    printf("."); fflush(stdout);
	    nbytes += decoded.length;
	    for (pos = 0; pos<decoded.length ; pos++)
	    {
		ret = rr_set(romlist, decoded.address+pos, decoded.data[pos]);
		if (ret == RET_RANGE)
		{
		    fprintf(stderr, "0x%04x: OUT OF RANGE\n", decoded.address+pos);
		}
	    }
	}
	
	printf("Done.\n"); 
	fclose(ihx_fp);
	rr_save_list( romlist );
	rr_dump_stats( romlist, patchdir );
	printf("%21s %d bytes %s.\n",
		"", nbytes, (patchdir)?"patched":"encoded");
	fflush(stdout);
    }
    rr_free_list(romlist);

    return 0;
}
