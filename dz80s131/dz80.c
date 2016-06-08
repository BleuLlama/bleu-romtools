/*
        dZ80 - Z80 Disassembler v1.31

		Written by Mark Incley, 1st November, 1996 (v1.00)
*/

#include <stdio.h>
#include <string.h>

#ifdef NEW_BUILD
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "types.h"
#include "dz80.h"
#include "dissz80.h"
#include "parsecmd.h"

char	SrcFileName[_MAX_PATH], DestFileName[_MAX_PATH];
char	OpMapFileName[_MAX_PATH], RefFileName[_MAX_PATH];
WORD	StartAddr, EndAddr, BaseOffset, FileBaseOffset, DisFlags;
WORD	MaxReferences;
BYTE	HUGE *MemoryBase, PC;
int		SpecifiedEnd, SpecifiedStart;
static 	DWORD BytesLoaded;


/* Not a great big table at the moment :) */

DISFILE FileExtList[]=
{
	{"sna", 27, 16384},		/* .SNA - Spectrum Snapshots */
	{NULL, 0, 0}
};


int main(int argc, char* argv[])
{
	char	Buffer[96];
    DISZ80	Dis;

	sprintf(Buffer, "dZ80 - Z80 Disassembler %s, Copyright 1996-1998 by Mark Incley.\n", VersionString);
	PrintToErrOut(Buffer);

	SpecifiedEnd 	= FALSE;
	SpecifiedStart	= FALSE;
	DisFlags		= DISFLAG_OPCODEDUMP | DISFLAG_ADDRDUMP | DISFLAG_USELABELADDRS;
	FileBaseOffset	= 0;
	BaseOffset 		= 0;
	StartAddr  		= 0;
	EndAddr	   		= 65535;
    MaxReferences	= 3000;		/* Default max references */

	if (ParseCmdLine(argc, argv))
		exit(1);

	if (SrcFileName[0] == 0)
		{
		ShowUsage();
		exit(0);
		}

	ScanFilenameForPresets();

    ParseFilenames();

	if (LoadFile(SrcFileName, &BytesLoaded, &MemoryBase, BaseOffset, FileBaseOffset))
		exit(ERR_LOADFILE);

	if (BytesLoaded == 0)
		{
		PrintToErrOut("Cannot load a zero byte file.\n");
		exit(1);
		}

/* C'mon, try and be a slightly intelligent about the start and end addresses */

	if (StartAddr < BaseOffset)
		{
		if (!SpecifiedStart)
			StartAddr = BaseOffset;
		}

	if (EndAddr > (BaseOffset+BytesLoaded))
		{
		if (!SpecifiedEnd)
			EndAddr = BaseOffset+BytesLoaded-1;
		}


/* Fill in the DISZ80 structure required by DisassembleZ80 */

    Dis.Mem0Start = MemoryBase;
    Dis.Start = StartAddr;
    Dis.End = EndAddr;
    Dis.Flags = DisFlags;
    Dis.MaxReferences 	= MaxReferences;
    Dis.SrcFileName		= SrcFileName;
	Dis.OutFileName		= (DestFileName[0]) ? DestFileName : NULL;
    Dis.OpMapFileName 	= (OpMapFileName[0]) ? OpMapFileName : NULL;
    Dis.RefFileName		= (RefFileName[0]) ? RefFileName : NULL;

/* And do the biz.... */

    DisassembleZ80(&Dis);

	FreeZ80Mem(&MemoryBase);

	exit (0);
}


void ParseFilenames(void)
{
	char	*ExtPtr;

/* If specified a reference filename, don't interfere */

	if (RefFileName[0])
		return;

/* Are we going to need a reference filename ? */

    if (DisFlags & DISFLAG_ANYREF)
    	{
		strcpy(RefFileName, SrcFileName);

/* Hardly, foolproof, but this will replace any extension with .PIC */

		ExtPtr = (char *)strrchr(RefFileName, '.');

/* If there is a period, make sure it isn't a terminating one */

		if (ExtPtr != NULL)
			{
			if (*ExtPtr+1)
				{
				strcpy(ExtPtr+1, "ref");
				return;
				}
			}
        else
			{
			strcat(RefFileName, ".ref");
            }
        }

	return;
}


void ScanFilenameForPresets(void)
{
char 	*ExtPtr;
char	FileExt[16], Buffer[80];
int 	i;

	ExtPtr = strrchr(SrcFileName, '.');
	if (ExtPtr == NULL)
		return; 			/* No extension, so no presets. */

	strcpy(FileExt, (ExtPtr)+1);
	StringToLower(FileExt);		/* Make lower case */

	for (i=0; ; i++)
		{
		if (FileExtList[i].Extension == 0)
			return;

		if (!strcmp(FileExt, FileExtList[i].Extension) )
			{
			if (!(DisFlags & DISFLAG_QUIET))
				{
				sprintf(Buffer, "Using presets for .%s file extension.\n\n", FileExt);
				PrintToErrOut(Buffer);
				}
			FileBaseOffset = FileExtList[i].FileHeaderSize;
			BaseOffset = FileExtList[i].BaseOffset;
			return;
			}
		}
}


void PrintToErrOut(char *Str)
{
	fwrite(Str, 1, strlen(Str), stderr);
	fwrite("\n", 1, 1, stderr);
	return;
}


void PrintToConsole(char *Str)
{
	printf("%s\n", Str);
	return;
}


/*
	This function is frequently called by the disassembler module.
    It will be used by the Windows version of dZ80 to implement a progress
    meter.
*/

void dZ80ProgressUpdate(DWORD BytesProcessed, DWORD BytesToProcess, int DissingToScreen)
{
    return;
}



void ShowUsage(void)
{
	printf("Disassembles a Z80 binary file.              E-mail: dz80@inkland.demon.co.uk\n\n");
	printf("DZ80 [switches] Z80file [Output Assembly File]\n"
	       "\n"
	       "  /H=nn     Skips past the first nn bytes of the input file.\n"
	       "  /M=nn     Specifies whereabouts in Z80 memory the source file starts.\n"
	       "  /S=nn     Specifies the address to start disassembling from.\n"
	       "  /E=nn     Specifies the ending disassembly address.\n"
           "  /R=file   Specify the name for the reference file. Used with /XA /XN /XO /XI.\n"
           "  /XI       Create reference of input ports.\n"
           "  /XO       Create reference of output ports.\n"
           "  /XA       Create reference of addresses.\n"
           "  /XN       Create reference of indirect addresses.\n"
           "  /XR       Limits references to those outside the disassembly range.\n"
	       "  /L	    Create a labelled (assembleable) output file.\n"
		   "  /P=opmap  Use an opcode map to determine the addresses to disassemble.\n"
	       "\n"
	       "If no output file is specified, dZ80 will disassemble to the screen.\n"
           "\n"
	       "Example: DZ80 /h=27 /m=16384 /s=32768 /e=40549 mayhem.sna\n"
		);

	return;
}

