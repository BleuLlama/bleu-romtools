/* asmain.c */

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

extern int optind;
extern char *optarg;


int
main(argc, argv)
int argc;
char *argv[];
{
	register char *p;
	register c, i;
	struct area *ap;
	char	*ofile;
	FILE *afile();

	inpfil = -1;
	xflag = 0;
	gflag = 1;
	while ((c = getopt(argc, argv, "udqxgaclsfo:")) != EOF)
	{
		switch(c)
		{
			case 'a':
				++aflag;
				break;

			case 'c':
				ccase = icase;
				break;

			case 'u':
				gflag = 0;
				break;

			case 'g':
				gflag = 1;
				break;

			case 'l':
				++lflag;
				break;

			case 'o':
				++oflag;
				ofile = optarg;
				break;

			case 's':
				++sflag;
				break;

			case 'x':
				xflag = 0;
				break;

			case 'q':
				xflag = 1;
				break;

			case 'd':
				xflag = 2;
				break;

			case 'f':
				++fflag;
				break;

			default:
				usage();
			}
	}
	for (; optind < argc; optind++)
	{
		inpfil++;
		p = argv[optind];
		sfp[inpfil] = afile(p, "", 0);
		if (inpfil == 0)
		{
			if (!oflag)
			{
				ofp = afile(p, "rel", 1);
				oflag = 1;
			}
			else
			{
				if ((ofp = fopen(ofile, "w")) == NULL) {
					fprintf(stderr, "%s: cannot create.\n",
						ofile);
					exit(1);
				}
			}

			if (lflag)
				lfp = afile(p, "lst", 1);
			if (sflag)
				tfp = afile(p, "sym", 1);
		}
	}
	if (inpfil < 0)
		usage();
	syminit();
	for (pass=0; pass<3; ++pass) {
		if (gflag && pass == 1)
			symglob();
		if (aflag && pass == 1)
			allglob();
		if (oflag && pass == 2)
			outgsd();
		flevel = 0;
		tlevel = 0;
		ifcnd[0] = 0;
		iflvl[0] = 0;
		radix = 10;
		line = 0;
		page = 0;
		stb[0] = 0;
		lop  = NLPP;
		cfile = 0;
		incfil = -1;
		for (i = 0; i <= inpfil; i++)
			rewind(sfp[i]);
		ap = areap;
		while (ap) {
			ap->a_fuzz = 0;
			ap->a_size = 0;
			ap = ap->a_ap;
		}
		fuzz = 0;
		dot.s_addr = 0;
		dot.s_area = &dca;
		symp = &dot;
		minit();
		while (asgetline()) {
			++line;
			cp = cb;
			cpt = cbt;
			ep = eb;
			ip = ib;
			if (setjmp(jump_env) == 0)
				asmbl();
			if (pass == 2) {
				diag();
				list();
			}
		}
		newdot(dot.s_area); /* Flush area info */
		if (flevel || tlevel)
			err('i');
	}
	if (oflag)
		outchk(HUGE, HUGE);  /* Flush */
	if (sflag) {
		lstsym(tfp);
	} else
	if (lflag) {
		lstsym(lfp);
	}

	return 0;
}

VOID
asmbl()
{
	register struct mne *mp;
	register struct sym *sp;
	register struct tsym *tp;
	register c;
	struct area  *ap;
	struct expr e1, e2;
	char id[NCPS];
	char opt[NCPS];
	char fn[FILSPC];
	char *p;
	int d, n, uaf, uf;
	int cnt;

	laddr = dot.s_addr;
	lmode = SLIST;
loop:
	if ((c=endline()) == 0) { return; }
	if (ctype[c] & DIGIT) {
		if (flevel)
			return;
		n = 0;
		while ((d = digit(c, 10)) >= 0) {
			n = 10*n + d;
			c = get();
		}
		if (c != '$' || get() != ':')
			qerr();
		tp = symp->s_tsym;
		if (pass == 0) {
			while (tp) {
				if (n == tp->t_num) {
					tp->t_flg |= S_MDF;
					break;
				}
				tp = tp->t_lnk;
			}
			if (tp == NULL) {
				tp=(struct tsym *) new (sizeof(struct tsym));
				tp->t_lnk = symp->s_tsym;
				tp->t_num = n;
				tp->t_flg = 0;
				tp->t_area = dot.s_area;
				tp->t_addr = dot.s_addr;
				symp->s_tsym = tp;
			}
		} else {
			while (tp) {
				if (n == tp->t_num) {
					break;
				}
				tp = tp->t_lnk;
			}
			if (tp) {
				if (pass == 1) {
					fuzz = tp->t_addr - dot.s_addr;
					tp->t_area = dot.s_area;
					tp->t_addr = dot.s_addr;
				} else {
					phase(tp->t_area, tp->t_addr);
					if (tp->t_flg & S_MDF)
						err('m');
				}
			} else {
				err('u');
			}
		}
		goto loop;
	}
	if ((ctype[c] & LETTER) == 0)
		if (flevel) {
			return;
		} else {
			qerr();
		}
	getid(id, c);
	c = getnb();
	if (c == ':') {
		if (flevel)
			return;
		if ((c = get()) != ':') {
			unget(c);
			c = 0;
		}
		symp = lookup(id);
		if (symp == &dot)
			err('.');
		if (pass == 0)
			if ((symp->s_type != S_NEW) &&
			   ((symp->s_flag & S_ASG) == 0))
				symp->s_flag |= S_MDF;
		if (pass != 2) {
			fuzz = symp->s_addr - dot.s_addr;
			symp->s_type = S_USER;
			symp->s_area = dot.s_area;
			symp->s_addr = dot.s_addr;
		} else {
			if (symp->s_flag & S_MDF)
				err('m');
			phase(symp->s_area, symp->s_addr);
		}
		if (c) {
			symp->s_flag |= S_GBL;
		}
		lmode = ALIST;
		goto loop;
	}
	if (c == '=') {
		if (flevel)
			return;
		if ((c = get()) != '=') {
			unget(c);
			c = 0;
		}
		expr(&e1, 0);
		sp = lookup(id);
		if (sp == &dot) {
			outall();
			if (e1.e_flag || e1.e_base.e_ap != dot.s_area)
				err('.');
		} else
		if (sp->s_type != S_NEW && (sp->s_flag & S_ASG) == 0) {
			err('m');
		}
		sp->s_type = S_USER;
		sp->s_area = e1.e_base.e_ap;
		sp->s_addr = laddr = e1.e_addr;
		sp->s_flag |= S_ASG;
		if (c) {
			sp->s_flag |= S_GBL;
		}
		lmode = ALIST;
		goto loop;
	}
	unget(c);
	lmode = flevel ? SLIST : CLIST;
	if ((mp = mlookup(id)) == NULL) {
		if (!flevel)
			err('o');
		return;
	}
	switch (mp->m_type) {

	case S_IF:
		n = absexpr();
		if (tlevel < MAXIF) {
			++tlevel;
			ifcnd[tlevel] = n;
			iflvl[tlevel] = flevel;
			if (n == 0) {
				++flevel;
			}
		} else {
			err('i');
		}
		lmode = ALIST;
		laddr = n;
		return;

	case S_ELSE:
		if (ifcnd[tlevel]) {
			if (++flevel > (iflvl[tlevel]+1)) {
				err('i');
			}
		} else {
			if (--flevel < iflvl[tlevel]) {
				err('i');
			}
		}
		lmode = SLIST;
		return;

	case S_ENDIF:
		if (tlevel) {
			flevel = iflvl[tlevel--];
		} else {
			err('i');
		}
		lmode = SLIST;
		return;

	case S_PAGE:
		lop = NLPP;
		lmode = NLIST;
		return;

	default:
		break;
	}
	if (flevel)
		return;
	switch (mp->m_type) {

	case S_EVEN:
		outall();
		dot.s_addr = (dot.s_addr + 1) & ~1;
		lmode = SLIST;
		break;

	case S_ODD:
		outall();
		dot.s_addr |= 1;
		lmode = SLIST;
		break;

	case S_BYTE:
	case S_WORD:
		do {
			expr(&e1, 0);
			if (mp->m_type == S_BYTE) {
				outrb(&e1, R_NORM);
			} else {
				outrw(&e1, R_NORM);
			}
		} while ((c = getnb()) == ',');
		unget(c);
		break;


	case S_ASCII:
	case S_ASCIZ:
		if ((d = getnb()) == '\0')
			qerr();
		while ((c = getmap(d)) >= 0)
			outab(c);
		if (mp->m_type == S_ASCIZ)
			outab(0);
		break;

	case S_BLK:
		expr(&e1, 0);
		outall();
		dot.s_addr += e1.e_addr*mp->m_valu;
		break;

	case S_TITLE:
		p = tb;
		if ((c = getnb()) != 0) {
			do {
				if (p < &tb[NTITL-1])
					*p++ = c;
			} while ((c = get()) != 0);
		}
		*p = 0;
		unget(c);
		lmode = SLIST;
		break;

	case S_SBTL:
		p = stb;
		if ((c = getnb()) != 0) {
			do {
				if (p < &stb[NSBTL-1])
					*p++ = c;
			} while ((c = get()) != 0);
		}
		*p = 0;
		unget(c);
		lmode = SLIST;
		break;

	case S_MODUL:
		getid(id, -1);
		if (pass == 0) {
			if (module[0])
				err('m');
			strncpy(module, id, NCPS);
		}
		lmode = SLIST;
		break;

	case S_GLOBL:
		do {
			getid(id, -1);
			sp = lookup(id);
			sp->s_flag |= S_GBL;
		} while ((c = getnb()) == ',');
		unget(c);
		lmode = SLIST;
		break;

	case S_DAREA:
		getid(id, -1);
		uaf = 0;
		uf  = A_CON|A_REL;
		if ((c = getnb()) == '(') {
			do {
				getid(opt, -1);
				mp = mlookup(opt);
				if (mp && mp->m_type == S_ATYP) {
					++uaf;
					uf |= mp->m_valu;
				} else {
					err('u');
				}
			} while ((c = getnb()) == ',');
			if (c != ')')
				qerr();
		} else {
			unget(c);
		}
		if ((ap = alookup(id)) != NULL) {
			if (uaf && uf != ap->a_flag)
				err('m');
		} else {
			ap = (struct area *) new (sizeof(struct area));
			ap->a_ap = areap;
			strncpy(ap->a_id, id, NCPS);
			ap->a_ref = areap->a_ref + 1;
			ap->a_size = 0;
			ap->a_fuzz = 0;
			ap->a_flag = uaf ? uf : (A_CON|A_REL);
			areap = ap;
		}
		newdot(ap);
		lmode = SLIST;
		break;

	case S_ORG:
		if (dot.s_area->a_flag & A_ABS) {
			outall();
			dot.s_addr = absexpr();
		} else {
			err('o');
		}
		lmode = SLIST;
		break;

	case S_BOUND:
		/* 1.50c  Scott Lawrence  yorgle@gmail.com */
		if( dot.s_addr > absexpr() ) {
			err('z');
		}
		lmode = SLIST;
		break;

	case S_REPEAT:
		/* 1.50d  Scott Lawrence  yorgle@gmail.com */
		/* .REPEAT <VALUE>,<COUNT> */

		expr( &e1, 0 );
		c=getnb();
		if( c != ',' ) {
			unget(c);
			err('q');
		}
		expr( &e2, 0 );

		cnt = e2.e_addr;
		while( cnt > 0 ) {
			outrb(&e1, R_NORM);
			cnt --;
		}
		break;

	case S_FILL:
		/* 1.50d  Scott Lawrence  yorgle@gmail.com */
		/* .REPEAT <VALUE>,<ENDADDRESS> */
		expr( &e1, 0 );
		c=getnb();
		if( c != ',' ) {
			unget(c);
			err('q');
		}
		expr( &e2, 0 );

		cnt = e2.e_addr;
		while( dot.s_addr <= cnt ) {
			outrb(&e1, R_NORM);
		}
		break;

	case S_RADIX:
		if (more()) {
			switch (getnb()) {
			case 'b':
			case 'B':
				radix = 2;
				break;
			case '@':
			case 'o':
			case 'O':
			case 'q':
			case 'Q':
				radix = 8;
				break;
			case 'd':
			case 'D':
				radix = 10;
				break;
			case 'h':
			case 'H':
			case 'x':
			case 'X':
				radix = 16;
				break;
			default:
				radix = 10;
				qerr();
				break;
			}
		} else {
		radix = 10;
		}
		lmode = SLIST;
		break;

	case S_INCL:
		d = getnb();
		p = fn;
		while ((c = get()) != d) {
			if (p < &fn[FILSPC-1]) {
				*p++ = c;
			} else {
				break;
			}
		}
		*p = 0;
		if (++incfil == MAXINC ||
		   (ifp[incfil] = fopen(fn, "r")) == NULL) {
			--incfil;
			err('i');
			lmode = SLIST;
		} else {
			lop = NLPP;
			lmode = NLIST;
		}
		break;

	default:
		machine(mp);
	}
	goto loop;
}

FILE *
afile(fn, ft, wf)
char *fn;
char *ft;
int wf;
{
	register char *p1, *p2, *p3;
	register c;
	FILE *fp;
	char fb[FILSPC];

	p1 = fn;
	p2 = fb;
	p3 = ft;
	while ((c = *p1++) != 0 && c != FSEPX) {
		if (p2 < &fb[FILSPC-4])
			*p2++ = c;
	}
	*p2++ = FSEPX;
	if (*p3 == 0) {
		if (c == FSEPX) {
			p3 = p1;
		} else {
			p3 = dsft;
		}
	}
	while ((c = *p3++) != 0) {
		if (p2 < &fb[FILSPC-1])
			*p2++ = c;
	}
	*p2++ = 0;
	if ((fp = fopen(fb, wf?"w":"r")) == NULL) {
		fprintf(stderr, "%s: cannot %s.\n", fb, wf?"create":"open");
		exit(1);
	}
	return (fp);
}

VOID
newdot(nap)
register struct area *nap;
{
	register struct area *oap;

	oap = dot.s_area;
	oap->a_fuzz = fuzz;
	oap->a_size = dot.s_addr;
	fuzz = nap->a_fuzz;
	dot.s_area = nap;
	dot.s_addr = nap->a_size;
	outall();
}

VOID
phase(ap, a)
struct area *ap;
addr_t a;
{
	if (pjumpused && ap == dot.s_area && a == dot.s_addr + 1) {
		outab(0x0);
		err('j');
	}
	else if (ap != dot.s_area || a != dot.s_addr)
		err('p');
}

char *usetxt[] = {
	"Usage: [-dqxgaclsf] [-o ofile] file1 [file2 file3 ...]",
	"  d	decimal	listing",
	"  q	octal	listing",
	"  x	hex	listing (default)",
	"  g	undefined symbols made global",
	"  u	undefined symbols made local (!g)",
	"  a	all user symbols made global",
	"  c	symbols are case insensitive",
	"  l	create list   output file1[lst]",
	"  o	create object output ofile",
	"  s	create symbol output file1[sym]",
	"  f	flag relocatable references by  `   in listing file",
	"",
	0
};

VOID
usage()
{
	register char	**dp;

	fprintf(stderr, "\nASxxxx Assembler %s  (%s)\n\n", VERSION, cpu);
	for (dp = usetxt; *dp; dp++)
		fprintf(stderr, "%s\n", *dp);
	exit(1);
}
