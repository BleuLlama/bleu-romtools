/* asout.c */

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


#define	 NTXT	16
#define	 NREL	16

char	 txt[NTXT];
char	 rel[NREL];

char	*txtp = { &txt[0] };
char	*relp = { &rel[0] };

/*
 * Output absolute byte.
 */
VOID
outab(b)
int b;
{
	if (pass == 2) {
		out_lb(b,0);
		if (oflag) {
			outchk(1, 0);
			*txtp++ = lobyte(b);
		}
	}
	++dot.s_addr;
}

/*
 * Output absolute word.
 */
VOID
outaw(w)
int w;
{
	if (pass == 2) {
		out_lw(w,0);
		if (oflag) {
			outchk(2, 0);
			out_tw(w);
		}
	}
	dot.s_addr += 2;
}

/*
 * Output relocatable byte.
 */
VOID
outrb(esp, r)
register struct expr *esp;
int r;
{
	register n;

	if (pass == 2) {
		if (esp->e_flag==0 && esp->e_base.e_ap==NULL) {
			out_lb(esp->e_addr,0);
			if (oflag) {
				outchk(1, 0);
				*txtp++ = lobyte(esp->e_addr);
			}
		} else {
			r |= R_BYTE|R_BYT2;
			out_lb(esp->e_addr,r|R_RELOC);
			if (oflag) {
				outchk(2, 4);
				out_tw(esp->e_addr);
				if (esp->e_flag) {
					n = esp->e_base.e_sp->s_ref;
					r |= R_SYM;
				} else {
					n = esp->e_base.e_ap->a_ref;
				}
				*relp++ = r;
				*relp++ = txtp - txt - 2;
				out_rw(n);
			}
		}
	}
	++dot.s_addr;
}

/*
 * Output relocatable word.
 */
VOID
outrw(esp, r)
register struct expr *esp;
int r;
{
	register n;

	if (pass == 2) {
		if (esp->e_flag==0 && esp->e_base.e_ap==NULL) {
			out_lw(esp->e_addr,0);
			if (oflag) {
				outchk(2, 0);
				out_tw(esp->e_addr);
			}
		} else {
			r |= R_WORD;
			out_lw(esp->e_addr,r|R_RELOC);
			if (oflag) {
				outchk(2, 4);
				out_tw(esp->e_addr);
				if (esp->e_flag) {
					n = esp->e_base.e_sp->s_ref;
					r |= R_SYM;
				} else {
					n = esp->e_base.e_ap->a_ref;
				}
				*relp++ = r;
				*relp++ = txtp - txt - 2;
				out_rw(n);
			}
		}
	}
	dot.s_addr += 2;
}

/*
 * Output Page Information
 */
VOID
outdp(carea, esp)
register struct area *carea;
register struct expr *esp;
{
	register n, r;

	if (pass == 2 && oflag != 0) {
		outchk(16,16);
		out_tw(carea->a_ref);
		out_tw(esp->e_addr);
		if (esp->e_flag || esp->e_base.e_ap!=NULL) {
			r = R_WORD;
			if (esp->e_flag) {
				n = esp->e_base.e_sp->s_ref;
				r |= R_SYM;
			} else {
				n = esp->e_base.e_ap->a_ref;
			}
			*relp++ = r;
			*relp++ = txtp - txt - 2;
			out_rw(n);
		}
		outbuf("P");
	}
}

/*
 * Clear out any bufferred text and relocation information
 */
VOID
outall()
{
	if (oflag && pass==2)
		outbuf("R");
}

/*
 * Check text buffer and relocation buffer.
 */
VOID
outchk(nt, nr)
int nt, nr;
{
	register struct area *ap;

	if (txtp+nt > &txt[NTXT] || relp+nr > &rel[NREL]) {
		outbuf("R");
	}
	if (txtp == txt) {
		out_tw(dot.s_addr);
		if ((ap = dot.s_area) != NULL) {
			*relp++ = R_WORD|R_AREA;
			*relp++ = 0;
			out_rw(ap->a_ref);
		}
	}
}

/*
 * Output any bufferred text and relocation information
 */
VOID
outbuf(s)
char *s;
{
	if (txtp > &txt[2]) {
		fprintf(ofp, "T");
		out(txt,(int) (txtp-txt));
		fprintf(ofp, "\n");
		fprintf(ofp, s);
		out(rel,(int) (relp-rel));
		fprintf(ofp, "\n");
	}
	txtp = txt;
	relp = rel;
}

/*
 * Walk through the symbol table and the
 * area list and put out the global
 * symbol information at the front of the
 * relocatable file. This routine is also
 * responsible for setting up the reference
 * numbers in the symbol tabel.
 */
VOID
outgsd()
{
	register struct area *ap;
	register struct sym  *sp;
	register i, j;
	char *ptr;
	int c, narea, nglob, rn;

	narea = areap->a_ref + 1;
	nglob = 0;
	for (i = 0; i < NHASH; ++i) {
		sp = symhash[i];
		while (sp) {
			if (sp->s_flag&S_GBL)
				++nglob;
			sp = sp->s_sp;
		}
	}
	if (xflag == 0) {
		fprintf(ofp, "X%c\n", hilo ? 'H' : 'L');
		fprintf(ofp, "H %X areas %X global symbols\n", narea, nglob);
	} else
	if (xflag == 1) {
		fprintf(ofp, "Q%c\n", hilo ? 'H' : 'L');
		fprintf(ofp, "H %o areas %o global symbols\n", narea, nglob);
	} else
	if (xflag == 2) {
		fprintf(ofp, "D%c\n", hilo ? 'H' : 'L');
		fprintf(ofp, "H %u areas %u global symbols\n", narea, nglob);
	}		

	/*
	 * Module name
	 */
	if (module[0]) {
		fprintf(ofp, "M ");
		ptr = &module[0];
		while (ptr < &module[NCPS]) {
			if ((c = *ptr++) != 0)
				putc(c, ofp);
		}
		putc('\n', ofp);
	}

	/*
	 * Global references and absolutes.
	 */
	rn = 0;
	for (i=0; i<NHASH; ++i) {
		sp = symhash[i];
		while (sp) {
			if (sp->s_area==NULL && sp->s_flag&S_GBL) {
				sp->s_ref = rn++;
				outsym(sp);
			}
			sp = sp->s_sp;
		}
	}

	/*
	 * Global relocatables.
	 */
	for (i=0; i<narea; ++i) {
		ap = areap;
		while (ap->a_ref != i)
			ap = ap->a_ap;
		outarea(ap);
		for (j=0; j<NHASH; ++j) {
			sp = symhash[j];
			while (sp) {
				if (sp->s_area==ap && sp->s_flag&S_GBL) {
					sp->s_ref = rn++;
					outsym(sp);
				}
				sp = sp->s_sp;
			}
		}
	}
}

/*
 * Output the relocatable item defining
 * an area.
 */
VOID
outarea(ap)
register struct area *ap;
{
	register char *ptr;
	register c;

	fprintf(ofp, "A ");
	ptr = &ap->a_id[0];
	while (ptr < &ap->a_id[NCPS]) {
		if ((c = *ptr++) != 0)
			putc(c, ofp);
	}
	if (xflag == 0) {
		fprintf(ofp, " size %X flags %X\n", ap->a_size, ap->a_flag);
	} else
	if (xflag == 1) {
		fprintf(ofp, " size %o flags %o\n", ap->a_size, ap->a_flag);
	} else
	if (xflag == 2) {
		fprintf(ofp, " size %u flags %u\n", ap->a_size, ap->a_flag);
	}
}

/*
 * Output the relocatable item describing a
 * global symbol.
 */
VOID
outsym(sp)
register struct sym *sp;
{
	register char *ptr;
	register c;

	fprintf(ofp, "S ");
	ptr = &sp->s_id[0];
	while (ptr < &sp->s_id[NCPS]) {
		if ((c = *ptr++) != 0)
			putc(c, ofp);
	}
	fprintf(ofp, " %s", sp->s_type==S_NEW ? "Ref" : "Def");
	if (xflag == 0) {
		fprintf(ofp, "%04X\n", sp->s_addr);
	} else
	if (xflag == 1) {
		fprintf(ofp, "%06o\n", sp->s_addr);
	} else
	if (xflag == 2) {
		fprintf(ofp, "%05u\n", sp->s_addr);
	}
}

VOID
out(p, n)
register char *p;
register int n;
{
	while (n--) {
		if (xflag == 0) {
			fprintf(ofp, " %02X", (*p++)&0377);
		} else
		if (xflag == 1) {
			fprintf(ofp, " %03o", (*p++)&0377);
		} else
		if (xflag == 2) {
			fprintf(ofp, " %03u", (*p++)&0377);
		}
	}
}

/*
 * Output a byte to the listing buffer.
 */
VOID
out_lb(b,t)
register int b,t;
{
	if (cp < &cb[NCODE])
		*cp++ = b;
		*cpt++ = t;
}

/*
 * Output ordered word to the listing buffer.
 */
VOID
out_lw(n,t)
register int n,t;
{
	if (hilo) {
		out_lb(hibyte(n),t ? t|R_HIGH : 0);
		out_lb(lobyte(n),t);
	} else {
		out_lb(lobyte(n),t);
		out_lb(hibyte(n),t ? t|R_HIGH : 0);
	}
}

/*
 * Output ordered 'Relocation' word.
 */
VOID
out_rw(n)
register int n;
{
	if (hilo) {
		*relp++ = hibyte(n);
		*relp++ = lobyte(n);
	} else {
		*relp++ = lobyte(n);
		*relp++ = hibyte(n);
	}
}

/*
 * Output ordered 'Text' word.
 */
VOID
out_tw(n)
register int n;
{
	if (hilo) {
		*txtp++ = hibyte(n);
		*txtp++ = lobyte(n);
	} else {
		*txtp++ = lobyte(n);
		*txtp++ = hibyte(n);
	}
}

/*
 * Extract low half of a word.
 */
int
lobyte(n)
int n;
{
	return (n&0377);
}

/*
 * Extract high half of a word.
 */
int
hibyte(n)
int n;
{
	return ((n>>8)&0377);
}
