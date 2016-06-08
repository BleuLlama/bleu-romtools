/* assym.c */

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
 * This routine is called early in the
 * game to set up the hashtables. First
 * all buckets in a table are cleared.
 * Then a pass is made through the respective
 * symbol lists, linking them into their hash
 * buckets. Finally the area list and
 * 'dca' are set up.
 */
VOID
syminit()
{
	register struct mne  *mp;
	struct mne **mpp;
	register struct sym  *sp;
	struct sym **spp;
	register h;

	mpp = &mnehash[0];
	while (mpp < &mnehash[NHASH])
		*mpp++ = NULL;
	mp = &mne[0];
	for (;;) {
		h = hash(mp->m_id);
		mp->m_mp = mnehash[h];
		mnehash[h] = mp;
		if (mp->m_flag&S_END)
			break;
		++mp;
	}

	spp = &symhash[0];
	while (spp < &symhash[NHASH])
		*spp++ = NULL;
	sp = &sym[0];
	for (;;) {
		h = hash(sp->s_id);
		sp->s_sp = symhash[h];
		symhash[h] = sp;
		if (sp->s_flag&S_END)
			break;
		++sp;
	}

	areap = &dca;
}

/*
 * Lookup the area `id'.
 */
struct area *
alookup(id)
char *id;
{
	register struct area *ap;

	ap = areap;
	while (ap) {
		if (symeq(id, ap->a_id)) {
			return (ap);
		}
		ap = ap->a_ap;
	}
	return(NULL);
}

/*
 * Lookup the mnemonic (or directive) `id' in the hashtable.
 * If it is not found return a NULL.
 */
struct mne *
mlookup(id)
char *id;
{
	register struct mne *mp;
	register h;

	h = hash(id);
	mp = mnehash[h];
	while (mp) {
		if (symeq(id, mp->m_id))
			return (mp);
		mp = mp->m_mp;
	}
	return (NULL);
}

/*
 * Lookup the label `id' in the hashtable.
 * If it is not found return a
 * pointer to a newly created hash table
 * entry.
 */
struct sym *
lookup(id)
char *id;
{
	register struct sym *sp;
	register h;

	h = hash(id);
	sp = symhash[h];
	while (sp) {
		if (symeq(id, sp->s_id))
			return (sp);
		sp = sp->s_sp;
	}
	sp = (struct sym *) new (sizeof(struct sym));
	sp->s_sp = symhash[h];
	symhash[h] = sp;
	sp->s_tsym = NULL;
	strncpy(sp->s_id, id, NCPS);
	sp->s_type = S_NEW;
	sp->s_flag = 0;
	sp->s_area = NULL;
	sp->s_ref = 0;
	sp->s_addr = 0;
	return (sp);
}

/*
 * Mark all symbols of type `S_NEW' global.
 * Called at the beginning of pass 1 if '-g'.
 */
VOID
symglob()
{
	register struct sym *sp;
	register i;

	for (i=0; i<NHASH; ++i) {
		sp = symhash[i];
		while (sp != NULL) {
			if (sp->s_type == S_NEW)
				sp->s_flag |= S_GBL;
			sp = sp->s_sp;
		}
	}
}

/*
 * Mark all symbols of type `S_USER' global.
 * Called at the beginning of pass 1 if '-a'.
 */
VOID
allglob()
{
	register struct sym *sp;
	register i;

	for (i=0; i<NHASH; ++i) {
		sp = symhash[i];
		while (sp != NULL) {
			if (sp != &dot && sp->s_type == S_USER)
				sp->s_flag |= S_GBL;
			sp = sp->s_sp;
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

		if (ccase[*p1++] != ccase[*p2++])
			return (0);
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
		h += ccase[*p++];
	} while (--n);
	return (h&HMASK);
}

/*
 * Allocate a block of space.
 * Leave if there is no space left
 * at all.
 */
VOID *
new(n)
unsigned int n;
{
	register VOID *p;

	if ((p = (VOID *) malloc(n)) == NULL) {
		fprintf(stderr, "Out of space!\n");
		exit(1);
	}
	return (p);
}
