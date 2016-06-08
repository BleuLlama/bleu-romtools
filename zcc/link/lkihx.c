/* lkihx.c */

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
ihx(i)
int i;
{
	register addr_t chksum;

	if (i) {
		if (hilo == 0) {
			chksum = rtval[0];
			rtval[0] = rtval[1];
			rtval[1] = chksum;
		}
		for (i = 0, chksum = -2; i < rtcnt; i++) {
			if (rtflg[i])
				chksum++;
		}
		fprintf(ofp, ":%02X", chksum);
		for (i = 0; i < rtcnt ; i++) {
			if (rtflg[i]) {
				fprintf(ofp, "%02X", rtval[i]);
				chksum += rtval[i];
			}
			if (i == 1) {
				fprintf(ofp, "00");
			}
		}
		fprintf(ofp, "%02X\n", (-chksum) & 0xff);
	} else {
		fprintf(ofp, ":00000001FF\n");
	}
}
