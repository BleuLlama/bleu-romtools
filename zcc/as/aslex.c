/* aslex.c */

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

VOID
getid(id, c)
register c;
char *id;
{
	register char *p;

	if (c < 0) {
		c = getnb();
		if ((ctype[c] & LETTER) == 0)
			qerr();
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

int
getnb()
{
	register c;

	while ((c=get()) == ' ' || c == '\t')
		;
	return (c);
}

int
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
	if (c)
		--ip;
}

int
getmap(d)
int d;
{
	register int c, n, v;

	if ((c=get()) == '\0')
		qerr();
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
register i;

loop:	if (incfil >= 0) {
		if (fgets(ib, sizeof ib, ifp[incfil]) == NULL) {
			fclose(ifp[incfil--]);
			lop = NLPP;
			goto loop;
		}
	} else
	if (fgets(ib, sizeof ib, sfp[cfile]) == NULL) {
		if (++cfile <= inpfil)
			goto loop;
		return (0);
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

int
endline()
{
	register c;

	c = getnb();
	return( (c == '\0' || c == ';') ? 0 : c );
}
