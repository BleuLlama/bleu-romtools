/* lkrloc.c */

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
 * Process relocation operations
 * and call designated output routine
 */
VOID
reloc(c)
char c;
{
	switch(c) {

	case 'T':
		relt();
		break;

	case 'R':
		relr();
		break;

	case 'P':
		relp();
		break;

	case 'E':
		rele();
		break;

	default:
		fprintf(stderr, "Undefined Relocation Operation\n");
		break;

	}
}


/*
 * Relocation 'T' processing.
 */
VOID
relt()
{
	rtcnt = 0;
	while (more()) {
		if (rtcnt < NTXT) {
			rtval[rtcnt] = eval();
			rtflg[rtcnt] = 1;
			rtcnt++;
		}
	}
}

/*
 * Relocation 'R' processing
 */
VOID
relr()
{
	register mode;
	register addr_t reli, relv;
	int aindex, rindex, rtp, error;
	addr_t r, rtbase, rtofst, paga, pags, pc;
	struct areax **a;
	struct sym **s;

	/*
	 * Get area and symbol lists
	 */
	a = hp->a_list;
	s = hp->s_list;

	/*
	 * Verify Area Mode
	 */
	if (eval() != (R_WORD | R_AREA) || eval())
		fprintf(stderr, "R input error\n");

	/*
	 * Get area pointer
	 */
	aindex = evword();
	if (aindex >= hp->h_narea) {
		fprintf(stderr, "R area error\n");
		return;
	}

	/*
	 * Base values
	 */
	rtbase = add_w(0, 0);
	rtofst = 2;

	/*
	 * Relocate address
	 */
	pc = add_w(a[aindex]->a_addr, 0);

	/*
	 * Do remaining relocations
	 */
	while (more()) {
		error = 0;
		mode = eval();
		rtp = eval();
		rindex = evword();

		/*
		 * R_SYM or R_AREA references
		 */
		if (mode & R_SYM) {
			if (rindex >= hp->h_nglob) {
				fprintf(stderr, "R symbol error\n");
				return;
			}
			reli = symval(s[rindex]);
		} else {
			if (rindex >= hp->h_narea) {
				fprintf(stderr, "R area error\n");
				return;
			}
			reli = a[rindex]->a_addr;
		}

		/*
		 * R_PCR addressing
		 */
		if (mode & R_PCR) {
			if (mode & R_BYTE) {
				reli -= (pc + (rtp-rtofst) + 1);
			} else {
				reli -= (pc + (rtp-rtofst) + 2);
			}
		}

		/*
		 * R_PAG0 or R_PAG addressing
		 */
		if (mode & (R_PAG0|R_PAG)) {
			paga  = sdp.s_area->a_addr;
			pags  = sdp.s_addr;
			reli -= paga + pags;
		}

		/*
		 * R_BYTE or R_WORD operation
		 */
		if (mode & R_BYTE) {
			if (mode & R_BYT2) {
				relv = add_b2(reli, rtp);
			} else {
				relv = add_b1(reli, rtp);
			}
		} else {
			relv = add_w(reli, rtp);
		}

		/*
		 * R_BYTE with R_BYT2 offset adjust
		 */
		if (mode & R_BYTE) {
			if (mode & R_BYT2) {
				rtofst += 1;
			}
		}

		/*
		 * Unsigned Byte Checking
		 */
		if (mode & R_USGN && mode & R_BYTE && relv & ~0xFF)
			error = 1;

		/*
		 * PCR Relocation Error Checking
		 */
		if (mode & R_PCR && mode & R_BYTE) {
			r = relv & ~0x7F;
			if (r != (addr_t) ~0x7F && r != 0)
				error = 2;
		}

		/*
		 * Page Relocation Error Checking
		 */
		if (mode & R_PAG0 && (relv & ~0xFF || paga || pags))
			error = 3;
		if (mode & R_PAG  && (relv & ~0xFF))
			error = 4;

		/*
		 * Error Processing
		 */
		if (error) {
			rerr.aindex = aindex;
			rerr.mode = mode;
			rerr.rtbase = rtbase + rtp - rtofst - 1;
			rerr.rindex = rindex;
			rerr.rval = relv - reli;
			relerr(errmsg[error-1]);
		}
	}
	if (oflag == 1) {
		ihx(1);
	} else
	if (oflag == 2) {
		s19(1);
	}
}

char *errmsg[] = {
	"Unsigned Byte error",
	"Byte PCR relocation error",
	"Page0 relocation error",
	"Page Mode relocation error"
};


/*
 * Relocation 'P' processing
 */
VOID
relp()
{
	register aindex, rindex;
	int mode, rtp;
	addr_t relv;
	struct areax **a;
	struct sym **s;

	/*
	 * Get area and symbol lists
	 */
	a = hp->a_list;
	s = hp->s_list;

	/*
	 * Verify Area Mode
	 */
	if (eval() != (R_WORD | R_AREA) || eval())
		fprintf(stderr, "P input error\n");

	/*
	 * Get area pointer
	 */
	aindex = evword();
	if (aindex >= hp->h_narea) {
		fprintf(stderr, "P area error\n");
		return;
	}

	/*
	 * Do remaining relocations
	 */
	while (more()) {
		mode = eval();
		rtp = eval();
		rindex = evword();

		/*
		 * R_SYM or R_AREA references
		 */
		if (mode & R_SYM) {
			if (rindex >= hp->h_nglob) {
				fprintf(stderr, "P symbol error\n");
				return;
			}
			relv = symval(s[rindex]);
		} else {
			if (rindex >= hp->h_narea) {
				fprintf(stderr, "P area error\n");
				return;
			}
			relv = a[rindex]->a_addr;
		}
		add_w(relv, rtp);
	}

	/*
	 * Paged values
	 */
	aindex = add_w(0,2);
	if (aindex >= hp->h_narea) {
		fprintf(stderr, "P area error\n");
		return;
	}
	sdp.s_areax = a[aindex];
	sdp.s_area = sdp.s_areax->a_bap;
	sdp.s_addr = add_w(0,4);
	if (sdp.s_area->a_addr & 0xFF || sdp.s_addr & 0xFF)
		relerp("Page Definition Boundary Error");
}

/*
 * EOF processing
 */
VOID
rele()
{
	if (oflag == 1) {
		ihx(0);
	} else
	if (oflag == 2) {
		s19(0);
	}
}

/*
 * Evaluate word
 */
addr_t
evword()
{
	register addr_t v;

	if (hilo) {
		v =  (eval() << 8);
		v +=  eval();
	} else {
		v =   eval();
		v += (eval() << 8);
	}
	return(v);
}

/*
 * Add byte values
 */
addr_t
add_b1(v, i)
register addr_t v;
register int i;
{
	return(rtval[i] += v);
}

/*
 * Add byte values
 */
addr_t
add_b2(v, i)
addr_t v;
int i;
{
	register addr_t j;

	if (hilo) {
		j = v + (rtval[i] << 8) + (rtval[i+1] & 0xff);
		rtflg[i] = 0;
		rtval[i+1] = j & 0xff;
	} else {
		j = v + (rtval[i] & 0xff) + (rtval[i+1] << 8);
		rtval[i] = j & 0xff;
		rtflg[i+1] = 0;
	}
	return(j);
}

/*
 * Add word values
 */
addr_t
add_w(v, i)
register addr_t v;
register int i;
{
	register addr_t j;

	if (hilo) {
		j = v + (rtval[i] << 8) + (rtval[i+1] & 0xff);
		rtval[i] = (j >> 8) & 0xff;
		rtval[i+1] = j & 0xff;
	} else {
		j = v + (rtval[i] & 0xff) + (rtval[i+1] << 8);
		rtval[i] = j & 0xff;
		rtval[i+1] = (j >> 8) & 0xff;
	}
	return(j);
}

/*
 * Relocation Error Report
 */
VOID
relerr(str)
char *str;
{
	errdmp(stderr, str);
	if (mfp)
		errdmp(mfp, str);
}

/*
 * Relocation error dump routine
 */
VOID
errdmp(fptr, str)
FILE *fptr;
char *str;
{
	int mode, aindex, rindex;
	struct sym **s;
	struct areax **a;
	struct areax *raxp;

	a = hp->a_list;
	s = hp->s_list;

	mode = rerr.mode;
	aindex = rerr.aindex;
	rindex = rerr.rindex;

	/*
	 * Print Error
	 */
	fprintf(fptr, "\n?ASlink-W-%s", str);

	/*
	 * Print symbol if symbol based
	 */
	if (mode & R_SYM) {
		fprintf(fptr, " for symbol  %.*s\n", NCPS, &s[rindex]->s_id[0]);
	} else {
		fprintf(fptr, "\n");
	}

	/*
	 * Print Ref Info
	 */
	fprintf(fptr,
		"         file        module      area        offset\n");
	fprintf(fptr,
		"  Refby  %-8.8s    %-8.8s    %-8.8s    ",
			hp->h_lfile->f_idp,
			&hp->m_id[0],
			&a[aindex]->a_bap->a_id[0]);
	prntval(fptr, rerr.rtbase);

	/*
	 * Print Def Info
	 */
	if (mode & R_SYM) {
		raxp = s[rindex]->s_axp;
	} else {
		raxp = a[rindex];
	}
	fprintf(fptr,
		"  Defin  %-8.8s    %-8.8s    %-8.8s    ",
			raxp->a_bhp->h_lfile->f_idp,
			&raxp->a_bhp->m_id[0],
			&raxp->a_bap->a_id[0]);
	if (mode & R_SYM) {
		prntval(fptr, s[rindex]->s_addr);
	} else {
		prntval(fptr, rerr.rval);
	}
}

VOID
prntval(fptr, v)
FILE *fptr;
addr_t v;
{
	if (xflag == 0) {
		fprintf(fptr, "%04X\n", v);
	} else
	if (xflag == 1) {
		fprintf(fptr, "%06o\n", v);
	} else
	if (xflag == 2) {
		fprintf(fptr, "%05u\n", v);
	}
}

/*
 * Paging Error Report
 */
VOID
relerp(str)
char *str;
{
	erpdmp(stderr, str);
	if (mfp)
		erpdmp(mfp, str);
}

/*
 * Paging error dump routine
 */
VOID
erpdmp(fptr, str)
FILE *fptr;
char *str;
{
	register struct head *thp;

	thp = sdp.s_areax->a_bhp;

	/*
	 * Print Error
	 */
	fprintf(fptr, "\n?ASlink-W-%s\n", str);

	/*
	 * Print PgDef Info
	 */
	fprintf(fptr,
		"         file        module      pgarea      pgoffset\n");
	fprintf(fptr,
		"  PgDef  %-8.8s    %-8.8s    %-8.8s    ",
			thp->h_lfile->f_idp,
			&thp->m_id[0],
			&sdp.s_area->a_id[0]);
	prntval(fptr, sdp.s_area->a_addr + sdp.s_addr);
}
