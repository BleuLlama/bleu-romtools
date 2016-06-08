/* lksym.c */

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
 * This routine is called early in the
 * game to set up the symbol hashtable.
 */
VOID
syminit()
{
	struct sym **spp;

	spp = &symhash[0];
	while (spp < &symhash[NHASH])
		*spp++ = NULL;
}

/*
 * Find/Create a global symbol entry.
 *
 * S xxxxxx Defnnnn
 *   |      |  |
 *   |      |  `-- sp->s_addr
 *   |      `----- sp->s_type
 *   `------------ sp->s_id
 *
 */
struct sym *
newsym()
{
	register c, i, nglob;
	struct sym *tsp;
	struct sym **s;
	char id[NCPS];

	getid(id, -1);
	tsp = lkpsym(id, 1);
	c = getnb();get();get();
	if (c == 'R') {
		tsp->s_type |= S_REF;
		if (eval())
			fprintf(stderr, "Non zero S_REF\n");
	} else
	if (c == 'D') {
		i = eval();
		if (tsp->s_type & S_DEF && tsp->s_addr != i)
			fprintf(stderr, "Multiple definition of %.8s\n", id);
		tsp->s_type |= S_DEF;
		/*
		 * Set value and area extension link.
		 */
		tsp->s_addr = i;
		tsp->s_axp = axp;
	} else {
		fprintf(stderr, "Invalid symbol type %c for %.8s\n", c, id);
		exit(1);
	}
	/*
	 * Place pointer in header symbol list
	 */
	if (headp == NULL) {
		fprintf(stderr, "No header defined\n");
		exit(1);
	}
	nglob = hp->h_nglob;
	s = hp->s_list;
	for (i=0; i < nglob ;++i) {
		if (s[i] == NULL) {
			s[i] = tsp;
			return(tsp);
		}
	}
	fprintf(stderr, "Header symbol list overflow\n");
	exit(1);
}

/*
 * Lookup the name `id' in the hashtable.
 * If it is not found either return a
 * `NULL' (`f' is false) or a
 * pointer to a newly created hash table
 * entry (`f' is true).
 */
struct sym *
lkpsym(id, f)
char *id;
int f;
{
	register struct sym *sp;
	register h;

	h = hash(id);
	sp = symhash[h];
	while (sp != NULL) {
		if (symeq(id, sp->s_id))
			return (sp);
		sp = sp->s_sp;
	}
	if (f == 0)
		return (NULL);
	sp = (struct sym *) new (sizeof(struct sym));
	sp->s_sp = symhash[h];
	symhash[h] = sp;
	strncpy(sp->s_id, id, NCPS);
	return (sp);
}

/*
 * Get value of relocated symbol
 */
addr_t
symval(tsp)
register struct sym *tsp;
{
	register addr_t val;

	val = tsp->s_addr;
	if (tsp->s_axp) {
		val += tsp->s_axp->a_addr;
	}
	return(val);
}

/*
 * Check for undefined symbols
 */
VOID
symdef(fp)
FILE *fp;
{
	register struct sym *sp;
	register i;

	for (i=0; i<NHASH; ++i) {
		sp = symhash[i];
		while (sp) {
			if (sp->s_axp == NULL)
				sp->s_axp = areap->a_axp;
			if ((sp->s_type & S_DEF) == 0)
				symmod(fp, sp);
			sp = sp->s_sp;
		}
	}
}

VOID
symmod(fp, tsp)
FILE *fp;
struct sym *tsp;
{
	register int i;
	struct sym **p;

	if ((hp = headp) != NULL) {
	    while(hp) {
		p = hp->s_list;
		for (i=0; i<hp->h_nglob; ++i) {
		    if (p[i] == tsp) {
			fprintf(fp, "\n?ASlink-W-Undefined Global %8.8s ", tsp->s_id);
			fprintf(fp, "referenced by module %8.8s\n", hp->m_id);
		    }
		}
	    hp = hp->h_hp;
	    }
	}
}

/*
 * Compare two symbol names.
 */
int
symeq(p1, p2)
register char *p1, *p2;
{
	register n;

	n = NCPS;
	do {

#if	CASE_SENSITIVE
		if (*p1++ != *p2++)
			return (0);
#else
		if (ccase[*p1++] != ccase[*p2++])
			return (0);
#endif

	} while (--n);
	return (1);
}

/*
 * Given a pointer to a symbol name
 * compute and return the hash table
 * bucket.
 * The `sum of all the characters mod
 * table size' algorithm is perhaps
 * not the best.
 */
int
hash(p)
register char *p;
{
	register h, n;

	h = 0;
	n = NCPS;
	do {

#if	CASE_SENSITIVE
		h += *p++;
#else
		h += ccase[*p++];
#endif

	} while (--n);
	return (h&HMASK);
}

/*
 * Allocate and clear a block of space.
 * Leave if there is no space left
 * at all.
 */
VOID *
new(n)
unsigned int n;
{
	register char *p,*q;
	register unsigned int i;

	if ((p = (char *) malloc(n)) == NULL) {
		fprintf(stderr, "Out of space!\n");
		exit(1);
	}
	for (i=0,q=p; i<n; i++) {
		*q++ = 0;
	}
	return (p);
}
