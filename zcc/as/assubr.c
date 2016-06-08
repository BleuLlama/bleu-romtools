/* assubr.c */

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
 * Note an error.
 * If the error is already in the
 * error buffer don't note it again.
 * If the error is serious enough to
 * stop the parse (a `q' error) just
 * give up.
 */
VOID
err(c)
register c;
{
	register char *p;

	p = eb;
	while (p < ep)
		if (*p++ == c)
			return;
	if (p < &eb[NERR]) {
		*p++ = c;
		ep = p;
	}
	if (c == 'q')
		longjmp(jump_env, -1);
}

static char *errors[] =
{
"(.) Absolute direct assignment of the current location counter incorrect",
"(a) Machine specific addressing or addressing mode error",
"(b) Direct page boundary error",
"(d) Direct page addressing error",
"(i) .include file error or an .if/.endif mismatch",
"(m) Multiple definitions error",
"(o) Directive or mnemonic error\n\tor the use of the .org directive in a relocatable area",
"(p) Phase error: label location changing between passes 2 and 3",
"(q) Questionable syntax: missing or improper operators,\n\tterminators, or delimiters",
"(r) Relocation error",
"(u) Undefined symbol encountered during assembly",
"(j) NOP inserted to handle forward pseudojump",
"(z) Boundary byte reached\n\tAdjust code accordingly",
NULL,
};

char *
geterror(int c)
{
	int	i;

	for (i = 0; errors[i] != NULL; i++) {
		if (c == errors[i][1]) {
			return errors[i];
		}
	}
	return NULL;
}

/*
 * Send errors to the standard output.
 */
VOID
diag()
{
	register char *p;
	char	*e;

	if (eb != ep) {
		fprintf(stderr, "Assembler Error: line %04d\n", line);
		p = eb;
		while (p < ep) {
			e = geterror(*p);
			if (e == NULL) {
				fprintf(stderr, "    Unknown error (%c)\n", *p);
			}
			else {
				fprintf(stderr, "    %s\n", e);
			}
			p++;
		}
	}
}

/*
 * Note an 'r' error.
 */
VOID
rerr()
{
	err('r');
}

/*
 * Note an 'a' error.
 */
VOID
aerr()
{
	err('a');
}

/*
 * Note a 'q' error.
 */
VOID
qerr()
{
	err('q');
}
