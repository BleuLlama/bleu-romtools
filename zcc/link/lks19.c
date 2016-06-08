/* lks19.c */

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

VOID
s19(i)
int i;
{
	register addr_t chksum;

	if (i) {
		if (hilo == 0) {
			chksum = rtval[0];
			rtval[0] = rtval[1];
			rtval[1] = chksum;
		}
		for (i = 0, chksum = 1; i < rtcnt; i++) {
			if (rtflg[i])
				chksum++;
		}
		fprintf(ofp, "S1%02X", chksum);
		for (i = 0; i < rtcnt ; i++) {
			if (rtflg[i]) {
				fprintf(ofp, "%02X", rtval[i]);
				chksum += rtval[i];
			}
		}
		fprintf(ofp, "%02X\n", (-chksum-1) & 0xff);
	} else {
		fprintf(ofp, "S9030000FC\n");
	}
}
