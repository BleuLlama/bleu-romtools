/* aslist.c */

/*
 * (C) Copyright 1989,1990
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */

#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <alloc.h>
#include "asm.h"

/*
 * Copy a block of source and code to
 * the listing file. If no listing file or
 * the current line is not to be listed
 * just return. Multiple code bytes get
 * put out on extra lines after the source
 * line.
 */
VOID
list()
{
	register char *wp;
	register int *wpt;
	register nb;

	if (lfp == NULL || lmode == NLIST)
		return;
	slew(lfp, 0);
	while (ep < &eb[NERR])
		*ep++ = ' ';
	fprintf(lfp, "%.2s", eb);
	if (lmode == SLIST) {
		fprintf(lfp, "%24s%5u %s\n", "", line, ib);
		return;
	}
	if (xflag == 0) {		/* HEX */
		fprintf(lfp, " %04X", laddr);
		if (lmode == ALIST) {
			fprintf(lfp, "%19s%5u %s\n", "", line, ib);
			return;
		}
		wp = cb;
		wpt = cbt;
		nb = (int) (cp - cb);
		list1(wp, wpt, nb, 1);
		fprintf(lfp, " %5u %s\n", line, ib);
		while ((nb -= 6) > 0) {
			wp += 6;
			wpt += 6;
			slew(lfp, 0);
			fprintf(lfp, "%7s", "");
			list1(wp, wpt, nb, 0);
			putc('\n', lfp);
		}
	} else
	if (xflag == 1) {		/* OCTAL */
		fprintf(lfp, " %06o", laddr);
		if (lmode == ALIST) {
			fprintf(lfp, "%17s%5u %s\n", "", line, ib);
			return;
		}
		wp = cb;
		wpt = cbt;
		nb = (int) (cp - cb);
		list1(wp, wpt, nb, 1);
		fprintf(lfp, " %5u %s\n", line, ib);
		while ((nb -= 4) > 0) {
			wp += 4;
			wpt += 4;
			slew(lfp, 0);
			fprintf(lfp, "%9s", "");
			list1(wp, wpt, nb, 0);
			putc('\n', lfp);
		}
	} else
	if (xflag == 2) {		/* DECIMAL */
		fprintf(lfp, "  %05u", laddr);
		if (lmode == ALIST) {
			fprintf(lfp, "%17s%5u %s\n", "", line, ib);
			return;
		}
		wp = cb;
		wpt = cbt;
		nb = (int) (cp - cb);
		list1(wp, wpt, nb, 1);
		fprintf(lfp, " %5u %s\n", line, ib);
		while ((nb -= 4) > 0) {
			wp += 4;
			wpt += 4;
			slew(lfp, 0);
			fprintf(lfp, "%9s", "");
			list1(wp, wpt, nb, 0);
			putc('\n', lfp);
		}
	}
}

/*
 * Send bytes of code to the listing.
 * A subroutine of `list'.
 */
VOID
list1(wp, wpt, nb, f)
register char *wp;
register int nb, *wpt, f;
{
	register i;

	if (xflag == 0) {		/* HEX */
		if (nb > 6)
			nb = 6;
		for (i=0; i<nb; ++i) {
			list2(*wpt++);
			fprintf(lfp, "%02X", (*wp++)&0377);
		}
		if (f) {
			while (i < 6) {
				fprintf(lfp, "   ");
				++i;
			}
		}
	} else
	if (xflag == 1) {		/* OCTAL */
		if (nb > 4)
			nb = 4;
		for (i=0; i<nb; ++i) {
			list2(*wpt++);
			fprintf(lfp, "%03o", (*wp++)&0377);
		}
		if (f) {
			while (i < 4) {
				fprintf(lfp, "    ");
				++i;
			}
		}
	} else
	if (xflag == 2) {		/* DECIMAL */
		if (nb > 4)
			nb = 4;
		for (i=0; i<nb; ++i) {
			list2(*wpt++);
			fprintf(lfp, "%03u", (*wp++)&0377);
		}
		if (f) {
			while (i < 4) {
				fprintf(lfp, "    ");
				++i;
			}
		}
	}
}

/*
 * Send relocation type to the listing.
 * A subroutine of `list1'.
 */
VOID
list2(t)
register int t;
{
	register int c;

	c = ' ';
	if (fflag == 1) {
		if (t & R_RELOC) {
			c = '`';
		}
	} else
	if (fflag >= 2) {
		if (t & R_RELOC) {
			if (t & (R_PAG0|R_PAG)) {
				c = '*';
			} else if (t & R_USGN) {
				c = 'u';
			} else if (t & R_PCR) {
				c = 'p';
			} else {
				c = 'r';
			}
			if (t & R_HIGH) c += 1;
		}
	}
	putc(c, lfp);
}

/*
 * Increment the count of lines on the
 * page. If the page overflows:
 * 1) put out a page skip,
 * 2) a title,
 * 3) a subtitle,
 * 4) and reset the line count.
 */
VOID
slew(fp, doprint)
FILE	*fp;
int	doprint;
{
	if (lop++ >= NLPP && doprint) {
		fprintf(fp, "\fASxxxx Assembler %s  (%s), page %u.\n",
			VERSION, cpu, ++page);
		fprintf(fp, "%s\n", tb);
		fprintf(fp, "%s\n\n", stb);
	}
}

/*
 * Symbol and Area Table Output
 */
VOID
lstsym(fp)
FILE *fp;
{
	register int c, i, j, k;
	register char *ptr;
	int nmsym, narea;
	struct sym *sp;
	struct sym **p;
	struct area *ap;

	/*
	 * Symbol Table Header
	 */
	strcpy(stb, &symtbl[0]);
	lop = NLPP;
	if (fp == tfp)
		page = 0;
	slew(fp, 1);

	/*
	 * Find number of symbols
	 */
	nmsym = 0;
	for (i=0; i<NHASH; i++) {
		sp = symhash[i];
		while (sp) {
			if (sp != &dot)
				++nmsym;
			sp = sp->s_sp;
		}
	}
	if (nmsym == 0)
		goto atable;

	/*
	 * Allocate space for an array of pointers to symbols
	 * and load array.
	 */
	if ((p = (struct sym **) malloc(sizeof((struct sym *) sp)*nmsym))
		== NULL) {
		fprintf(fp, "Insufficient space to build Symbol Table.\n");
		return;
	}
	nmsym = 0;
	for (i=0; i<NHASH; i++) {
		sp = symhash[i];
		while (sp) {
			if (sp != &dot)
				p[nmsym++] = sp;
			sp = sp->s_sp;
		}
	}

	/*
	 * Bubble Sort on Symbol Table Array
	 */
	j = 1;
	c = nmsym - 1;
	while (j) {
		j = 0;
		for (i=0; i<c; ++i) {
			if (strcmp(&p[i]->s_id[0],&p[i+1]->s_id[0]) > 0) {
				j = 1;
				sp = p[i+1];
				p[i+1] = p[i];
				p[i] = sp;
			}
		}
	}

	/*
	 * Symbol Table Output
	 */
	for (i=0; i<nmsym;) {
		sp = p[i];
		if (sp->s_area) {
			j = sp->s_area->a_ref;
			if (xflag == 0) {
				fprintf(fp, " %2X ", j);
			} else
			if (xflag == 1) {
				fprintf(fp, "%3o ", j);
			} else
			if (xflag == 2) {
				fprintf(fp, "%3u ", j);
			}
		} else {
			fprintf(fp, "    ");
		}
		ptr = &sp->s_id[0];
		while (ptr < &sp->s_id[NCPS]) {
			if ((c = *ptr++) != 0) {
				putc(c, fp);
			} else {
				putc(' ', fp);
			}
		}
		if (sp->s_flag & S_ASG) {
			putc('=', fp);
		} else {
			putc(' ', fp);
		}
		if (sp->s_type == S_NEW) {
			if (xflag == 0) {
				fprintf(fp, "  **** ");
			} else
			if (xflag == 1) {
				fprintf(fp, "****** ");
			} else
			if (xflag == 2) {
				fprintf(fp, " ***** ");
			}
		} else {
			j = sp->s_addr;
			if (xflag == 0) {
				fprintf(fp, "  %04X ", j);
			} else
			if (xflag == 1) {
				fprintf(fp, "%06o ", j);
			} else
			if (xflag == 2) {
				fprintf(fp, " %05u ", j);
			}
		}
		j = 0;
		if (sp->s_flag & S_GBL) {
			putc('G', fp);
			++j;
		}
		if (sp->s_area != NULL) {
			putc('R', fp);
			++j;
		}
		if (sp->s_type == S_NEW) {
			putc('X', fp);
			++j;
		}
		putc('\n', fp);
		slew(fp, 0);
		++i;
#if 0
		if (++i % 3 == 0) {
			putc('\n', fp);
			slew(fp);
		} else
		if (i < nmsym) {
			while (j++ < 4)
				putc(' ', fp);
			fprintf(fp, "| ");
		}
#endif
	}
	putc('\n', fp);

	/*
	 * Area Table Header
	 */

atable:
	strcpy(stb, &aretbl[0]);
	lop = NLPP;
	slew(fp, 1);

	/*
	 * Area Table Output
	 */
	narea = 0;
	ap = areap;
	while (ap) {
		++narea;
		ap = ap->a_ap;
	}
	for (i=0; i<narea; ++i) {
		ap = areap;
		for (j=i+1; j<narea; ++j)
			ap = ap->a_ap;
		j = ap->a_ref;
		if (xflag == 0) {
			fprintf(fp, "  %2X ", j);
		} else
		if (xflag == 1) {
			fprintf(fp, " %3o ", j);
		} else
		if (xflag == 2) {
			fprintf(fp, " %3u ", j);
		}
		ptr = &ap->a_id[0];
		while (ptr < &ap->a_id[NCPS]) {
			if ((c = *ptr++) != 0) {
				putc(c, fp);
			} else {
				putc(' ', fp);
			}
		}
		j = ap->a_size;
		k = ap->a_flag;
		if (xflag==0) {
			fprintf(fp, "   size %4X   flags %X\n", j, k);
		} else
		if (xflag==1) {
			fprintf(fp, "   size %6o   flags %o\n", j, k);
		} else
		if (xflag==2) {
			fprintf(fp, "   size %5u   flags %u\n", j, k);
		}
	}		
}
