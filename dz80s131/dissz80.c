/*
	Z80 Disassembler Module v1.31, Copyright 1996-1998 Mark Incley.

    * Now requires an external function "dz80ProgressUpdate" which is
      called regularly during the disassembly with two DWORD parameters,
      the first one is the number of bytes actually processed, the second is
      total number of bytes to be processed.

      This allows the Windows version to "PeekMessage" and keep dZ80
      responsive, and draw a progress bar. Other platforms may simply
      return, or display a % done indicator. The return type is void.
*/

#include <time.h>
#include <stdio.h>

#ifdef NEW_BUILD
#include <stdlib.h>
#include <string.h>
#endif

#include "types.h"
#include "dissz80.h"

#define	AddToDis(str)	strcat(DisBuf, str);

const char	*VersionString 	= "v1.31";		/* Disassembler version */
const char	*IMModes[4]		= {"0",	"0", "1", "2"};

static	char	HexDisBuf[32];		/* Holds the hex. disassembly (easy one!) */
static	char	DisBuf[128];		/* Holds the disassembly */
static	WORD	PC,	LastPC,	LastRefAddr, NumInstructions;
static	WORD	DisFlags;
static 	int		op,	realop, LineCmd;
static	BYTE	HUGE *Z80MemBase;
static	char	IXIYDisp;			/* The IXIY displacement (-128 to 127) */
static	int		Z80Flags, Pass,	NumRecursions, SkippedLastOpcode;
static	BYTE	*FnMap,	*OpMap,	*RefTable;
static	char	Buffer[128], CommentBuffer[128];
static	DWORD	BytesToProcess,	BytesProcessed;
static	FILE	*OutStream,	*OpStream, *RefStream;
static	int		LabelledOutput, RefOutput, DissingToScreen, HaveTabbed;
static	int		ProgressCounter, ShownRefWarning;
static	WORD	DisStart, DisEnd;
static	unsigned int MaxReferences, NumRefs;


char 	*Conditions[8]=
{
	"nz",
	"z",
	"nc",
	"c",
	"po",
	"pe",
	"p",
	"m"
};

char	*AccRotType[8]=
{
	"rlca",
	"rrca",
	"rla",
	"rra",
	"daa",
	"cpl",
	"scf",
	"ccf"
};

char	*CBRotType[8] =
{
	"rlc",
	"rrc",
	"rl",
	"rr",
	"sla",
	"sra",
	"sll",
	"srl"
};

char	*Reg8Idx[8] =
{
	"b",
	"c",
	"d",
	"e",
	"h",
	"l",
	"(hl)",
	"a"	   
};

char	*Reg8AFIdx[8] =
{
	"b",
	"c",
	"d",
	"e",
	"h",
	"l",
	"f",
	"a"	   
};


char	*Reg16Idx[5] =
{
	"bc",
	"de",
	"hl",
	"sp",
	"af"
};

char	*BasicOps[8] =
{
	"add",
	"adc",
	"sub",
	"sbc",
	"and",
	"xor",
	"or",
	"cp"
};

char	*RepeatOps[16] =
{
	"ldi",
	"cpi",
	"ini",
	"outi",

	"ldd",
	"cpd",
	"ind",
	"outd",

	"ldir",
	"cpir",
	"inir",
	"otir",

	"lddr",
	"cpdr",
	"indr",
	"otdr"
};


char *Z180RepeatOps[4] =
{
	"otim",
    "otdm",
    "otimr",
    "otdmr"
};

int	DisassembleZ80(DISZ80 *Dis)
{
char	DisLine[128];
int		i, NumPasses;

	Dis->CreatedREFOK = FALSE;

	Z80MemBase = Dis->Mem0Start;
	DisFlags = Dis->Flags;
    MaxReferences = Dis->MaxReferences;
	NumPasses =	1;
    LabelledOutput = RefOutput = DissingToScreen = FALSE;
	OutStream =	NULL;
	SkippedLastOpcode =	ShownRefWarning = FALSE;	/* Used to blank lines */
	Dis->NumInstructions = 0;

    if ( (DisFlags & DISFLAG_CONSOLE) || (Dis->OutFileName == NULL) )
        DissingToScreen = TRUE;

/* Has labelled output been requested? */

    if (DisFlags & DISFLAG_LABELLED)
    	{
        NumPasses = 2;
    	LabelledOutput = TRUE;
        }

/* Has reference output been requested? */

	if (DisFlags & DISFLAG_ANYREF)
    	{
        RefOutput = TRUE;
        }

	FnMap =	OpMap =	RefTable = NULL;
	OutStream =	OpStream = RefStream = NULL;

/* If creating assembler output, switch off the opcode and address dump */

	if (LabelledOutput == TRUE)
		DisFlags &=	~(DISFLAG_ADDRDUMP | DISFLAG_OPCODEDUMP);

	DisStart = Dis->Start;
	DisEnd = Dis->End;

	if (DisEnd < DisStart)
		{
		LastPC = DisStart;
		DisStart = DisEnd;
		DisEnd = LastPC;
		}

	BytesToProcess = ((DWORD)DisEnd	- (DWORD)DisStart + 1)	* NumPasses;
	BytesProcessed = 0;

	if (!(DisFlags & DISFLAG_QUIET))
		{
		sprintf(Buffer,	"Starting address: %u",	DisStart);
		PrintToConsole(Buffer);
		sprintf(Buffer,	"  Ending address: %u",	DisEnd);
		PrintToConsole(Buffer);
		sprintf(Buffer,	"     Output file: %s",	(Dis->OutFileName != NULL) ? Dis->OutFileName :	"(none)");
		PrintToConsole(Buffer);
		}

/* Using an opcode map file ? */

	if (!PrepareOpMap(Dis))
		return FALSE;

/* Assembler output (2 passes) ? */

	if (LabelledOutput == TRUE)
		{
		if ((FnMap = AllocateMap("Couldn't allocate memory for the function map.", Z80MEMSIZE / 8))	== NULL)
			{
			DisZ80CleanUp();
			return FALSE;
			}
		 for (i=0; i < 8; i++)
   			FlagFn(i <<	3);			// Flag the RST's addresses
		}

/* Are we creating any references ? */

	if (!PrepareReferenceListing(Dis))
		return FALSE;

/* If a destination file has been given, disassemble to file. */

	if (!CreateOutputASMFile(Dis))
		return FALSE;

/* Draw the progress indicator */

	ProgressCounter = 0;
    DoProgress();

/* Ok, at last, here's the disassembly loop! */

	for	(Pass =	NumPasses; Pass	> 0; Pass--)
		{
		if (!(DisFlags & DISFLAG_QUIET))
			{
		   	sprintf(Buffer,	"Pass %d of %d", NumPasses - Pass +	1, NumPasses);
			PrintToConsole(Buffer);
			}

        NumInstructions = 0;				/* Number of instructions disassembled */
    	PC = DisStart;
		LastPC = PC;
		while (PC <= DisEnd)
			{
			if (LastPC > PC)		/* Check for PC wraparound */
				break;

			LastPC = PC;

			HexDisBuf[0] = DisBuf[0] = CommentBuffer[0] = 0;
			Z80Flags = NumRecursions = 0;	/* Clear and IX/IY prefixes, etc. */
            HaveTabbed = FALSE;

            DoProgress();

            LineCmd = 0;				/* Clear out line commands */
			DisassembleInstruction();
			NumInstructions++;			/* Another line disassembled */

/* If we're building up the FnMap, don't disassemble */

			if (Pass !=	1)
				continue;

			if (Dis->OpMapFileName != NULL)
				if ((OpMap[LastPC >> 3]	& (1 <<	(LastPC	& 7))) == 0)
					{
					SkippedLastOpcode =	TRUE;
					continue;
					}

/* Clear out the disline (where the disassembly is built up) */

			DisLine[0] = 0;

			if (SkippedLastOpcode)
				sprintf(DisLine, ";\n;\n");

			SkippedLastOpcode =	FALSE;

/* Add the instruction's address ? */

			if (DisFlags & DISFLAG_ADDRDUMP)
				{
				sprintf(Buffer,	"%04x  ", LastPC);
				strcat(DisLine,	Buffer);
				}

/* Add the hex dump? */

			if (DisFlags & DISFLAG_OPCODEDUMP)
				{
				sprintf(Buffer,	"%-8s  ", HexDisBuf);
				strcat(DisLine,	Buffer);
				}

/* Add the disassembly. */

			if (LabelledOutput && IsFnUsed(LastPC))
				sprintf(DisLine, "\nl%04x:  ", LastPC);
			else
				if ((DisFlags &	(DISFLAG_OPCODEDUMP	| DISFLAG_ADDRDUMP)) ==	0)
					strcat(DisLine,	"        ");

			strcat(DisLine,	DisBuf);

/* Add any comment */

            if (CommentBuffer[0])
            	{
                i = strlen(DisLine);
                if (i < COMMENTCOLUMN)
                	{
                	memset(Buffer, ' ', COMMENTCOLUMN-i);
                    Buffer[COMMENTCOLUMN-i] = 0;
                    strcat(DisLine, Buffer);
                    }
	            strcat(DisLine, CommentBuffer);
                }

/* Make uppercase if necessary */

			if (DisFlags & DISFLAG_UPPER)
				StringToUpper(DisLine);

/* Finally, add the new-line */

			strcat(DisLine,"\n");

/* Really finally, process any line commands */

            if (!(DisFlags & DISFLAG_NOLINECMDS))
            	{
	            if (LineCmd & LC_BLANKLINE)
    	        	strcat(DisLine, "\n");
                }

			if ( (OutStream	== NULL) ||	(DisFlags &	DISFLAG_CONSOLE) )
				printf("%s", DisLine);

			if (OutStream != NULL)
				{
				if (!fwrite(DisLine, strlen(DisLine), 1, OutStream))
					{
					PrintToErrOut("Couldn't write to file. The disk is probably full.");
					DisZ80CleanUp();
					return FALSE;
					}
				}

			ProgressCounter--;
			}					/* Next instruction */

		ProgressCounter = 0;	/* Force progress update */
        DoProgress();
		}						/* Next pass */

    if (RefOutput)
    	WriteReferenceFile(Dis);

	if (!(DisFlags & DISFLAG_QUIET))
		{
		sprintf(Buffer,	"\nDisassembled %u instructions.",	NumInstructions);
		PrintToConsole(Buffer);
		}

	Dis->NumInstructions = NumInstructions;

	DisZ80CleanUp();
	return TRUE;			/* Disassembled OK */
}

/* Release allocated memory, close open files, etc.. */

void DisZ80CleanUp(void)
{
	if (RefStream != NULL)
		{
		fclose(RefStream);
		RefStream =	NULL;
		}

	if (OutStream != NULL)
		{
		fclose(OutStream);
		OutStream =	NULL;
		}

	if (OpStream !=	NULL)
		{
		fclose(OpStream);
		OpStream = NULL;
		}

	if (RefTable != NULL)
		{
		free(RefTable);
		}

	if (OpMap != NULL)
		{
		free(OpMap);
		}

	if (FnMap != NULL)
		{
		free(FnMap);
		}

	return;
}

/* Time to disassemble an instruction */

void DisassembleInstruction(void)
{
	if (++NumRecursions	> 3)
		{
		AddToDisUnknown(NULL);
		return;
		}

	GetNextOpCode();	// Get initial opcode in op
	switch (op)
		{

		case 0xcb:		// Disassemble the Rotates, SETs and RES's
			Z80Flags |=	Z80CB;
			DisCB();
			return;

		case 0xed:
			Z80Flags |=	Z80ED;
			DisED();
			return;

		case 0xdd:
			Z80Flags |=	Z80IX;		/* Signal IX prefix */
			Z80Flags &=	~Z80IY;		/* Clear IY prefix */
			DisassembleInstruction();
			return;

		case 0xfd:
			Z80Flags |=	Z80IY;		/* Signal IY prefix */
			Z80Flags &=	~Z80IX;		/* Clear IX prefix */
			DisassembleInstruction();
			return;
		}

	realop = op;

	if (op < 0x40)
		{
		Dis00to3F();
		return;
		}

	if (op >= 0x40 && op <=	0x7f)
		{
		Dis40to7F();			// All the LD's
		return;
		}

	if (op >= 0x80 && op <=	0xbf)
		{
		Dis80toBF();
		return;
		}

	if (op >= 0xc0)
		{
		DisC0toFF();
		return;
		}

	AddToDisUnknown(NULL);
}

/* The main man! */

int	GetNextOpCode(void)
{
	op = Z80MemBase[PC++];
	sprintf(Buffer,	"%02x",	(BYTE)op);
	strcat(HexDisBuf, Buffer);

	BytesProcessed++;

	return op;
}

void DisCB(void)
{
char	Num[3];

/*
	If there's an IX IY prefix, then the displacement comes *BEFORE* the
	final opcode. LD (IX+dd),nn is a similar case.
*/

	if (Z80Flags & Z80IXIY)
		GetIXIYDisplacement();

	GetNextOpCode();

/* Test for undocumented DDCB stuff */

	if (Z80Flags & Z80IXIY)
		if ((op	& 7) !=	REG_HL)
			{
			DisDDCB();
			return;
			}

	if (op < 0x40)
		{
/* Do the rotates */
		AddToDisTab(CBRotType[(op >> 3)	& 7]);
		AddToDisReg8(op, FALSE);
		}
	else
		{
		switch (op >> 6)
			{
			case 1:
				AddToDisTab("bit");
				break;

			case 2:
				AddToDisTab("res");
				break;

			case 3:
				AddToDisTab("set");
				break;
			}

		Num[0] = '0'+((op >> 3)& 7);
		Num[1] = ',';
		Num[2] = 0;
		AddToDis(Num);

/* Finally, add the register component. */
		AddToDisReg8(op, FALSE);
		}
}


/* Decode the DDCB instruction */

void DisDDCB(void)
{
char	Num[4],	BitResSet;


	if (op < 0x40)
		{
/* Do the undocumented rotates */
		AddToDisTabLD("");
		AddToDisReg8(op, REG_HL);		/* REG_HL stops L->IXl, etc.. */
		AddToDis(",");
		AddToDis(CBRotType[(op >> 3) & 7]);
		AddToDis(" ");
		AddToDisReg8(REG_HL, FALSE);
		return;
		}

	BitResSet =	op >> 6;

	if (BitResSet >= 2)
		{
		AddToDisTabLD("");
		AddToDisReg8(op, REG_HL);		/* REG_HL stops L->IXl, etc.. */
		AddToDis(",");
		}

	switch (BitResSet)
		{
		case 1:
			AddToDisTab("bit");
			break;
		case 2:
			AddToDisTab("res");
			break;
		case 3:
			AddToDisTab("set");
			break;
		}

	Num[0] = ' ';
	Num[1] = '0'+((op >> 3)& 7);
	Num[2] = ',';
	Num[3] = 0;
	
	AddToDis(Num + (BitResSet <	2) );

	// Finally, add the register component.
	AddToDisReg8(REG_HL, FALSE);

	if (BitResSet == 1)
		AddToDisUndoc();
	return;
}


void DisED(void)
{
	GetNextOpCode();

/* Catch the solitary SLT pseudo-opcode */

   switch (op)
          {
          case 0xFB:
            AddToDisUnknown("Super Loader Trap");
		    return;
		  }

    if (op <= 0x3f)
       if (DisED00to3F())
          return;

	if (op >= 0x40 && op <=	0x7f)
       if (DisED40to7F())
          return;

	if (op >= 0x80 && op <=	0xbf)
		if (DisED80toBF())
           return;

/* Unknown EDxx opcode */

	AddToDisUnknown("Undocumented 8 T-State NOP");
	return;
}

/* ED00 - ED3F are currently only Z180 instructions */

int DisED00to3F(void)
{
	int		r, bb;

    if (DisFlags & DISFLAG_EXCLUDEZ180)
    	return FALSE;

    bb = op & 7;
    r = (op >> 3) & 7;

    switch (bb)
    	{
        case 0:		// ED 0x00 - 0x38
        	AddToDisTab("in0");
            AddToDis(Reg8Idx[r]);
            AddToDis(",(");
            AddToDis8BitAbs(FALSE);
            AddToDis(")");
            AddToDisCommentZ180();
            return TRUE;

        case 1:		// ED 0x01 - 0x39
        	AddToDisTab("out0");
            AddToDis("(");
            AddToDis8BitAbs(FALSE);
            AddToDis("),");
            AddToDis(Reg8Idx[r]);
            AddToDisCommentZ180();
            return TRUE;

        case 4:		// ED 0x04 - 0x3c
        	AddToDisTab("tst");
            AddToDis(Reg8Idx[r]);
            AddToDisCommentZ180();
            return TRUE;
        }

	return FALSE;
}


int	DisED40to7F(void)
{
char	Address[16];
BYTE	EDop;

/* First, let's get those nasty special case opcodes. */

	EDop = op;


	switch (op)
		{
        case 0x76:		// This is SLP which clashes with the undocumented Z80's IM 1
        	if (!(DisFlags & DISFLAG_EXCLUDEZ180))
            	{
                AddToDisTab("slp");
                AddToDisCommentZ180();
	            return TRUE;
                }
            break;


        case 0x4c:		// The Z180's MLT instructions
        case 0x5c:
        case 0x6c:
        case 0x7c:
        	if (DisFlags & DISFLAG_EXCLUDEZ180)
            	return FALSE;
            AddToDisTab("mlt");
            AddToDis(Reg16Idx[(op>>4) & 3]);
            AddToDisCommentZ180();
            return TRUE;

        case 0x64:		// Z180's TST nn
        	if (DisFlags & DISFLAG_EXCLUDEZ180)
            	return FALSE;
            AddToDisTab("tst");
            AddToDis8BitAbs(FALSE);
            AddToDisCommentZ180();
            return TRUE;

        case 0x74:
        	if (DisFlags & DISFLAG_EXCLUDEZ180)
            	return FALSE;
            AddToDisTab("tstio");
            AddToDis("(");
            AddToDis8BitAbs(FALSE);
            AddToDis(")");
            AddToDisCommentZ180();
            return TRUE;


/* Back to the regular Z80's stuff */

		case 0x45:
			AddToDisTab("retn");
            LineCmd |= LC_BLANKLINE;
			return TRUE;

		case 0x47:
			AddToDisTabLD("i,a");
			return TRUE;

		case 0x4d:
			AddToDisTab("reti");
            LineCmd |= LC_BLANKLINE;
			return TRUE;

		case 0x4f:
			AddToDisTabLD("r,a");
			return TRUE;

		case 0x57:
			AddToDisTabLD("a,i");
			return TRUE;

		case 0x5f:
			AddToDisTabLD("a,r");
			return TRUE;

		case 0x67:
			AddToDisTab("rrd");
			return TRUE;

		case 0x6f:
			AddToDisTab("rld");
			return TRUE;
		}

	switch (op & 7)
		{
		case 0:
			AddToDisTab("in");
			AddToDis(Reg8AFIdx[(op >> 3) & 7] );
			AddToDis(",(c)");
			return TRUE;

		case 1:
			AddToDisTab("out");
			AddToDis("(c),");
			AddToDis(Reg8AFIdx[(op >> 3) & 7] );
			return TRUE;

		case 2:
			AddToDisTab( (op & 0x8)	? "adc"	: "sbc");
			AddToDisHLIXIY();
			AddToDis(",");
			AddToDisReg16(op>>4);
			return TRUE;

		case 3:
			sprintf(Address, "(#%04x)",	Get16BitParam()	);
			AddRefEntry(LastRefAddr, LastPC, DISREF_ADDR);
			AddToDisTabLD("");
			if (EDop & 0x8)
				{
				AddToDisReg16(EDop >> 4);
				AddToDis(",");
				AddToDis(Address);
				return TRUE;
				}
			else
				{
				AddToDis(Address);
				AddToDis(",");
				AddToDisReg16(EDop >> 4);
				return TRUE;
				}

		case 4:
			AddToDisTab("neg");		/* It's a NEG */
			if (EDop !=	0x44)
				AddToDisUndoc();	/* But undocumented? */
			return TRUE;

		case 5:
			AddToDisTab("ret");
			AddToDisUndoc();
            LineCmd |= LC_BLANKLINE;
			return TRUE;

		case 6:
			AddToDisTab("im");		/* Interrupt mode... */
			AddToDis(IMModes[(EDop & 0x18) >> 3] );
			if ((EDop == 0x4e) || (EDop	>= 0x60))
				AddToDisUndoc();
			return TRUE;
		}

	return FALSE;
}


int	DisED80toBF(void)
{

	if (!(DisFlags & DISFLAG_EXCLUDEZ180))
    	{
		switch (op)
    		{
        	case 0x83:		// otim
        	case 0x8b:		// otdm
        	case 0x93:		// otimr
        	case 0x9b:		// otdmr
            	AddToDisTab(Z180RepeatOps[(op >> 3) & 3]);
            	AddToDisCommentZ180();
            	return TRUE;
        	}
        }

	if (op >= 0xA0)
    	{
		if ((op	& 7) > 3)
			return FALSE;		/* Don't know this! */

		AddToDisTab(RepeatOps[(op &	3)+((op	& 0x18)	>> 1)] );
        return TRUE;
        }

	return FALSE;
}


void Dis00to3F(void)
{
	switch (op & 0x0f)
		{
		case 0:
		case 8:
			switch ((op	>> 3) &	7)
				{
				case 0:		//0x00
					AddToDisTab("nop");
					return;

				case 1:		//0x08
					AddToDisTab("ex");
					AddToDis("af,af'");
					return;

				case 2:		//0x10
					AddToDisTab("djnz");
					FlagFn(AddToDisRel8(FALSE));
					return;

				case 3:		//0x18
					AddToDisTab("jr");
					FlagFn(AddToDisRel8(FALSE));
					return;

				default:
					AddToDisTab("jr");
					AddToDis(Conditions[(op	>> 3) &	3] );
					FlagFn(AddToDisRel8(TRUE));
					return;
				}
		case 1:
			AddToDisTabLD("");
			AddToDisReg16(op >>	4);
			AddToDis16BitAbs(TRUE);
			AddRefEntry(LastRefAddr, LastPC, DISREF_ADDR);
			return;

		case 2:
			switch ((op	>> 4) &	3)
				{
				case 0:	   	/* ld (bc),a */
				case 1:		/* ld (de),a */
					AddToDisTabLD("(");
					AddToDis(Reg16Idx[(op >> 4)	& 1] );
					AddToDis("),a");
					return;

				case 2:	   	// 0x22 = ld (nn),hl
				case 3:		// 0x32 = ld (nn),a
					AddToDisTabLD("(");
					AddToDis16BitAbs(FALSE);
					AddToDis("),");
					if (realop & 0x10)
						{
						AddToDis("a");
						}
					else
						{
						AddToDisHLIXIY();
						}
					AddRefEntry(LastRefAddr, LastPC, DISREF_INDIRECT);
					return;
				}

		case 3:
			AddToDisTab("inc");
			AddToDisReg16(op>>4);
			return;

		case 4:
			AddToDisTab("inc");
			AddToDisReg8(op>>3,	op>>3);
			return;

		case 5:
			AddToDisTab("dec");
			AddToDisReg8(op>>3,	op>>3);
			return;

		case 6:
		case 0x0e:
			AddToDisTabLD("");
			AddToDisReg8(op>>3,	op>>3);
			AddToDis8BitAbs(TRUE);
			return;

		case 7:
		case 0x0f:
			AddToDisTab(AccRotType[(op>>3) & 7]	);
			return;

		case 9:
			AddToDisTab("add");
			AddToDisHLIXIY();
			AddToDis(",");
			AddToDisReg16((op>>4) &	3);
			return;

		case 0x0a:
			switch ((op	>> 4) &	3)
				{
				case 0:
				case 1:
					AddToDisTabLD("a,(");
					AddToDis(Reg16Idx[(op >> 4)	& 1] );
					AddToDis(")");
					return;

				case 2:
				case 3:
					if (op & 0x10)
						AddToDisTabLD("a");
					else
						{
						AddToDisTabLD("");
						AddToDisHLIXIY();
						}
					AddToDis(",(");
					AddToDis16BitAbs(FALSE);
            		AddToDis(")");
			        AddRefEntry(LastRefAddr, LastPC, DISREF_INDIRECT);
					return;
				}

		case 0xb:
			AddToDisTab("dec");
			AddToDisReg16((op>>4) &	3);
			return;


		case 0xc:
			AddToDisTab("inc");
			AddToDisReg8(op>>3,	op>>3);
			return;

		case 0xd:
			AddToDisTab("dec");
			AddToDisReg8(op>>3,	op>>3);
			return;
		}

	AddToDisUnknown(NULL);
}

void Dis40to7F(void)
{
	if (op == 0x76)
		{
		AddToDisTab("halt");
		return;
		}

	AddToDisTabLD("");
	AddToDisReg8(realop	>> 3, realop);
	AddToDis(",");
	AddToDisReg8(realop, realop	>> 3);
	return;
}


void Dis80toBF(void)
{
int	GenOp;

	GenOp =	(op>>3)	& 7;
	AddToDisTab(BasicOps[GenOp]);

	if (GenOp <	2 || GenOp == 3)
		AddToDis("a,");

	AddToDisReg8(op, op);
}


void	DisC0toFF(void)
{
int	GenOp;

	GenOp =	(op	>> 3) &	7;

	switch (op & 0xf)		/* Break it down into eight basics */
		{
		case 0:
		case 8:
			AddToDisTab("ret");
			AddToDis(Conditions[GenOp]);
            LineCmd |= LC_BLANKLINE;
			return;

		case 1:			/* POP rr */
		case 5:			/* PUSH rr */
			AddToDisTab((op	& 4) ? "push" :	"pop");
			AddToDisReg16NoAnd(((op	>> 4) &	3) + (op >=	0xf0));
			return;

		case 2:
		case 0x0a:
			AddToDisTab("jp");
			AddToDis(Conditions[GenOp]);
			FlagFn(AddToDis16BitAbs(TRUE));
			return;

		case 4:
		case 0x0c:
			AddToDisTab("call");
			AddToDis(Conditions[GenOp]);
			FlagFn(AddToDis16BitAbs(TRUE));
			return;

		case 3:
			switch ((op	>> 4) &	3)
				{
				case 0:		// 0xc3
					AddToDisTab("jp");
					FlagFn(AddToDis16BitAbs(FALSE));
					return;

				case 1:		// 0xd3
					AddToDisTab("out");
					sprintf(Buffer,"(#%02x),a",	GetNextOpCode()	);
					AddToDis(Buffer);
					AddRefEntry(op, LastPC, DISREF_OUTPORT);
					return;

				case 2:		// 0xe3
					AddToDisTab("ex");
					AddToDis("(sp),");
					AddToDisHLIXIY();
					return;

				case 3:		// 0xf3
					AddToDisTab("di");
					return;
				}

		case 6:
		case 0x0e:
			AddToDisTab(BasicOps[GenOp]	);
			if (GenOp <	2 || GenOp == 3)
				AddToDis("a,");
			AddToDis8BitAbs(FALSE);
			return;

		case 7:
		case 0x0f:
			AddToDisTab("rst");
						sprintf(Buffer,	"#%x", (op & (7	<< 3)) );
			AddToDis(Buffer);
			return;

		case 9:
			switch ((op	>> 4) &	3)
				{
				case 0:		// 0xc9
					AddToDisTab("ret");
                    LineCmd |= LC_BLANKLINE;
					return;

				case 1:		// 0xd9
					AddToDisTab("exx");
					return;

				case 2:		// 0xe9
					AddToDisTab("jp");
					AddToDis("(")
					AddToDisHLIXIY();
					AddToDis(")");
					return;

				case 3:		// 0xf9
					AddToDisTabLD("sp,");
					AddToDisHLIXIY();
					return;
				}


		case 0x0b:
			switch ((op	>> 4) &	3)
				{
				case 1:		// 0xdb
					AddToDisTab("in");
					sprintf(Buffer,"a,(#%02x)",	GetNextOpCode()	);
					AddToDis(Buffer);
					AddRefEntry(op, LastPC, DISREF_INPORT);
					return;

				case 2:		//0xeb
					AddToDisTab("ex");
					AddToDis("de,hl");
					return;

				case 3:		//0xfb
					AddToDisTab("ei");
					return;
				}



		case 0x0d:	   		/* N.B. this can only get here with #cd */
			AddToDisTab("call");
			FlagFn(AddToDis16BitAbs(FALSE));
			return;
		}

	AddToDisUnknown(NULL);
}


void AddToDisTab(char *str)
{
	int Len;

	AddToDis(str);

    if (HaveTabbed)
    	return;

    Len = strlen(DisBuf);
	memset(Buffer, ' ',	TABSIZE	- Len );
	Buffer[TABSIZE - Len] = 0;
	AddToDis(Buffer);

    HaveTabbed = TRUE;
	return;
}


void AddToDisTabLD(char	*str)
{
	AddToDisTab("ld");
	AddToDis(str);
	return;
}


void AddToDisCommentZ180(void)
{
	AddToDisComment("Z180 instruction");
    return;
}

void AddToDisComment(char *str)
{
	if (!(Z80Flags & Z80COMMENT))
			{
			sprintf(CommentBuffer, "; ");
            Z80Flags |= Z80COMMENT;
            }

    strcat(CommentBuffer, str);
	return;
}

void AddToDisHLIXIY(void)
{
	if (!(Z80Flags & Z80IXIY) )
		{
		AddToDis("hl");
		return;
		}

	AddToDis( (Z80Flags	& Z80IX) ? "ix"	: "iy");
	return;
}


/*  AddToDisReg8(opcode)

	Adds b,c,d,e,h,l,(hl) or a to disassembly, taking into consideration
	the IX and IY prefixes. "op2" is used to determine whether a "IXl"
	reference is valid or not.
*/

void AddToDisReg8(int op, int op2)
{
	op &= 7;

	if (Z80Flags & Z80IXIY)
		{
		op2	&= 7;

		if (!(op & (Z80CB |	Z80ED))	&& (op2	!= REG_HL) )
			{
			if (op == REG_L)
				{
				AddToDis((Z80Flags & Z80IX)	? "ixl"	: "iyl");
				return;
				}
		
			if (op == REG_H)
				{
				AddToDis((Z80Flags & Z80IX)	? "ixh"	: "iyh");
				return;
				}
			}

		   	if (op == REG_HL)
			{
			if (Z80IXIY)
				GetIXIYDisplacement();
			if (IXIYDisp >=	0)
				sprintf(Buffer,"(%s+#%02x)", ((Z80Flags	& Z80IX) ? "ix"	: "iy"), IXIYDisp );
			else
				sprintf(Buffer,"(%s-#%02x)", ((Z80Flags	& Z80IX) ? "ix"	: "iy"), -IXIYDisp );
			AddToDis(Buffer);
			return;
			}
		}

	AddToDis(Reg8Idx[op]);
	return;
}


/*	AddToDisReg16(opcode)

	Adds bc,de,hl or sp to disassembly, taking into consideration
	the IX and IY prefixes
*/

void AddToDisReg16(int op)
{
	op &= 3;
	if ( (op ==	2) && (Z80Flags	& Z80IXIY) )
		AddToDisHLIXIY();
	else
		AddToDis(Reg16Idx[op]);
	return;
}

void AddToDisReg16NoAnd(int	op)
{
	if ( (op ==	2) && (Z80Flags	& Z80IXIY) )
		AddToDisHLIXIY();
	else
		AddToDis(Reg16Idx[op]);
	return;
}


WORD AddToDisRel8(int CommaFlag)
{
	if (CommaFlag)
		AddToDis(",");

	GetNextOpCode();

	LastRefAddr	= (PC +	(char)op);
	Add16BitAddress(LastRefAddr);

	sprintf(Buffer,	"(%d)",	(char)op);
	AddToDisComment(Buffer);
	return LastRefAddr;
}

void AddToDis8BitAbs(int CommaFlag)
{
	if (CommaFlag)
		AddToDis(",");

	GetNextOpCode();

	sprintf(Buffer,	"#%02x",op );
	AddToDis(Buffer);
	return;
}

WORD AddToDis16BitAbs(int CommaFlag)
{
	if (CommaFlag)
		AddToDis(",");

	Get16BitParam();
	Add16BitAddress(LastRefAddr);
	return LastRefAddr;
}

void	AddToDisUndoc(void)
{
	AddToDisComment("Undocumented");
	return;
}

void	AddToDisUnknown(char *Comment)
{
int	i, NumOpCodes;

	AddToDisTab("db");

	NumOpCodes = abs(PC	- LastPC);
	for	(i = 0;	i <	NumOpCodes;	i++)
		{
		sprintf(Buffer,	"#%02x", Z80MemBase[(LastPC	+ i)&0xffff] );
		if (i <	(NumOpCodes-1))
			strcat(Buffer, ", ");
		AddToDis(Buffer);
		}

	if (Comment	== NULL)
		AddToDisComment("Unknown opcode");
	else
		AddToDisComment(Comment);

	return;
}

char	GetIXIYDisplacement(void)
{
	if (Z80Flags & Z80GOTIXIYDISP)	// Already got IXIY dispacement?
		return IXIYDisp;

	IXIYDisp = (char)GetNextOpCode();
	Z80Flags |=	Z80GOTIXIYDISP;
	return IXIYDisp;
}


WORD Get16BitParam(void)
{
	LastRefAddr	= GetNextOpCode();
	LastRefAddr	|= (GetNextOpCode()	<< 8);
	return LastRefAddr;
}

void FlagFn(unsigned int Addr)
{
	if (LabelledOutput == FALSE || Pass < 2)
		return;

	FnMap[Addr >> 3] |=	(1 << (Addr	& 7));
	return;
}

int	IsFnUsed(unsigned int Addr)
{
	return (FnMap[Addr >> 3] & (1 << (Addr & 7)) );
}



BYTE* AllocateMap(char *ErrorStr, unsigned int BytesWanted)
{
	BYTE *MapPtr;

	MapPtr = (BYTE *)malloc(BytesWanted);

	if (MapPtr == NULL)
		{
		PrintToErrOut(ErrorStr);
		}
	else
		{
		memset(MapPtr, 0, BytesWanted);		/* Clear the map */
		}
		
	return MapPtr;
}


/* Create the output .asm file and header */

int	CreateOutputASMFile(DISZ80 *Dis)
{

char 	MsgBuf[_MAX_PATH + 80];
time_t	secs_now;
struct	tm *time_now;

/* Set up the time structures */

	time(&secs_now);
	time_now = localtime(&secs_now);

	if (Dis->OutFileName !=	NULL)
		{
		OutStream =	fopen(Dis->OutFileName,"wt");
		if (OutStream == NULL)
			{
			sprintf(Buffer,	"Couldn't create the output file %s", Dis->OutFileName);
			PrintToErrOut(Buffer);
			DisZ80CleanUp();
			return FALSE;
			}
		sprintf(MsgBuf,	"; Disassembly of the file \"%s\"\n;\n", Dis->SrcFileName);
		fwrite(MsgBuf, strlen(MsgBuf), 1, OutStream);
		if (Dis->OpMapFileName != NULL)
			{
			sprintf(MsgBuf,	"; Using the opcode map file \"%s\"\n;\n", Dis->OpMapFileName);
			fwrite(MsgBuf, strlen(MsgBuf), 1, OutStream);
			}
		sprintf(MsgBuf,	"; Created with dZ80 %s\n;\n", VersionString);
		fwrite(MsgBuf, strlen(MsgBuf), 1, OutStream);
		strftime(MsgBuf, sizeof(MsgBuf), "; on %A, %d of %B %Y at %I:%M %p", time_now);
		fwrite(MsgBuf, strlen(MsgBuf), 1, OutStream);
		sprintf(MsgBuf,	"\n;\n");
 		fwrite(MsgBuf, strlen(MsgBuf), 1, OutStream);
		}

	return TRUE;
}


int	PrepareOpMap(DISZ80	*Dis)
{

	if (Dis->OpMapFileName != NULL)
		{
		if ((OpMap = AllocateMap("Couldn't allocate memory for the opcode map.", Z80MEMSIZE/8))	== NULL)
			{
			DisZ80CleanUp();
			return FALSE;
			}

 		OpStream = fopen(Dis->OpMapFileName, "rb");
		if (OpStream ==	NULL)
			{
			sprintf(Buffer,	"Couldn't open the opcode map file %s.", Dis->OpMapFileName);
			PrintToErrOut(Buffer);
			DisZ80CleanUp();
			return FALSE;
			}
		if (fread(OpMap, 1,	Z80MEMSIZE / 8,	OpStream) <	(Z80MEMSIZE	/ 8) )
			{
			PrintToErrOut("Warning: Couldn't read the entire opcode map file");
			}
		fclose(OpStream);
        OpStream = NULL;
		}

	return TRUE;
}


int	PrepareReferenceListing(DISZ80 *Dis)
{
unsigned int BytesWanted, ActualMaxRef;
char	MsgBuf[128];

	NumRefs = 0;

	if (!(DisFlags & DISFLAG_ANYREF))
		return TRUE;

	if (Dis->RefFileName ==	NULL)
		{
		PrintToErrOut("Missing reference filename.");
        DisZ80CleanUp();
		return FALSE;
	   	}

    BytesWanted = MaxReferences * sizeof(DISREF);

/*
	This seeminly pointless looking code protects the 16-bit version of dZ80
    from trying to allocate more than 65535 bytes - which it can't do using
    standard C
*/

	if (BytesWanted == 0)
    	{
        PrintToErrOut("Too many maximum references have been specified.\n");
        DisZ80CleanUp();
        return FALSE;
        }

    ActualMaxRef = BytesWanted / sizeof(DISREF);

    if (MaxReferences != ActualMaxRef)
    	{
        sprintf(MsgBuf, "Error: MaxReferences (%u) disagrees with allocated max (%u)\n", MaxReferences, ActualMaxRef);
        PrintToErrOut(MsgBuf);
        }

/* End of seemingly pointless 16-bit protection code :) */

	if ((RefTable = AllocateMap("Couldn't allocate memory for the reference map.", BytesWanted)) == NULL)
		{
		DisZ80CleanUp();
		return FALSE;
		}

    return TRUE;
}


void WriteReferenceFile(DISZ80 *Dis)
{

unsigned int		i, t;
char    MsgBuf[80 + _MAX_PATH];
char	*TypeMsg;
DISREF	*P;

	RefStream = fopen(Dis->RefFileName, "wt");
    if (RefStream == NULL)
    	{
        sprintf(MsgBuf, "Couldn't create the reference file \"%s\".", Dis->RefFileName);
        PrintToErrOut(MsgBuf);
        return;
        }

    sprintf(MsgBuf,"dZ80 %s Reference file from the disassembly of \"%s\".\n\n", VersionString, Dis->SrcFileName);
    fwrite(MsgBuf, 1, strlen(MsgBuf), RefStream);

	for (t = DISREF_INPORT; t <= DISREF_INDIRECT; t++)
    	{
    	if (t == DISREF_INPORT)
        	{
            if (Dis->Flags & DISFLAG_REFINPORT)
        		TypeMsg = "Input Port";
            else
            	continue;
            }

    	if (t == DISREF_OUTPORT)
        	{
            if (Dis->Flags & DISFLAG_REFOUTPORT)
                TypeMsg = "Output Port";
            else
            	continue;
            }

    	if (t == DISREF_ADDR)
        	{
            if (Dis->Flags & DISFLAG_REFADDR)
                TypeMsg = "Address";
            else
            	continue;
            }

        if (t == DISREF_INDIRECT)
        	{
            if (Dis->Flags & DISFLAG_REFINDIRECT)
                TypeMsg = "Indirect address";
            else
            	continue;
            }

		sprintf(MsgBuf, TypeMsg);
        strcat(MsgBuf, " Reference:\n\n");
	    fwrite(MsgBuf, 1, strlen(MsgBuf), RefStream);

        P = (DISREF *)RefTable;

        for(i=0; i < NumRefs; i++)
        	{
            if (P->RefType == t)
            	{
                sprintf(MsgBuf,	"%s %04x. The first of %u references is at address %04x.\n", TypeMsg, P->Addr, P->Hits, P->FirstRefAddr);
			    fwrite(MsgBuf, 1, strlen(MsgBuf), RefStream);
                }
            P++;
            }
      	sprintf(MsgBuf, "\n");
       	fwrite(MsgBuf, 1, strlen(MsgBuf), RefStream);
        }

	sprintf(MsgBuf, "End of reference file\n");
    fwrite(MsgBuf, 1, strlen(MsgBuf), RefStream);

    fclose(RefStream);
    RefStream = NULL;
	Dis->CreatedREFOK = TRUE;
    return;
}


void AddRefEntry(WORD Addr, WORD PC, int RefType)
{
	unsigned int	i;
    DISREF	*P;

	switch (RefType)
    	{
        case DISREF_INPORT:
        	if (!(DisFlags & DISFLAG_REFINPORT))
            	return;
            break;

        case DISREF_OUTPORT:
        	if (!(DisFlags & DISFLAG_REFOUTPORT))
            	return;
            break;

		case DISREF_ADDR:
        case DISREF_INDIRECT:
        	if (!(DisFlags & (DISFLAG_REFADDR | DISFLAG_REFINDIRECT)))
            	return;

            if (DisFlags & DISFLAG_REFLIMITRANGE)
            	if (Addr >= DisStart && Addr <= DisEnd)
                	return;
            break;

        default:
        	PrintToErrOut("Unexpected reference type.");
            return;
        }

	P = (DISREF *)RefTable;

	for (i=0; i < NumRefs; i++)
    	{
		if (P->Addr == Addr)
        	if (P->RefType == RefType)
            	{
                P->Hits++;
                return;
                }
        P++;
        }
/*
 	Ok, so there's no match, so it's new entry time.
    N.B. P already points to the new entry
*/
	if (NumRefs >= MaxReferences)
    	{
        if (ShownRefWarning == FALSE)
        	{
        	ShownRefWarning = TRUE;
       		PrintToErrOut("Warning: Out of reference table entries.");
            }
        return;
    	}

	P->RefType = RefType;
    P->Addr = Addr;
    P->Hits = 1;
    P->FirstRefAddr = PC;
	NumRefs++;

	return;
}

void DoProgress(void)
{
    if (ProgressCounter-- <= 0)
    	{
		dZ80ProgressUpdate(BytesProcessed, BytesToProcess, DissingToScreen);
        ProgressCounter = PROGRESSUPDATEFREQ;
        }

    return;
}


void Add16BitAddress(WORD Addr)
{
    if ((LabelledOutput == TRUE) && (Pass == 1) && (DisFlags & DISFLAG_USELABELADDRS))
		if (IsFnUsed(Addr))
			{
			sprintf(Buffer, "l%04x", Addr);
			AddToDis(Buffer);
			return;
			}

	sprintf(Buffer,	"#%04x", Addr );
	AddToDis(Buffer);
	return;
}


void StringToLower(char *s)
{
	while (*s)
    	*s++ = tolower(*s);

    return;
}

void StringToUpper(char *s)
{
	while (*s)
    	*s++ = toupper(*s);

    return;
}

