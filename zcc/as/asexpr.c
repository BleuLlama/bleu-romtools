/* asexpr.c */

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
#ifndef __APPLE__
#include <alloc.h>
#endif
#include "asm.h"

/*
 * Read an expression. The value of the
 * expression and its associated relocation
 * information is stored into the `expr'
 * structure supplied by the user. `N' is
 * a firewall priority; all top level calls
 * (from the user) should be made with `n'
 * set to 0.
 */
VOID
expr(esp, n)
int n;
register struct expr *esp;
{
	register c, p;
	struct area *ap;
	struct expr re;

	term(esp);
	while (ctype[c = getnb()] & BINOP) {
		if ((p = oprio(c)) <= n)
			break;
		if ((c == '>' || c == '<') && c != get())
			qerr();
		expr(&re, p);
		if (c == '+') {
			if (esp->e_base.e_ap == NULL) {
				esp->e_flag = re.e_flag;
				esp->e_base.e_ap = re.e_base.e_ap;
			} else
			if (re.e_base.e_ap) {
				rerr();
			}
			esp->e_addr += re.e_addr;
		} else
		if (c == '-') {
			if ((ap = re.e_base.e_ap) != NULL) {
				if (esp->e_base.e_ap == ap) {
					esp->e_base.e_ap = NULL;
				} else {
					rerr();
				}
				if (esp->e_flag || re.e_flag) {
					rerr();
				}
			}
			esp->e_addr -= re.e_addr;
		} else {
			abscheck(esp);
			abscheck(&re);
			switch (c) {

			case '*':
				esp->e_addr *= re.e_addr;
				break;

			case '/':
				esp->e_addr /= re.e_addr;
				break;

			case '&':
				esp->e_addr &= re.e_addr;
				break;

			case '|':
				esp->e_addr |= re.e_addr;
				break;

			case '%':
				esp->e_addr %= re.e_addr;
				break;

			case '^':
				esp->e_addr ^= re.e_addr;
				break;

			case '<':
				esp->e_addr <<= re.e_addr;
				break;

			case '>':
				esp->e_addr >>= re.e_addr;
				break;
			}
		}
	}
	unget(c);
}

/*
 * Read an absolute expression.
 */
addr_t
absexpr()
{
	struct expr e;

	expr(&e, 0);
	abscheck(&e);
	return (e.e_addr);
}

/*
 * Read a term.
 * Handles unary operators, brackets,
 * constants in decimal, octal or hexadecimal
 * and identifiers. This routine is also
 * responsible for setting the relocation type
 * to symbol based (e.flag != 0) on global
 * references.
 */
VOID
term(esp)
register struct expr *esp;
{
	register c, n;
	register char *jp;
	char id[NCPS];
	struct sym  *sp;
	struct tsym *tp;
	int r, v;

	c = getnb();
	if (c == '#') { c = getnb(); }
	if (c == LFTERM) {
		expr(esp, 0);
		if (getnb() != RTTERM)
			qerr();
		return;
	}
	if (c == '-') {
		expr(esp, 100);
		abscheck(esp);
		esp->e_addr = -esp->e_addr;
		return;
	}
	if (c == '~') {
		expr(esp, 100);
		abscheck(esp);
		esp->e_addr = ~esp->e_addr;
		return;
	}
	if (c == '\'') {
		esp->e_mode = S_USER;
		esp->e_flag = 0;
		esp->e_base.e_ap = NULL;
		esp->e_addr = getmap(-1)&0377;
		return;
	}
	if (c == '\"') {
		esp->e_mode = S_USER;
		esp->e_flag = 0;
		esp->e_base.e_ap = NULL;
		if (hilo) {
		    esp->e_addr  = (getmap(-1)&0377)<<8;
		    esp->e_addr |= (getmap(-1)&0377);
		} else {
		    esp->e_addr  = (getmap(-1)&0377);
		    esp->e_addr |= (getmap(-1)&0377)<<8;
		}
		return;
	}
	if (c == '>' || c == '<') {
		expr(esp, 100);
		abscheck(esp);
		if (c == '>')
			esp->e_addr >>= 8;
		esp->e_addr &= 0377;
		return;
	}
	if (ctype[c] & DIGIT) {
		esp->e_mode = S_USER;
		esp->e_flag = 0;
		esp->e_base.e_ap = NULL;
		jp = ip;
		while (ctype[*jp] & RAD10) {
			jp++;
		}
		if (*jp == '$') {
			n = 0;
			while ((v = digit(c, 10)) >= 0) {
				n = 10*n + v;
				c = get();
			}
			tp = symp->s_tsym;
			while (tp) {
				if (n == tp->t_num) {
					esp->e_base.e_ap = tp->t_area;
					esp->e_addr = tp->t_addr;
					return;
				}
				tp = tp->t_lnk;
			}
			err('u');
			esp->e_addr = 0;
			return;
		}
		r = radix;
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
		n = 0;
		while ((v = digit(c, r)) >= 0) {
			n = r*n + v;
			c = get();
		}
		unget(c);
		esp->e_addr = n;
		return;
	}
	if (ctype[c] & LETTER) {
		esp->e_mode = S_USER;
		esp->e_flag = 0;
		esp->e_base.e_ap = NULL;
		esp->e_addr = 0;
		getid(id, c);
		sp = lookup(id);
		if (sp->s_type == S_NEW) {
			if (sp->s_flag&S_GBL) {
				esp->e_flag = 1;
				esp->e_base.e_sp = sp;
				return;
			}
			err('u');
		} else {
			esp->e_mode = sp->s_type;
			esp->e_addr = sp->s_addr;
			esp->e_base.e_ap = sp->s_area;
		}
		return;
	}
	qerr();
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
 * Check if the value of the supplied
 * expression is absolute; if not give
 * a relocation error and force the
 * type to absolute.
 */
VOID
abscheck(esp)
register struct expr *esp;
{
	if (esp->e_flag || esp->e_base.e_ap) {
		esp->e_flag = 0;
		esp->e_base.e_ap = NULL;
		rerr();
	}
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
