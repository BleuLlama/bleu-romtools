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



typedef struct _rom_region {
	char * filename;
	char * label;
	unsigned long start;
	unsigned long size;
	unsigned char * data;
	unsigned char * changedData;
	unsigned long touched;
	struct _rom_region * next;
} ROM_REGION;


extern ROM_REGION * romlist;

ROM_REGION * rr_add(
	ROM_REGION * the_list,
	char * srcDir,
        char * filename,
        char * label,
        unsigned long start,
        unsigned long size
);

void rr_free_list(ROM_REGION * the_list);

#define RET_OKAY 	(0)	/* memory added */
#define RET_RANGE	(1)	/* memory is out of range */

int rr_set(
	ROM_REGION * the_list,
	unsigned long address,
	unsigned char datum
);


void rr_dump(ROM_REGION * rom);

void rr_save(ROM_REGION * rom);
void rr_save_list(ROM_REGION * the_list);
void rr_dump_stats( ROM_REGION * the_list, char * patchdir );
