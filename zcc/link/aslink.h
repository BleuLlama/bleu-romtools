/* aslink.h */

/*
 * (C) Copyright 1989,1990
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */

/*)BUILD
	$(PROGRAM) =	ASLINK
	$(INCLUDE) =	ASLINK.H
	$(FILES) = {
		LKMAIN.C
		LKLEX.C
		LKAREA.C
		LKHEAD.C
		LKSYM.C
		LKEVAL.C
		LKDATA.C
		LKLIST.C
		LKRLOC.C
		LKS19.C
		LKIHX.C
	}
	$(STACK) = 2000
*/

#define	VERSION	"V01.50"

/* DECUS C void definition */
/* File/extension seperator */

#ifdef	decus
#define	VOID	char
#define	FSEPX	'.'
#endif

/* PDOS C void definition */
/* File/extension seperator */

#ifdef	PDOS
#define	VOID	char
#define	FSEPX	':'
#endif

/* Default void definition */
/* File/extension seperator */

#ifndef	VOID
#define	VOID	void
#define	FSEPX	'.'
#endif

/*
 * Case Sensitivity Flag
 */
#define	CASE_SENSITIVE	1

/*
 * This file defines the format of the
 * relocatable binary file.
 */

#define	NCPS	32		/* characters per symbol */
#define	NDATA	16		/* actual data */
#define	NINPUT	128		/* Input buffer size */
#define	NHASH	64		/* Buckets in hash table */
#define	HMASK	077		/* Hash mask */
#define	NLPP	60		/* Lines per page */
#define	NTXT	16		/* T values */
#define	FILSPC	255		/* File spec length */

/*
 * Relocation types.
 */
#define	R_WORD	0000		/* 16 bit */
#define	R_BYTE	0001		/*  8 bit */

#define	R_AREA	0000		/* Base type */
#define	R_SYM	0002

#define	R_NORM	0000		/* PC adjust */
#define	R_PCR	0004

#define	R_BYT1	0000		/* Byte count for R_BYTE = 1 */
#define	R_BYT2	0010		/* Byte count for R_BYTE = 2 */

#define	R_SGND	0000		/* Signed value */
#define	R_USGN	0020		/* Unsigned value */

#define	R_NOPAG	0000		/* Page Mode */
#define	R_PAG0	0040		/* Page '0' */
#define	R_PAG	0100		/* Page 'nnn' */

/*
 * Global symbol types.
 */
#define	S_REF	1		/* referenced */
#define	S_DEF	2		/* defined */

/*
 * Area types
 */
#define	A_CON	000		/* concatenate */
#define	A_OVR	004		/* overlay */
#define	A_REL	000		/* relocatable */
#define	A_ABS	010		/* absolute */
#define	A_NOPAG	000		/* non-paged */
#define	A_PAG	020		/* paged */

/*
 * File types
 */
#define	F_STD	1		/* stdin */
#define	F_LNK	2		/* File.lnk */
#define	F_REL	3		/* File.rel */

typedef unsigned int addr_t;

struct	head
{
	struct	head   *h_hp;	/* Header link */
	struct	lfile  *h_lfile;/* Associated file */
	int	h_narea;	/* # of areas */
	struct	areax **a_list;	/* Area list */
	int	h_nglob;	/* # of global symbols */
	struct	sym   **s_list;	/* Globle symbol list */
	char	m_id[NCPS];	/* Module name */
};

struct	area
{
	struct	area	*a_ap;	/* Area link */
	struct	areax	*a_axp;	/* Area extension link */
	addr_t	a_addr;		/* Beginning address of area */
	addr_t	a_size;		/* Total size of the area */
	char	a_type;		/* Area subtype */
	char	a_flag;		/* Flag byte */
	char	a_id[NCPS];	/* Name */
};

struct	areax
{
	struct	areax	*a_axp;	/* Area extension link */
	struct	area	*a_bap;	/* Base area link */
	struct	head	*a_bhp;	/* Base header link */
	addr_t	a_addr;		/* Beginning address of section */
	addr_t	a_size;		/* Size of the area in section */
};

struct	sym
{
	struct	sym	*s_sp;	/* Symbol link */
	struct	areax	*s_axp;	/* Symbol area link */
	char	s_type;		/* Symbol subtype */
	char	s_flag;		/* Flag byte */
	addr_t	s_addr;		/* Address */
	char	s_id[NCPS];	/* Name */
};

struct	lfile
{
	struct	lfile	*f_flp;	/* lfile link */
	int	f_type;		/* File type */
	char	*f_idp;		/* Pointer to file spec */
};

struct	base
{
	struct	base  *b_base;	/* Base link */
	char	      *b_strp;	/* String pointer */
};

struct	globl
{
	struct	globl *g_globl;	/* Global link */
	char	      *g_strp;	/* String pointer */
};

struct	sdp
{
	struct	area  *s_area;	/* Paged Area link */
	struct	areax *s_areax;	/* Paged Area Extension Link */
	addr_t	s_addr;		/* Page address offset */
};

struct	rerr
{
	int	aindex;		/* Linking area */
	int	mode;		/* Relocation mode */
	addr_t	rtbase;		/* Base address in section */
	int	rindex;		/* Area/Symbol reloaction index */
	addr_t	rval;		/* Area/Symbol offset value */
};

/* Variable definitions */

extern		char	*_abs_;
extern		char	*dext;
extern		char	*ip;
extern		char	ib[NINPUT];
extern		unsigned char	ctype[];

extern	char	scase[];
extern	char	icase[];
extern	char	*ccase;

#define	SPACE	0000
#define ETC	0000
#define	LETTER	0001
#define	DIGIT	0002
#define	BINOP	0004
#define	RAD2	0010
#define	RAD8	0020
#define	RAD10	0040
#define	RAD16	0100
#define	ILL	0200

#define	DGT2	DIGIT|RAD16|RAD10|RAD8|RAD2
#define	DGT8	DIGIT|RAD16|RAD10|RAD8
#define	DGT10	DIGIT|RAD16|RAD10
#define	LTR16	LETTER|RAD16

extern	struct	lfile	*startp;
extern	struct	lfile	*linkp;
extern	struct	lfile	*lfp;
extern	struct	lfile	*filep;
extern	struct	lfile	*cfp;
extern	struct	head	*headp;
extern	struct	head	*hp;
extern	struct	area	*areap;
extern	struct	area	*ap;
extern	struct	areax	*axp;
extern	struct	sym	*symhash[NHASH];
extern	struct	base	*basep;
extern	struct	base	*bsp;
extern	struct	globl	*globlp;
extern	struct	globl	*gsp;
extern	struct	sdp	sdp;
extern	struct	rerr	rerr;

extern		char	outnam[];

extern		FILE	*sfp;
extern		FILE	*ofp;
extern		FILE	*mfp;
extern		int	inpfil;
extern		int	cfile;
extern		int	oflag;
extern		int	sflag;
extern		int	mflag;
extern		int	xflag;
extern		int	pflag;
extern		int	radix;
extern		int	hilo;
extern		int	pass;
extern		int	line;
extern		int	page;
extern		int	lop;
extern		int	rtcnt;
extern		addr_t	rtval[];
extern		int	rtflg[];

/* C Library function definitions */
/* for reference only
extern	VOID		exit();
extern	int		fclose();
extern	char *		fgets();
extern	FILE *		fopen();
extern	int		fprintf();
extern	VOID		free();
extern	VOID *		malloc();
extern	char		putc();
extern	char *		strcpy();
extern	int		strlen();
extern	char *		strncpy();
*/

/* Program function definitions */

/* lkmain.c */
extern	FILE *		afile();
extern	VOID		bassav();
extern	VOID		gblsav();
extern	VOID		link();
extern	VOID		map();
extern	int		parse();
extern	VOID		setbas();
extern	VOID		setgbl();
extern	VOID		usage();

/* lklex.c */
extern	char		endline();
extern	char		get();
extern	VOID		getfid();
extern	VOID		getid();
extern	int		asgetline();
extern	int		getmap();
extern	char		getnb();
extern	int		more();
extern	VOID		skip();
extern	VOID		unget();

/* lkarea.c */
extern	VOID		lkparea();
extern	VOID		lnkarea();
extern	VOID		lnksect();
extern	VOID		newarea();

/* lkhead.c */
extern	VOID		module();
extern	VOID		newhead();

/* lksym.c */
extern	int		hash();
extern	struct	sym *	lkpsym();
extern	VOID *		new();
extern	struct	sym *	newsym();
extern	VOID		symdef();
extern	int		symeq();
extern	VOID		syminit();
extern	VOID		symmod();
extern	addr_t		symval();

/* lkeval.c */
extern	int		digit();
extern	addr_t		eval();
extern	addr_t		expr();
extern	int		oprio();
extern	addr_t		term();

/* lklist.c */
extern	VOID		lstarea();
extern	VOID		newpag();
extern	VOID		slew();

/* lkrloc.c */
extern	addr_t		add_b1();
extern	addr_t		add_b2();
extern	addr_t		add_w();
extern	addr_t		evword();
extern	VOID		rele();
extern	VOID		reloc();
extern	VOID		relt();
extern	VOID		relr();
extern	VOID		relp();
extern	VOID		relerr();
extern	char *		errmsg[];
extern	VOID		errdmp();
extern	VOID		relerp();
extern	VOID		erpdmp();
extern	VOID		prntval();

/* lks19.c */
extern	VOID		s19();

/* lkihx.c */
extern	VOID		ihx();
