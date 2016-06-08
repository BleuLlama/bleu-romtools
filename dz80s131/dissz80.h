/* dZ80 Disassembler Module Header */

#ifndef _MIDISSZ80_
#define _MIDISSZ80_

#ifdef _WINDOWS_
#define	HUGE
#endif

#define	Z80MEMSIZE		65536

#define	Z80IX			0x01		/* We're on a IX prefix */
#define	Z80IY			0x02		/* We're on a IY prefix	*/
#define	Z80GOTIXIYDISP	0x04		/* Got IX/IY displacement  */
#define	Z80CB			0x08		/* Processing a CB op-code */
#define	Z80ED			0x10		/* Processing a ED op-code */
#define Z80COMMENT      0x20        /* Have written a comment  */

#define	Z80IXIY		(Z80IX | Z80IY)

#define	DISFLAG_OPCODEDUMP		0x01
#define	DISFLAG_ADDRDUMP		0x02
#define	DISFLAG_UPPER			0x04
#define	DISFLAG_CONSOLE			0x08
#define	DISFLAG_VERBOSE			0x10
#define	DISFLAG_QUIET			0x20
#define	DISFLAG_LABELLED		0x40
#define	DISFLAG_REFINPORT		0x80
#define	DISFLAG_REFOUTPORT		0x100
#define	DISFLAG_REFADDR			0x200
#define DISFLAG_REFINDIRECT		0x400
#define	DISFLAG_REFLIMITRANGE	0x800
#define	DISFLAG_USELABELADDRS	0x1000
#define	DISFLAG_EXCLUDEZ180		0x2000	/* Exclude Z180 instructions */
#define	DISFLAG_NOLINECMDS		0x4000	/* Ignore line commands */

/* These options are used by the Windows 95 version of dZ80 */
#define	DISFLAG_AUTOOUTFILENAME	0x40000000	/* Auto write to (src).asm */
#define	DISFLAG_SHOWASM			0x80000000	/* Show the assembly when done */

/* The options are used to control what happens after each instruction disassembly */
#define	LC_BLANKLINE			0x0001

/* Reference listing stuff */
#define	DISFLAG_ANYREF			(DISFLAG_REFINPORT | DISFLAG_REFOUTPORT | DISFLAG_REFADDR | DISFLAG_REFINDIRECT)

#define	DISREF_INPORT			1
#define	DISREF_OUTPORT			2
#define	DISREF_ADDR				3
#define	DISREF_INDIRECT			4

#define	TABSIZE					8
#define	COMMENTCOLUMN			40

#define	PROGRESSUPDATEFREQ		200		/* Num. instructions to disassemble before updating progress */

#define	REG_B		0
#define	REG_C		1
#define	REG_D		2
#define	REG_E		3
#define	REG_H		4
#define	REG_L		5
#define	REG_HL		6
#define	REG_A		7

#define	MN_ADD		0
#define	MN_ADC		1
#define	MN_SUB		2
#define	MN_SBC		3
#define	MN_AND		4
#define	MN_XOR		5
#define	MN_OR		6
#define	MN_CP		7

typedef struct
{
	BYTE	HUGE *Mem0Start;		/* Pointer to Z80's zero address */
 	WORD	Start;					/* Starting disassembler address */
 	WORD	End;					/* Ending disassembler address */
 	WORD	Flags;					/* See DISFLAG_ defines */
	WORD	MaxReferences;			/* Maximum number of references */
	unsigned int NumInstructions;	/* How many instructions were disassembled */
	int		CreatedREFOK;			/* Set to TRUE if created .ref file OK */
	char	*SrcFileName;			/* Used only for report header */
 	char	*OutFileName;		  	/* Destination assembler file */
 	char	*OpMapFileName;			/* Opcode map file name */
	char	*RefFileName;			/* Reference file name */
} DISZ80;


typedef struct
{
	WORD	RefType;				/* Reference type */
	WORD	Addr;					/* Memory or port address */
	WORD	Hits;					/* Number of times referenced */
	WORD	FirstRefAddr;			/* Address where it's first refrenced */
} DISREF;

extern const char *VersionString;

int		DisassembleZ80(DISZ80 *Dis);
void	DisassembleInstruction(void);
int		GetNextOpCode(void);
void	DisCB(void);
void	DisDDCB(void);
void	DisED(void);
void	AddToDisTab(char *str);
void	AddToDisTabLD(char *str);
void	AddToDisReg8(int op, int op2);
void	AddToDisReg16(int op);
void	AddToDisReg16NoAnd(int op);
void	AddToDisHLIXIY(void);
void	AddToDisUnknown(char *Comment);
char	GetIXIYDisplacement(void);
void	Dis40to7F(void);
void	Dis80toBF(void);
void	DisC0toFF(void);
void	Dis00to3F(void);
WORD	AddToDis16BitAbs(int CommaFlag);
void	AddToDis8BitAbs(int CommaFlag);
WORD	AddToDisRel8(int CommaFlag);
int 	DisED00to3F(void);
int		DisED40to7F(void);
int		DisED80toBF(void);
void	AddToDisComment(char *str);
void 	AddToDisCommentZ180(void);
void	FlagFn(unsigned int Addr); 
int		IsFnUsed(unsigned int Addr);
void	PrintToErrOut(char *Str);
void	PrintToConsole(char *Str);
void	AddToDisUndoc(void);
WORD	Get16BitParam(void);
void	DisZ80CleanUp(void);
int		CreateOutputASMFile(DISZ80 *Dis);
int		PrepareOpMap(DISZ80 *Dis);
void	AddRefEntry(WORD Addr, WORD PC, int RefType);
void	WriteReferenceFile(DISZ80 *Dis);
int		PrepareReferenceListing(DISZ80 *Dis);
void	DoProgress(void);
void 	Add16BitAddress(WORD Addr);


void	dZ80ProgressUpdate(DWORD BytesProcessed, DWORD BytesToProcess, int DissingToScreen);
BYTE*	AllocateMap(char *ErrorStr, unsigned int BytesWanted);

void	StringToLower(char *s);
void	StringToUpper(char *s);


#endif		/* _MIDISSZ80_ */






