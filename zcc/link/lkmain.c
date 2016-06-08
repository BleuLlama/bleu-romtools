/* lkmain.c */

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

char ofile[FILSPC];

int
main(argc, argv)
int argc;
char *argv[];
{
	register char *p;
	char *s;
	register c, i;
	FILE *afile();

	pflag = 0;
	oflag = 1;
	strcpy(ofile, "a");

	for (i=1; i<argc; ++i)
	{
	    p = argv[i];
	    if (*p == '-') {
		c = *(++p);
		switch(c) {
			case 'c':
				ccase = icase;
				break;

			case 'i':
				oflag = 1;
				break;

			case 's':
				oflag = 2;
				break;

			case 'm':
				mflag = 1;
				break;

			case 'x':
				xflag = 0;
				break;
			case 'q':
				mflag = 1;
				break;
			case 'd':
				xflag = 2;
				break;

			case 'b':
				if (*(p+1) != '\0')
				{
					bassav(p+1);
				}
				else
				{
					bassav(argv[++i]);
				}
				break;

			case 'g':
				if (*(p+1) != '\0')
				{
					gblsav(p+1);
				}
				else
				{
					gblsav(argv[++i]);
				}
				break;

			case 'o':
				if (*(p+1) != '\0')
				{
					strcpy(ofile, p+1);
				}
				else
				{
					strcpy(ofile, argv[++i]);
				}
				/* remove trailing .ihx if is exists */
				s = strstr(ofile, ".ihx");
				if (s)
				    	*s = '\0';

				break;

			case 'l':
			case 'L':	/* for future expansion of libraries */
				break;

			default:
				usage();
		    }
		}
		else
		{
			if (linkp == NULL) {
				linkp = (struct lfile *)
						new (sizeof (struct lfile));
				lfp = linkp;
			} else {
				lfp->f_flp = (struct lfile *)
						new (sizeof (struct lfile));
				lfp = lfp->f_flp;
			}
			lfp->f_idp = (char *) new (strlen(p)+1);
			strcpy(lfp->f_idp, p);
			lfp->f_type = F_REL;
		}
	}

	syminit();
	for (pass=0; pass<2; ++pass) {
		cfp = NULL;
		sfp = NULL;
		filep = linkp;
		hp = NULL;
		radix = 10;

		while (asgetline()) {
			ip = ib;
			link();
		}
		if (pass == 0) {
			/*
			 * Set area base addresses.
			 */
			setbas();
			/*
			 * Link all area addresses.
			 */
			lnkarea();
			/*
			 * Process global definitions.
			 */
			setgbl();
			/*
			 * Check for undefined globals.
			 */
			symdef(stderr);
			/*
			 * Output Link Map.
			 */
			if (mflag)
				map();
			/*
			 * Open output file
			 */
			if (oflag == 1)
				ofp = afile(ofile, "ihx", 1);
			if (oflag == 2)
				ofp = afile(ofile, "s19", 1);
		} else {
			reloc('E');
		}
	}

	return 0;
}

VOID
link()
{
	register c;

	if ((c=endline()) == 0) { return; }
	switch (c) {

	case 'X':
		radix = 16;
		break;

	case 'D':
		radix = 10;
		break;

	case 'Q':
		radix = 8;
		break;

	case 'H':
		if (pass == 0) {
			newhead();
		} else {
			if (hp == 0) {
				hp = headp;
			} else {
				hp = hp->h_hp;
			}
		}
		sdp.s_area = NULL;
		sdp.s_areax = NULL;
		sdp.s_addr = 0;
		break;

	case 'M':
		if (pass == 0)
			module();
		break;

	case 'A':
		if (pass == 0)
			newarea();
		if (sdp.s_area == NULL) {
			sdp.s_area = areap;
			sdp.s_areax = areap->a_axp;
			sdp.s_addr = 0;
		}
		break;

	case 'S':
		if (pass == 0)
			newsym();
		break;

	case 'T':
	case 'R':
	case 'P':
		if (pass == 0)
			break;
		reloc(c);
		break;

	default:
		break;
	}
	if (c == 'X' || c == 'D' || c == 'Q') {
		if ((c = get()) == 'H') {
			hilo = 1;
		} else
		if (c == 'L') {
			hilo = 0;
		}
	}
}

VOID
map()
{
	register i;
	register struct head *hdp;

	/*
	 * Open Map File
	 */
	mfp = afile(ofile, "map", 1);

	/*
	 * Output Map Area Lists
	 */
	page = 0;
	ap = areap;
	while (ap) {
		lstarea(ap);
		ap = ap->a_ap;
	}
	/*
	 * List Linked Files
	 */
	fprintf(mfp, "\nFiles Linked      [ module(s) ]\n\n");
	hdp = headp;
	filep = linkp;
	while (filep) {
		fprintf(mfp, "%-16s", filep->f_idp);
		i = 0;
		while ((hdp != NULL) && (hdp->h_lfile == filep)) {
			if (i % 5) {
			    fprintf(mfp, ", %8.8s", hdp->m_id);
			} else {
			    if (i) {
				fprintf(mfp, ",\n%20s%8.8s", "", hdp->m_id);
			    } else {
				fprintf(mfp, "  [ %8.8s", hdp->m_id);
			    }
			}
			hdp = hdp->h_hp;
			i++;
		}
		if (i)
			fprintf(mfp, " ]");
		fprintf(mfp, "\n");
		filep = filep->f_flp;
	}
	/*
	 * List Base Address Definitions
	 */
	if (basep) {
		fprintf(mfp, "\nUser Base Address Definitions\n\n");
		bsp = basep;
		while (bsp) {
			fprintf(mfp, "%s\n", bsp->b_strp);
			bsp = bsp->b_base;
		}
	}
	/*
	 * List Global Definitions
	 */
	if (globlp) {
		fprintf(mfp, "\nUser Global Definitions\n\n");
		gsp = globlp;
		while (gsp) {
			fprintf(mfp, "%s\n", gsp->g_strp);
			gsp = gsp->g_globl;
		}
	}
	fprintf(mfp, "\n");
	symdef(mfp);
}

/*
 * Base string save
 */
VOID
bassav(value)
char	*value;
{
	if (basep == NULL) {
		basep = (struct base *)
			new (sizeof (struct base));
		bsp = basep;
	} else {
		bsp->b_base = (struct base *)
				new (sizeof (struct base));
		bsp = bsp->b_base;
	}
	bsp->b_strp = (char *) new (strlen(value)+1);
	strcpy(bsp->b_strp, value);
}

VOID
setbas()
{
	register v;
	char id[NCPS];

	bsp = basep;
	while (bsp) {
		ip = bsp->b_strp;
		getid(id, -1);
		if (getnb() == '=') {
			v = expr(0);
			for (ap = areap; ap != NULL; ap = ap->a_ap) {
				if (symeq(id, ap->a_id))
					break;
			}
			if (ap == NULL) {
				fprintf(stderr,
				"No definition of area %s\n", id);
			} else {
				ap->a_addr = v;
			}
		} else {
			fprintf(stderr, "No '=' in base expression");
		}
		bsp = bsp->b_base;
	}
}

/*
 * Global string save
 */
VOID
gblsav(value)
char *value;
{
	if (globlp == NULL) {
		globlp = (struct globl *)
			new (sizeof (struct globl));
		gsp = globlp;
	} else {
		gsp->g_globl = (struct globl *)
				new (sizeof (struct globl));
		gsp = gsp->g_globl;
	}
	gsp->g_strp = (char *) new (strlen(value)+1);
	strcpy(gsp->g_strp, value);
}
	
VOID
setgbl()
{
	register v;
	register struct sym *sp;
	char id[NCPS];

	gsp = globlp;
	while (gsp) {
		ip = gsp->g_strp;
		getid(id, -1);
		if (getnb() == '=') {
			v = expr(0);
			sp = lkpsym(id, 0);
			if (sp == NULL) {
				fprintf(stderr,
				"No definition of symbol %s\n", id);
			} else {
				if (sp->s_flag & S_DEF) {
					fprintf(stderr,
					"Redefinition of symbol %s\n", id);
					sp->s_axp = NULL;
				}
				sp->s_addr = v;
				sp->s_type |= S_DEF;
			}
		} else {
			fprintf(stderr, "No '=' in global expression");
		}
		gsp = gsp->g_globl;
	}
}

FILE *
afile(fn, ft, wf)
char *fn;
char *ft;
int wf;
{
	FILE *fp;
	char fb[FILSPC];

	strcpy(fb, fn);
	if (ft != NULL)
	{
		strcat(fb, ".");
		strcat(fb, ft);
	}
	if ((fp = fopen(fb, wf?"w":"r")) == NULL) {
		fprintf(stderr, "%s: cannot %s.\n", fb, wf?"create":"open");
		exit(1);
	}
	return (fp);
}

char *usetxt[] = {
	"Usage: [-Options] file [file ...]",
	"  -c   symbols are case insensitive",
	"Relocation:",
	"  -b   area base address = expression",
	"  -g   global symbol = expression",
	"Map format:",
	"  -m   Map output generated as file[map]",
	"  -x   Hexadecimal (default)",
	"  -d   Decimal",
	"  -q   Octal",
	"Output:",
	"  -i   Intel Hex as file[ihx]",
	"  -s   Motorola S19 as file[s19]",
	"",
	0
};

VOID
usage()
{
	register char	**dp;

	fprintf(stderr, "\nASxxxx Linker %s\n\n", VERSION);
	for (dp = usetxt; *dp; dp++)
		fprintf(stderr, "%s\n", *dp);
	exit(1);
}
