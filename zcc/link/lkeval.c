/* lkeval.c */

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
 * Evaluate input term.
 */
addr_t
eval()
{
	register c, v;
	register addr_t n;

	c = getnb();
	n = 0;
	while ((v = digit(c, radix)) >= 0) {
		n = n*radix + v;
		c = get();
	}
	unget(c);
	return(n);
}

/*
 * Expression evaluation.
 * `N' is a firewall priority; all top level calls
 * (from the user) should be made with `n' set to 0.
 */
addr_t
expr (n)
int n;
{
	register c, p;
	register addr_t v, ve;

	v = term();
	while (ctype[c = getnb()] & BINOP) {
		if ((p = oprio(c)) <= n)
			break;
		if ((c == '>' || c == '<') && c != get()) {
			fprintf(stderr, "Invalid expression");
			return(v);
		}
		ve = expr(p);
		if (c == '+') {
			v += ve;
		} else
		if (c == '-') {
			v -= ve;
		} else {
			switch (c) {

			case '*':
				v *= ve;
				break;

			case '/':
				v /= ve;
				break;

			case '&':
				v &= ve;
				break;

			case '|':
				v |= ve;
				break;

			case '%':
				v %= ve;
				break;

			case '^':
				v ^= ve;
				break;

			case '<':
				v <<= ve;
				break;

			case '>':
				v >>= ve;
				break;
			}
		}
	}
	unget(c);
	return(v);
}

/*
 * Read a term.
 * Handles unary operators, brackets,
 * constants in decimal, octal or hexadecimal
 * and identifiers.
 */
addr_t
term()
{
	register c, r, n;
	register addr_t v;
	struct sym *sp;
	char id[NCPS];

	c = getnb();
	if (c == '#') { c = getnb(); }
	if (c == '(') {
		v = expr(0);
		if (getnb() != ')')
			fprintf(stderr, "Missing delimiter");
		return(v);
	}
	if (c == '-') {
		return(-expr(100));
	}
	if (c == '~') {
		return(~expr(100));
	}
	if (c == '\'') {
		return(getmap(-1)&0377);
	}
	if (c == '\"') {
		if (hilo) {
			v  = (getmap(-1)&0377)<<8;
			v |=  getmap(-1)&0377;
		} else {
			v  =  getmap(-1)&0377;
			v |= (getmap(-1)&0377)<<8;
		}
		return(v);
	}
	if (c == '>' || c == '<') {
		v = expr(100);
		if (c == '>')
			v >>= 8;
		return(v&0377);
	}
	if (ctype[c] & DIGIT) {
		r = 10;
		if (c == '0') {
			c = get();
			switch (c) {
			case 'b':
			case 'B':
				r = 2;
				c = get();
				break;
			case '@':
			case 'o':
			case 'O':
			case 'q':
			case 'Q':
				r = 8;
				c = get();
				break;
			case 'd':
			case 'D':
				r = 10;
				c = get();
				break;
			case 'h':
			case 'H':
			case 'x':
			case 'X':
				r = 16;
				c = get();
				break;
			default:
				break;
			}
		}
		v = 0;
		while ((n = digit(c, r)) >= 0) {
			v = r*v + n;
			c = get();
		}
		unget(c);
		return(v);
	}
	if (ctype[c] & LETTER) {
		getid(id, c);
		if ((sp = lkpsym(id, 0)) == NULL) {
			fprintf(stderr, "Undefined symbol %8s\n", id);
			return(0);
		} else {
			return(symval(sp));
		}
	}
}

/*
 * If `c' is a legal radix `r' digit
 * return its value; otherwise return
 * -1.
 */
int
digit(c, r)
register c, r;
{
	if (r == 16) {
		if (ctype[c] & RAD16) {
			if (c >= 'A' && c <= 'F')
				return (c - 'A' + 10);
			if (c >= 'a' && c <= 'f')
				return (c - 'a' + 10);
			return (c - '0');
		}
	} else
	if (r == 10) {
		if (ctype[c] & RAD10)
			return (c - '0');
	} else
	if (r == 8) {
		if (ctype[c] & RAD8)
			return (c - '0');
	} else
	if (r == 2) {
		if (ctype[c] & RAD2)
			return (c - '0');
	}
	return (-1);
}

/*
 * Return the priority of the binary
 * operator `c'.
 */
int
oprio(c)
register c;
{
	if (c == '*' || c == '/' || c == '%')
		return (10);
	if (c == '+' || c == '-')
		return (7);
	if (c == '<' || c == '>')
		return (5);
	if (c == '^')
		return (4);
	if (c == '&')
		return (3);
	if (c == '|')
		return (1);
	return (0);
}
