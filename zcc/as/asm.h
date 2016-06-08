/* asm.h */

/*
 * (C) Copyright 1989,1990
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */

#define	VERSION	"V01.50d"
/*
 *  1.50a	adds .bound directive		Scott Lawrence
 *  1.50b	adds .pword directive		Scott Lawrence
 *  1.50c	removes .pword directive	Scott Lawrence
 *  1.50d	Adds .repeat directive
 *              Adds .fill directive		Scott Lawrence yorgle@gmail.com
 */

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
 * Case Sensitivity Flag (not used any more, runtime flag -c)
 */
#define	CASE_SENSITIVE	1

/*
 * Assembler definitions.
 */
#define	LFTERM	'('		/* Left expression delimeter */
#define	RTTERM	')'		/* Right expression delimeter */

#define NCPS	32		/* Chars. per symbol */
#define	HUGE	1000		/* A huge number */
#define NERR	3		/* Errors per line */
#define NINPUT	128	 	/* Input buffer size */
#define NCODE	(64*1024)	/* Listing code buffer size (128 originally) */
#define NTITL	64		/* Title buffer size */
#define	NSBTL	64		/* SubTitle buffer size */
#define	NHASH	64		/* Buckets in hash table */
#define	HMASK	077		/* Hash mask */
#define	NLPP	60		/* Lines per page */
#define	MAXFIL	6		/* Maximum command line input files */
#define	MAXINC	6		/* Maximum nesting of include files */
#define	MAXIF	10		/* Maximum nesting of if/else/endif */
#define	FILSPC	255		/* Chars. in filespec */

#define NLIST	0		/* No listing */
#define SLIST	1		/* Source only */
#define ALIST	2		/* Address only */
#define CLIST	3		/* Code */

#define	dot	sym[0]		/* Dot, current loc */
#define	dca	area[0]		/* Dca, default code area */

typedef	unsigned int addr_t;

struct	area
{
	struct	area *a_ap;	/* Area link */
	char	a_id[NCPS];	/* Area Name */
	int	a_ref;		/* Ref. number */
	addr_t	a_size;		/* Area size */
	addr_t	a_fuzz;		/* Area fuzz */
	int	a_flag;		/* Area flags */
};

#define	A_CON	000		/* Concatenating */
#define	A_OVR	004		/* Overlaying */
#define	A_REL	000		/* Relocatable */
#define	A_ABS	010		/* absolute */
#define	A_NOPAG	000		/* Non-Paged */
#define	A_PAG	020		/* Paged */

#define	R_WORD	0000		/* 16 bit */
#define	R_BYTE	0001		/*  8 bit */

#define	R_AREA	0000		/* Base type */
#define	R_SYM	0002

#define	R_NORM	0000		/* PC adjust */
#define	R_PCR	0004

#define	R_BYT1	0000		/* Byte count for R_BYTE = 1 */
#define	R_BYT2	0010		/* Byte count for R_BYTE = 2 */

#define	R_SGND	0000		/* Signed Byte */
#define	R_USGN	0020		/* Unsigned Byte */

#define	R_NOPAG	0000		/* Page Mode */
#define	R_PAG0	0040		/* Page '0' */
#define	R_PAG	0100		/* Page 'nnn' */

#define	R_HIGH	0040000		/* High Byte */
#define	R_RELOC	0100000		/* Relocation */

#define	R_DEF	00		/* Global def. */
#define	R_REF	01		/* Global ref. */
#define	R_REL	00		/* Relocatable */
#define	R_ABS	02		/* Absolute */
#define	R_GBL	00		/* Global */
#define	R_LCL	04		/* Local */

struct	mne
{
	struct	mne *m_mp;	/* Hash link */
	char	m_id[NCPS];	/* Mnemonic */
	char	m_type;		/* Mnemonic subtype */
	char	m_flag;		/* Mnemonic flags */
	addr_t	m_valu;		/* Value */
};

struct	sym
{
	struct	sym  *s_sp;	/* Hash link */
	struct	tsym *s_tsym;	/* Temporary symbol link */
	char	s_id[NCPS];	/* Symbol */
	char	s_type;		/* Symbol subtype */
	char	s_flag;		/* Symbol flags */
	struct	area *s_area;	/* Area line, 0 if absolute */
	int	s_ref;		/* Ref. number */
	addr_t	s_addr;		/* Address */
};

#define	S_GBL	01		/* Global */
#define	S_ASG	02		/* Assigned */
#define	S_MDF	04		/* Mult. def */
#define	S_END	010		/* End mark for pst. */

#define	S_NEW	0		/* New name */
#define	S_USER	1		/* User name */
				/* unused slot */
				/* unused slot */
				/* unused slot */

#define	S_BYTE	5		/* .byte */
#define	S_WORD	6		/* .word */
#define	S_ASCII	7		/* .ascii */
#define	S_ASCIZ	8		/* .asciz */
#define	S_BLK	9		/* .blkb or .blkw */
#define	S_INCL	10		/* .include */
#define	S_DAREA	11		/* .area */
#define	S_ATYP	12		/* .area type */
#define	S_AREA	13		/* .area name */
#define	S_GLOBL	14		/* .globl */
#define	S_PAGE	15		/* .page */
#define	S_TITLE	16		/* .title */
#define	S_SBTL	17		/* .sbttl */
#define	S_IF	18		/* .if */
#define	S_ELSE	19		/* .else */
#define	S_ENDIF	20		/* .endif */
#define	S_EVEN	21		/* .even */
#define	S_ODD	22		/* .odd */
#define	S_RADIX	23		/* .radix */
#define	S_ORG	24		/* .org */
#define	S_MODUL	25		/* .module */
#define	S_BOUND	26		/* .bound */
#define S_REPEAT 27		/* .repeat */
#define S_FILL  28		/* .fill */



struct	tsym
{
	struct	tsym *t_lnk;	/* Link to next */
	char t_num;		/* 0-255$ */
	char t_flg;		/* flags */
	struct	area *t_area;	/* Area */
	addr_t	t_addr;		/* Address */
};

extern	jmp_buf	jump_env;
extern	int	inpfil;
extern	int	incfil;
extern	int	cfile;
extern	int	flevel;
extern	int	tlevel;
extern	int	ifcnd[];
extern	int	iflvl[];
extern	int	radix;
extern	VOID	*new();
extern	int	line;
extern	int	page;
extern	int	lop;
extern	int	pass;
extern	int	pjumpused;
extern	int	lflag;
extern	int	gflag;
extern	int	aflag;
extern	int	oflag;
extern	int	sflag;
extern	int	xflag;
extern	int	fflag;
extern	addr_t	laddr;
extern	addr_t	fuzz;
extern	int	lmode;
extern	struct	area	area[];
extern	struct	area *areap;
extern	struct	sym	sym[];
extern	struct	sym *symp;
extern	struct	sym *symhash[NHASH];
extern	struct	mne *mnehash[NHASH];
extern	char	*ep;
extern	char	eb[NERR];
extern	char	*ip;
extern	char	ib[NINPUT];
extern	char	*cp;
extern	char	cb[NCODE];
extern	int	*cpt;
extern	int	cbt[NCODE];
extern	char	tb[NTITL];
extern	char	stb[NSBTL];
extern	char	symtbl[];
extern	char	aretbl[];
extern	char	module[NCPS];
extern	FILE	*lfp;
extern	FILE	*ofp;
extern	FILE	*tfp;
extern	FILE	*sfp[];
extern	FILE	*ifp[];
extern	unsigned char	ctype[];

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

/*
 * Expression.
 */
struct	expr
{
	char	e_mode;		/* Address mode */
	char	e_flag;		/* Symbol flag */
	addr_t	e_addr;		/* Address */
	union	{
		struct area *e_ap;
		struct sym  *e_sp;
	} e_base;		/* Rel. base */
};

/* C Library functions */
/* for reference only
extern	int		fclose();
extern	char *		fgets();
extern	FILE *		fopen();
extern	int		fprintf();
extern	VOID		longjmp();
extern	VOID *		malloc();
extern	int		printf();
extern	char		putc();
extern	int		rewind();
extern	int		setjmp();
extern	int		strcmp();
extern	char *		strcpy();
extern	int		strlen();
extern	char *		strncpy();
*/

/* Machine independent functions */

/* asmain.c */
extern	VOID		asmbl();
extern	VOID		newdot();
extern	VOID		phase();
extern	VOID		usage();

/* aslex.c */
extern	int		endline();
extern	int		get();
extern	VOID		getid();
extern	int		asgetline();
extern	int		getmap();
extern	int		getnb();
extern	int		more();
extern	VOID		unget();

/* assym.c */
extern	struct	area *	alookup();
extern	struct	mne *	mlookup();
extern	int		hash();
extern	struct	sym *	lookup();
extern	VOID *		new();
extern	int		symeq();
extern	VOID		syminit();
extern	VOID		symglob();
extern	VOID		allglob();

/* assubr.c */
extern	VOID		aerr();
extern	VOID		diag();
extern	VOID		err();
extern	VOID		qerr();
extern	VOID		rerr();

/* asexpr.c */
extern	VOID		abscheck();
extern	addr_t		absexpr();
extern	int		digit();
extern	VOID		expr();
extern	int		oprio();
extern	VOID		term();

/* aslist.c */
extern	VOID		list();
extern	VOID		list1();
extern	VOID		list2();
extern	VOID		lstsym();
extern	VOID		slew();

/* asout.c */
extern	int		hibyte();
extern	int		lobyte();
extern	VOID		out();
extern	VOID		outab();
extern	VOID		outarea();
extern	VOID		outaw();
extern	VOID		outall();
extern	VOID		outbuf();
extern	VOID		outchk();
extern	VOID		outgsd();
extern	VOID		outrb();
extern	VOID		outrw();
extern	VOID		outsym();
extern	VOID		out_lb();
extern	VOID		out_lw();
extern	VOID		out_rw();
extern	VOID		out_tw();


/* Machine dependent variables */

extern	char *		cpu;
extern	char *		dsft;
extern	int		hilo;
extern	struct	mne	mne[];

/* Machine dependent functions */

extern	VOID		machin();
extern	VOID		minit();
