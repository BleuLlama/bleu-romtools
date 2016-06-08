/* romreg
 * 
 *   stuff for dealing with rom regions.
 *
 *  typical use would be:
 *	rr_add() for each rom
 *	rr_set() to fill the regions
 *	rr_save() to write out the changes to disk
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "romreg.h"


/*
typedef struct _rom_region {
	char * filename;
	char * label;
	unsigned long start;
	unsigned long size;
	unsigned char * data;
	unsigned long touched;
	struct _rom_region * next;
} ROM_REGION;
*/


ROM_REGION * romlist = NULL;

ROM_REGION * rr_add(
	ROM_REGION * list,
	char * srcDir,
        char * filename,
        char * label,
        unsigned long start,
        unsigned long size
)
{
    ROM_REGION * rrn = NULL;
    ROM_REGION * tt;

    rrn = (ROM_REGION *) malloc (sizeof(ROM_REGION));

    if (!rrn)  return list;

    rrn->filename = strdup(filename);
    rrn->label    = strdup(label);
    rrn->start    = start;
    rrn->size     = size;
    rrn->next     = NULL;
    rrn->data     = (unsigned char *)malloc(size);
    rrn->changedData = (unsigned char *)malloc(size);
    memset(rrn->data, 0, size);
    memset(rrn->changedData, 0, size);

    // if we have a dir to try to read in from, do so.
    if( srcDir )
    {
	char buf[1024];
	FILE * input = NULL;

	strncpy( buf, srcDir, 1024 );
	strncat( buf, "/", 1024 );
	strncat( buf, rrn->filename, 1024 );
	printf( "%16s ", buf ); 

	input = fopen( buf, "r" );
	if( input )
	{
	    // copy the file over in here
	    unsigned int nread = 
			    fread( rrn->data, 1, (size_t)rrn->size, input );

	    printf ( "%d bytes loaded.", nread );
	    if( nread != rrn->size )
	    {
		printf ( "  %ld bytes expected.\n", rrn->size );
	    } else {
		printf ( "\n" );
	    }
	    fclose( input );
	} else {
	    printf ( "Unable to open.\n" );
	}
    } 
    rrn->touched  = 0;

    // if there was no list, start one; return
    if( list == NULL )
	return( rrn );
    
    // advance to the last node in the list
    tt = list;
    while( tt->next )
	tt = tt->next;

    // shove ourselves on the end of the list and return
    tt->next = rrn;
    return( list );
}


void rr_free_list(ROM_REGION * the_list)
{
    ROM_REGION * temp;
    while (the_list)
    {
	temp = the_list->next;

	free(the_list->filename);
	free(the_list->label);
	free(the_list->data);
	free(the_list->changedData);
	free(the_list);

	the_list = temp;
    }
}


static int newlined = 0;

int rr_set(
	ROM_REGION * the_list,
	unsigned long address,
	unsigned char datum
)
{
    while (the_list)
    {
	if ( (address >= the_list->start) &&
	     (address < the_list->start + the_list->size) )
	{
	    if( the_list->changedData[address - the_list->start] == 0xff )
	    {
		if( !newlined )
		    printf( "\n" );
		newlined = 1;
		printf( "\tOverwrite: 0x%04lx\n", address);
	    } else {
		newlined = 0;
	    }
	    the_list->data[address - the_list->start] = datum;
	    the_list->changedData[address - the_list->start] = 0xff;
	    the_list->touched++;
	    return RET_OKAY;
	} 
	the_list = the_list->next;
    }
    return RET_RANGE;
}


void rr_dump(ROM_REGION * the_list)
{
    ROM_REGION *tr = the_list;

    while (tr)
    {
	printf("%10s %10s    0x%08lx to 0x%08lx\n",
		tr->label, tr->filename,
		tr->start, tr->start+tr->size-1);
	tr = tr->next;
    }
}

void rr_save(ROM_REGION * rom)
{
    FILE * fp;
    if (!rom) return;

    fp = fopen(rom->filename, "w");
    if (fp)
    {
	fwrite(rom->data, 1, rom->size, fp);
	fclose(fp);
    }
}


void rr_save_list(ROM_REGION * the_list)
{    
    ROM_REGION *tr = the_list;

    while (tr)
    {
	rr_save(tr);
	tr = tr->next;
    }
}

#define MIN( A, B )   ( A>B )?(B):(A)
#define MAX( A, B )   ( A>B )?(A):(B)

void rr_stats( ROM_REGION * rom, char * patchdir )
{
    unsigned int x;
    int count = 0;
    unsigned long low = 0xffffffff;
    unsigned long high = 0;

    for( x=0 ; x<rom->size ; x++ )
    {
	if( rom->changedData[x] )
	{
	    count++;
	    low = MIN( low, x );
	    high = MAX( high, x );
	}
    }
    if( count )
    {
	printf( "%20s: %6d/%6ld bytes %s  %%%2.0f  0x%04lx - 0x%04lx\n",
		rom->filename, count, rom->size,
		(patchdir)? "changed":"created",
		((float)count * 100.0)/(float)rom->size,
		low + rom->start, high + rom->start );
    } else {
	printf( "%20s: %6d/%6ld bytes %s  %%%2.0f\n",
		rom->filename, count, rom->size,
		(patchdir)? "changed":"created",
		((float)count * 100.0)/(float)rom->size );
    }
}


void rr_dump_stats( ROM_REGION * the_list, char * patchdir )
{
    ROM_REGION *tr = the_list;

    while( tr )
    {
	rr_stats( tr, patchdir );
	tr = tr->next;
    }
}
