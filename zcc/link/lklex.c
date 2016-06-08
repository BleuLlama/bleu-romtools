/* lklex.c */

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
getid(id, c)
register c;
char *id;
{
	register char *p;

	if (c < 0) {
		c = getnb();
	}
	p = id;
	do {
		if (p < &id[NCPS])
			*p++ = c;
	} while (ctype[c=get()] & (LETTER|DIGIT));
	unget(c);
	while (p < &id[NCPS])
		*p++ = 0;
}

VOID
getfid(str, c)
register c;
char *str;
{
	register char *p;

	if (c < 0)
		c = getnb();
	p = str;
	while (ctype[c] & (LETTER|DIGIT) || c == FSEPX) {
		if (p < &str[FILSPC-1])
			*p++ = c;
		c = get();
	}
	unget(c);
	while (p < &str[FILSPC])
		*p++ = 0;
}

char
getnb()
{
	register c;

	while ((c=get())==' ' || c=='\t')
		;
	return (c);
}

VOID
skip(c)
register c;
{
	if (c < 0)
		c = getnb();
	while (ctype[c=get()] & (LETTER|DIGIT)) { ; }
	unget(c);
}

char
get()
{
	register c;

	if ((c = *ip) != 0)
		++ip;
	return (c);
}

VOID
unget(c)
int c;
{
	if (c != 0)
		--ip;
}

int
getmap(d)
int d;
{
	register c, n, v;

	if ((c = get()) == '\0')
		return (-1);
	if (c == d)
		return (-1);
	if (c == '\\') {
		c = get();
		switch (c) {

		case 'b':
			c = '\b';
			break;

		case 'f':
			c = '\f';
			break;

		case 'n':
			c = '\n';
			break;

		case 'r':
			c = '\r';
			break;

		case 't':
			c = '\t';
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			n = 0;
			v = 0;
			while (++n<=3 && c>='0' && c<='7') {
				v = (v<<3) + c - '0';
				c = get();
			}
			unget(c);
			c = v;
			break;
		}
	}
	return (c);
}

int
asgetline()
{
	register i, ftype;
	register char *fid;

loop:	if (pflag && cfp && cfp->f_type == F_STD)
		fprintf(stdout, "ASlink >> ");

	if (sfp == NULL || fgets(ib, sizeof ib, sfp) == NULL) {
		if (sfp) {
			fclose(sfp);
		}
		if (cfp == NULL) {
			cfp = filep;
		} else {
			cfp = cfp->f_flp;
		}
		if (cfp) {
			ftype = cfp->f_type;
			fid = cfp->f_idp;
			if (ftype == F_STD) {
				sfp = stdin;
			} else
			if (ftype == F_LNK) {
				sfp = afile(fid, "lnk", 0);
			} else
			if (ftype == F_REL) {
				sfp = afile(fid, NULL, 0);
			} else {
				fprintf(stderr, "Invalid file type\n");
				exit(1);
			}
			goto loop;
		} else {
			filep = NULL;
			return(0);
		}
	}
	i = strlen(ib) - 1;
	if (ib[i] == '\n')
		ib[i] = 0;
	return (1);
}

int
more()
{
	register c;

	c = getnb();
	unget(c);
	return( (c == '\0' || c == ';') ? 0 : 1 );
}

char
endline()
{
	register c;

	c = getnb();
	return( (c == '\0' || c == ';') ? 0 : c );
}
