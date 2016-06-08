/*	File code8080.c: 2.2 (84/08/31,10:05:09) */
/*% cc -O -c %
 *
 */

#include <stdio.h>
#include "defs.h"
#include "data.h"

/*	Define ASNM and LDNM to the names of the assembler and linker
	respectively */

/*
 *	Some predefinitions:
 *
 *	INTSIZE is the size of an integer in the target machine
 *	BYTEOFF is the offset of an byte within an integer on the
 *		target machine. (ie: 8080,pdp11 = 0, 6809 = 1,
 *		360 = 3)
 *	This compiler assumes that an integer is the SAME length as
 *	a pointer - in fact, the compiler uses INTSIZE for both.
 */
#define	INTSIZE	2
#define	BYTEOFF	0

int	savestkp;

/*
 *	print all assembler info before any code is generated
 *
 */
header (n)
char	*n;
{
	outstr (";\tSmall C z80;\n;\tCoder (2.4,86/11/14)\n;");
	FEvers();
	nl ();
	ol (".globl\t.gchar,.gint,.pchar,.pint,.bool");
	ol (".globl\t.sxt");
	ol (".globl\t.or,.and,.xor");
	ol (".globl\t.eq,.ne,.gt,.le,.ge,.lt,.uge,.ult,.ugt,.ule");
	ol (".globl\t.asr,.asl");
	ol (".globl\t.sub,.neg,.com,.lneg,.mul,.div");
	ol (".globl\t.case");
	outstr ("\t.module");
	ol (n);
}

nl ()
{
	outbyte (EOL);
}

inclib()
{
	return("");
}

initmac()
{
	defmac("Z80\t1");
	defmac("smallc\t1");
}

galign(t)
int	t;
{
	return(t);
}

/*
 *	return size of an integer
 */
intsize() {
	return(INTSIZE);
}

/*
 *	return offset of ls byte within word
 *	(ie: 8080 & pdp11 is 0, 6809 is 1, 360 is 3)
 */
byteoff() {
	return(BYTEOFF);
}

/*
 *	Output internal generated label prefix
 */
olprfix() {
	outbyte('.');
}

/*
 *	Output a label definition terminator
 */
col ()
{
	outbyte (58);
}

/*
 *	begin a comment line for the assembler
 *
 */
comment ()
{
	outbyte (';');
}

/*
 *	Emit user label prefix
 */
prefix ()
{
}


/*
 *	print any assembler stuff needed after all code
 *
 */
trailer ()
{
}

/*
 *	function prologue
 */
prologue ()
{
}

#ifdef	FRAME_IX
/*
 *	set up frame pointer
 */
frame ()
{
	ol ("push\tix");
	/* make ix point to where sp was before push */
	ol ("ld\tix,#0");
	ol ("add\tix,sp");
	stkp = stkp - INTSIZE;
	savestkp = stkp;
	return (INTSIZE);
}
#endif

/*
 *	text (code) segment
 */
gtext ()
{
	ol(".area\t_CODE");
}

/*
 *	data segment
 */
gdata ()
{
	ol(".area\t_DATA");
}

/*
 *	bss segment
 */
gbss ()
{
	ol(".area\t_BSS");
}

/*
 *  Output the variable symbol at scptr as an extrn or a public
 */
ppubext(scptr) char *scptr; {
	if (cptr[STORAGE] == STATIC) return;
	ot (scptr[STORAGE] == EXTERN ? ".globl\t" : ".globl\t");
	prefix ();
	outstr (scptr);
	nl();
}

/*
 * Output the function symbol at scptr as an extrn or a public
 */
fpubext(scptr) char *scptr; {
	if (scptr[STORAGE] == STATIC) return;
	ot (scptr[OFFSET] == FUNCTION ? ".globl\t" : ".globl\t");
	prefix ();
	outstr (scptr);
	nl ();
}

/*
 *  Output a decimal number to the assembler file
 */
onum(num) int num; {
	outdec(num);	/* pdp11 needs a "." here */
}


/*
 *	fetch a static memory cell into the primary register
 */
getmem (sym)
char	*sym;
{
	if ((sym[IDENT] != POINTER) & (sym[TYPE] == CCHAR)) {
		ot ("ld\ta,(");
		outstr (sym + NAME);
		outbyte(')');
#ifdef	FRAME_IX
		ot (";static");
#endif
	 	nl ();
		gcall (".sxt");
	} else {
		ot ("ld\thl,(");
		outstr (sym + NAME);
		outbyte(')');
#ifdef	FRAME_IX
		ot (";static");
#endif
		nl ();
	}
}

/*
 *	fetch the address of the specified symbol into the primary register
 *
 */
getloc (sym)
char	*sym;
{
	int	i;

	immed ();
	if (sym[STORAGE] == LSTATIC) {
		printlabel(glint(sym));
#ifdef	FRAME_IX
		ot (";static");
#endif
		nl();
	} else {
		outdec (glint(sym) - stkp);
#ifdef	FRAME_IX
		/* generate the annotation only if in range of (ix) addressing */
		if ((i = glint(sym) - savestkp) >= -128 && i <= 127)
		{
			ot (";");
			outdec (i);
			outstr ("(ix)");
		}
#endif
		nl ();
		ol ("add\thl,sp");
	}
}

/*
 *	store the primary register into the specified static memory cell
 *
 */
putmem (sym)
char	*sym;
{
	if ((sym[IDENT] != POINTER) & (sym[TYPE] == CCHAR)) {
		ol ("ld\ta,l");
		ot ("ld\t(");
		outstr (sym + NAME);
		outstr ("),a");
		nl ();
	} else {
		ot ("ld\t(");
		outstr (sym + NAME);
#ifdef	FRAME_IX
		outstr ("),hl\t;static");
#else
		outstr ("),hl");
#endif
		nl ();
	}
}

/*
 *	store the specified object type in the primary register
 *	at the address on the top of the stack
 *
 */
putstk (typeobj)
char	typeobj;
{
	gpop ();
	if (typeobj == CCHAR)
		gcall (".pchar");
	else
		gcall (".pint");
}

/*
 *	fetch the specified object type indirect through the primary
 *	register into the primary register
 *
 */
indirect (typeobj)
char	typeobj;
{
	if (typeobj == CCHAR)
		gcall (".gchar");
	else
		gcall (".gint");
}

/*
 *	swap the primary and secondary registers
 *
 */
swap ()
{
	ol ("ex\tde,hl");
}

/*
 *	print partial instruction to get an immediate value into
 *	the primary register
 *
 */
immed ()
{
	ot ("ld\thl,#");
}

/*
 *	push the primary register onto the stack
 *
 */
gpush ()
{
	ol ("push\thl");
	stkp = stkp - INTSIZE;
}

/*
 *	pop the top of the stack into the secondary register
 *
 */
gpop ()
{
	ol ("pop\tde");
	stkp = stkp + INTSIZE;
}

/*
 *	swap the primary register and the top of the stack
 *
 */
swapstk ()
{
	ol ("ex\t(sp),hl");
}

/*
 *	call the specified subroutine name
 *
 */
gcall (sname)
char	*sname;
{
	ot ("call\t");
	outstr (sname);
	nl ();
}

/*
 *	return from subroutine
 *
 */
gret ()
{
	if (frameix)
		ol ("pop\tix");
	ol ("ret");
}

/*
 *	perform subroutine call to value on top of stack
 *
 */
callstk ()
{
	immed ();
	outstr ("$+5");
	nl ();
	swapstk ();
	ol ("jp\t(hl)");
	stkp = stkp + INTSIZE;
}

/*
 *	jump to specified internal label number
 *
 */
jump (label)
int	label;
{
	ot ("j\t");
	printlabel (label);
	nl ();
}

/*
 *	test the primary register and jump if false to label
 *
 */
testjump (label, ft)
int	label,
	ft;
{
	ol ("ld\ta,h");
	ol ("or\tl");
	if (ft)
		ot ("j\tnz,");
	else
		ot ("j\tz,");
	printlabel (label);
	nl ();
}

/*
 *	print pseudo-op  to define a byte
 *
 */
defbyte ()
{
	ot (".db\t");
}

/*
 *	print pseudo-op to define storage
 *
 */
defstorage ()
{
	ot (".ds\t");
}

/*
 *	print pseudo-op to define a word
 *
 */
defword ()
{
	ot (".dw\t");
}

/*
 *	modify the stack pointer to the new value indicated
 *
 */
modstk (newstkp)
int	newstkp;
{
	int	k;

	k = galign(newstkp - stkp);
	if (k == 0)
		return (newstkp);
	if (k > 0) {
		if (k < 7) {
			if (k & 1) {
				ol ("inc\tsp");
				k--;
			}
			while (k) {
				ol ("pop\tbc");
				k = k - INTSIZE;
			}
			return (newstkp);
		}
	} else {
		if (k > -7) {
			if (k & 1) {
				ol ("dec\tsp");
				k++;
			}
			while (k) {
				ol ("push\tbc");
				k = k + INTSIZE;
			}
			return (newstkp);
		}
	}
	swap ();
	immed ();
	outdec (k);
	nl ();
	ol ("add\thl,sp");
	ol ("ld\tsp,hl");
	swap ();
	return (newstkp);
}

/*
 *	multiply the primary register by INTSIZE
 */
gaslint ()
{
	ol ("add\thl,hl");
}

/*
 *	divide the primary register by INTSIZE
 */
gasrint()
{
	gpush();	/* push primary in prep for gasr */
	immed ();
	onum (1);
	nl ();
	gasr ();  /* divide by two */
}

/*
 *	Case jump instruction
 */
gjcase() {
	ot ("jp\t.case");
	nl ();
}

/*
 *	add the primary and secondary registers
 *	if lval2 is int pointer and lval is not, scale lval
 */
gadd (lval,lval2) int *lval,*lval2;
{
	gpop ();
	if (dbltest (lval2, lval)) {
		swap ();
		gaslint ();
		swap ();
	}
	ol ("add\thl,de");
}

/*
 *	subtract the primary register from the secondary
 *
 */
gsub ()
{
	gpop ();
	gcall (".sub");
}

/*
 *	multiply the primary and secondary registers
 *	(result in primary)
 *
 */
gmult ()
{
	gpop();
	gcall (".mul");
}

/*
 *	divide the secondary register by the primary
 *	(quotient in primary, remainder in secondary)
 *
 */
gdiv ()
{
	gpop();
	gcall (".div");
}

/*
 *	compute the remainder (mod) of the secondary register
 *	divided by the primary register
 *	(remainder in primary, quotient in secondary)
 *
 */
gmod ()
{
	gdiv ();
	swap ();
}

/*
 *	inclusive 'or' the primary and secondary registers
 *
 */
gor ()
{
	gpop();
	gcall (".or");
}

/*
 *	exclusive 'or' the primary and secondary registers
 *
 */
gxor ()
{
	gpop();
	gcall (".xor");
}

/*
 *	'and' the primary and secondary registers
 *
 */
gand ()
{
	gpop();
	gcall (".and");
}

/*
 *	arithmetic shift right the secondary register the number of
 *	times in the primary register
 *	(results in primary register)
 *
 */
gasr ()
{
	gpop();
	gcall (".asr");
}

/*
 *	arithmetic shift left the secondary register the number of
 *	times in the primary register
 *	(results in primary register)
 *
 */
gasl ()
{
	gpop ();
	gcall (".asl");
}

/*
 *	two's complement of primary register
 *
 */
gneg ()
{
	gcall (".neg");
}

/*
 *	logical complement of primary register
 *
 */
glneg ()
{
	gcall (".lneg");
}

/*
 *	one's complement of primary register
 *
 */
gcom ()
{
	gcall (".com");
}

/*
 *	Convert primary value into logical value (0 if 0, 1 otherwise)
 *
 */
gbool ()
{
	gcall (".bool");
}

/*
 *	increment the primary register by 1 if char, INTSIZE if
 *      int
 */
ginc (lval) int lval[];
{
	ol ("inc\thl");
	if (lval[2] == CINT)
		ol ("inc\thl");
}

/*
 *	decrement the primary register by one if char, INTSIZE if
 *	int
 */
gdec (lval) int lval[];
{
	ol ("dec\thl");
	if (lval[2] == CINT)
		ol("dec\thl");
}

/*
 *	following are the conditional operators.
 *	they compare the secondary register against the primary register
 *	and put a literl 1 in the primary if the condition is true,
 *	otherwise they clear the primary register
 *
 */

/*
 *	equal
 *
 */
geq ()
{
	gpop();
	gcall (".eq");
}

/*
 *	not equal
 *
 */
gne ()
{
	gpop();
	gcall (".ne");
}

/*
 *	less than (signed)
 *
 */
glt ()
{
	gpop();
	gcall (".lt");
}

/*
 *	less than or equal (signed)
 *
 */
gle ()
{
	gpop();
	gcall (".le");
}

/*
 *	greater than (signed)
 *
 */
ggt ()
{
	gpop();
	gcall (".gt");
}

/*
 *	greater than or equal (signed)
 *
 */
gge ()
{
	gpop();
	gcall (".ge");
}

/*
 *	less than (unsigned)
 *
 */
gult ()
{
	gpop();
	gcall (".ult");
}

/*
 *	less than or equal (unsigned)
 *
 */
gule ()
{
	gpop();
	gcall (".ule");
}

/*
 *	greater than (unsigned)
 *
 */
gugt ()
{
	gpop();
	gcall (".ugt");
}

/*
 *	greater than or equal (unsigned)
 *
 */
guge ()
{
	gpop();
	gcall (".uge");
}

/*	Squirrel away argument count in a register that modstk
	doesn't touch.
*/

gnargs(d)
int	d; {
	ot ("ld\ta,#");
	onum(d);
	nl ();
}
