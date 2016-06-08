/* lkarea.c */

/*
 * (C) Copyright 1989,1990
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */

#include <stdio.h>
#include <string.h>
#include <alloc.h>
#include "aslink.h"

/*
 * Create an area entry.
 *
 * A xxxxxx size nnnn flags mm
 *   |           |          |
 *   |           |          `--  ap->a_flag
 *   |           `------------- axp->a_size
 *   `-------------------------  ap->a_id
 *
 */
VOID
newarea()
{
	register i, narea;
	struct areax *taxp;
	struct areax **halp;
	char id[NCPS];

	/*
	 * Create Area entry
	 */
	getid(id, -1);
	lkparea(id);
	/*
	 * Evaluate area size
	 */
	skip(-1);
	axp->a_size = eval();
	/*
	 * Evaluate flags
	 */
	skip(-1);
	i = 0;
	taxp = ap->a_axp;
	while (taxp->a_axp) {
		++i;
		taxp = taxp->a_axp;
	}
	if (i == 0) {
		ap->a_flag = eval();
	} else {
		i = eval();
		if (i && (ap->a_flag != i))
		    fprintf(stderr, "Conflicting flags in area %.8s\n", id);
	}
	/*
	 * Place pointer in header area list
	 */
	if (headp == NULL) {
		fprintf(stderr, "No header defined\n");
		exit(1);
	}
	narea = hp->h_narea;
	halp = hp->a_list;
	for (i=0; i < narea ;++i) {
		if (halp[i] == NULL) {
			halp[i] = taxp;
			return;
		}
	}
	fprintf(stderr, "Header area list overflow\n");
	exit(1);
}

/*
 * Lookup the area `id'.
 * If it is not found create it.
 * Then append an area extension.
 */
VOID
lkparea(id)
char *id;
{
	register struct area *tap;
	register struct areax *taxp;

	ap = areap;
	axp = (struct areax *) new (sizeof(struct areax));
	while (ap) {
		if (symeq(id, ap->a_id)) {
			taxp = ap->a_axp;
			while (taxp->a_axp)
				taxp = taxp->a_axp;
			taxp->a_axp = axp;
			axp->a_bap = ap;
			axp->a_bhp = hp;
			return;
		}
		ap = ap->a_ap;
	}
	ap = (struct area *) new (sizeof(struct area));
	if (areap == NULL) {
		areap = ap;
	} else {
		tap = areap;
		while (tap->a_ap)
			tap = tap->a_ap;
		tap->a_ap = ap;
	}
	ap->a_axp = axp;
	axp->a_bap = ap;
	axp->a_bhp = hp;
	strncpy(ap->a_id, id, NCPS);
}

/*
 * Resolve all area addresses.
 */
VOID
lnkarea()
{
	register rloc;

	rloc = 0;
	ap = areap;
	while (ap) {
		if (ap->a_flag&A_ABS) {
			/*
			 * Absolute sections
			 */
			lnksect(ap);
		} else {
			/*
			 * Relocatable sections
			 */
			if (ap->a_addr == 0)
				ap->a_addr = rloc;
			lnksect(ap);
			rloc = ap->a_addr + ap->a_size;
		}
		ap = ap->a_ap;
	}
}

/*
 * Resolve section addresses within an area.
 */
VOID
lnksect(tap)
register struct area *tap;
{
	register addr_t size, addr;
	register struct areax *taxp;

	size = 0;
	addr = tap->a_addr;
	if ((tap->a_flag&A_PAG) && (addr & 0xFF))
	    fprintf(stderr,
			"\n?ASlink-W-Paged Area %.8s Boundary Error\n", tap->a_id);
	taxp = tap->a_axp;
	if (tap->a_flag&A_OVR) {
		/*
		 * Overlayed sections
		 */
		while (taxp) {
			taxp->a_addr = addr;
			if (taxp->a_size > size)
				size = taxp->a_size;
			taxp = taxp->a_axp;
		}
	} else {
		/*
		 * Concatenated sections
		 */
		while (taxp) {
			taxp->a_addr = addr;
			addr += taxp->a_size;
			size += taxp->a_size;
			taxp = taxp->a_axp;
		}
	}
	tap->a_size = size;
	if ((tap->a_flag&A_PAG) && (size > 256))
	    fprintf(stderr,
			"\n?ASlink-W-Paged Area %.8s Length Error\n", tap->a_id);
}
