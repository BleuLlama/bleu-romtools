/*
 * readroms
 *
 *  parse roms description file
 */


/*
 * mkroms - Generates ROM images from IHX files
 * Copyright (C) 2000 Scott Lawrence - yorgle@gmail.com
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


/*
 * the basic file layout is:

begin bitname
start_address 	size	romname	  label_name
start_address 	size	romname	  label_name
start_address 	size	romname	  label_name
end
...

an example (from 'Pengo'):

begin program
0x0000  0x1000  ic8             program_1
0x1000  0x1000  ic7             program_2
0x2000  0x1000  ic15            program_3
0x3000  0x1000  ic14            program_4
0x4000  0x1000  ic21            program_5
0x5000  0x1000  ic20            program_6
0x6000  0x1000  ic32            program_7
0x7000  0x1000  ic31            program_8
end

This specifies how big the roms are, their location in memory, and 
the names of the files to generate when the program is run.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_util.h"
#include "readihx.h"
#include "romreg.h"

int roms_loadfile(
	char * srcDir,
	char * filename, 
	int dump
)
{
    char * ts = NULL;
    char * rn = NULL;
    char * id = NULL;
    FILE * fp = NULL;
    int done = 0;
    int in_bank = 0;
    int in_program = 0;
    long start, size;

    fp = fopen(filename, "r");
    if (!fp) return -2;

    while (!done && EOF != fpeek(fp))
    {
	if (ts) free(ts);
	ts = read_string(fp);

	if (!strcmp(ts, "end"))
	{
	    if( dump != 2 )
		done = 1;
	    in_bank = 0;
	    in_program = 0;
	}

	if (!done && in_bank)
	{
	    if (ts[0] != '0' && ts[1] != 'x') 
	    {
		fprintf(stderr, "\"%s\" is not a valid start value.\n", ts);
		free(ts);
		fclose(fp);
		return -1;
	    }
	    start = hexstring_to_long(2, ts);
	    free(ts);

	    ts = read_string(fp);
	    if (ts[0] != '0' && ts[1] != 'x') 
	    {
		fprintf(stderr, "\"%s\" is not a valid size.\n", ts);
		free(ts);
		fclose(fp);
		return -1;
	    }
	    size = hexstring_to_long(2, ts);
	    free(ts);

	    rn = read_string(fp);
	    if( (dump == 2) || (in_program && dump) )
	    {
		printf( "%s ", rn );
	    }
	    id = read_string(fp);

	    romlist = rr_add(romlist, srcDir, rn, id, start, size);

	    //free(rn);		// uncomment this for a crash!?
	    free(id);
	}

	if (!strcmp(ts, "begin"))
	{
	    free(ts);
	    ts = read_string(fp);
	    in_bank = 1;
	    //printf ("Reading Bank '%s'.\n", ts);
	    if( !strcmp( ts, "program" ))
	    {
		in_program = 1;
	    }
	}

    }
    free(ts);

    fclose(fp);
    if( dump )
    {
	printf( "\n" );
    }
    return 0;
}

    
