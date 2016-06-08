/*	File function.c: 2.1 (83/03/20,16:02:04) */
/*% cc -O -c %
 *
 */

#include <stdio.h>
#include "defs.h"
#include "data.h"

/*
 *	begin a function
 *
 *	called from "parse", this routine tries to make a function out
 *	of what follows
 *	modified version.  p.l. woods
 *
 */
int argtop;
newfunc ()
{
	char	n[NAMESIZE], *ptr;
	int	i;
	fexitlab = getlabel();

	if (!symname (n) ) {
		error ("illegal function or declaration");
		kill ();
		return;
	}
	if (ptr = findglb (n)) {
		if (ptr[IDENT] != FUNCTION)
			multidef (n);
		else if (ptr[OFFSET] == FUNCTION)
			multidef (n);
		else
			ptr[OFFSET] = FUNCTION;
	} else
		addglb (n, FUNCTION, CINT, FUNCTION, PUBLIC);
	if (!match ("("))
		error ("missing open paren");
	prefix ();
	outstr (n);
	col ();
	nl ();
	prologue ();
	locptr = STARTLOC;
	argstk = 0;
	while (!match (")")) {
		if (symname (n)) {
			if (findloc (n))
				multidef (n);
			else {
				addloc (n, 0, 0, argstk, AUTO);
				argstk = argstk + intsize();
			}
		} else {
			error ("illegal argument name");
			junk ();
		}
		blanks ();
		if (!streq (line + lptr, ")")) {
			if (!match (","))
				error ("expected comma");
		}
		if (endst ())
			break;
	}
	stkp = 0;
	argtop = argstk;
	while (argstk) {
		if (amatch ("register", 8)) {
			if (amatch("char", 4)) 
				getarg(CCHAR);
			else if (amatch ("int", 3))
				getarg(CINT);
			else
				getarg(CINT);
			ns();
		} else if (amatch ("char", 4)) {
			getarg (CCHAR);
			ns ();
		} else if (amatch ("int", 3)) {
			getarg (CINT);
			ns ();
		} else {
			error ("wrong number args");
			break;
		}
	}
	i = frameix ? frame() : 0;
	statement(YES);
	printlabel(fexitlab);
	col();
	nl();
	modstk (-i);		/* don't adjust for frame pointer here */
	gret ();
	stkp = 0;
	locptr = STARTLOC;
}

/*
 *	declare argument types
 *
 *	called from "newfunc", this routine add an entry in the local
 *	symbol table for each named argument
 *	completely rewritten version.  p.l. woods
 *
 */
getarg (t)
int	t;
{
	int	j, legalname, address;
	char	n[NAMESIZE], *argptr;

	FOREVER {
		if (argstk == 0)
			return;
		if (match ("*"))
			j = POINTER;
		else
			j = VARIABLE;
		if (!(legalname = symname (n)))
			illname ();
		if (match ("[")) {
			while (inbyte () != ']')
				if (endst ())
					break;
			j = POINTER;
		}
		if (legalname) {
			if (argptr = findloc (n)) {
				argptr[IDENT] = j;
				argptr[TYPE] = t;
				address = argtop - glint(argptr);
				if (t == CCHAR && j == VARIABLE)
					address = address + byteoff();
				argptr[OFFSET] = (address) & 0xff;
				argptr[OFFSET + 1] = (address >> 8) & 0xff;
			} else
				error ("expecting argument name");
		}
		argstk = argstk - intsize();
		if (endst ())
			return;
		if (!match (","))
			error ("expected comma");
	}
}
