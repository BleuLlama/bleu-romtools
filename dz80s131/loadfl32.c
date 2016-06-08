/*
	LoadFile 32. Standard C Version. Written by Mark Incley

 	Allocates the Z80's address space and loads in the source file.
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef NEW_BUILD
#include <string.h>
#endif

#include "types.h"
#include "dz80.h"
#include "dissz80.h"

void FreeZ80Mem(BYTE **MemoryBase)
{
	free((BYTE *)*MemoryBase);
}

int LoadFile(char *FileName, DWORD *BytesLoaded, BYTE **MemoryBase, WORD Z80MemOffset, DWORD FileBaseOffset)
{
FILE *InStream;
char Buffer[_MAX_PATH + 64];

	*BytesLoaded = 0;			/* We've not loaded anything yet */

	*MemoryBase = (BYTE *)malloc(Z80MEMSIZE);

	if (*MemoryBase == NULL)
		{
		PrintToErrOut("dZ80 couldn't allocate memory for the Z80 program.");
		exit(ERR_OUTOFMEM);
		}

/* Clear the Z80's address space */

	memset(*MemoryBase, 0, Z80MEMSIZE);

	InStream=fopen(FileName, "rb");
	if (InStream == NULL)
		{
		sprintf(Buffer, "Couldn't open the source file \"%s\"", FileName);
		PrintToErrOut(Buffer);
		return(ERR_LOADFILE);
		}

/* See if it's a whopping great file */

	if (fseek(InStream, 0, SEEK_END))
		{
		sprintf(Buffer, "Couldn't determine the size of the file %s", FileName);
		PrintToErrOut(Buffer);
		}
	else
		{
		if (ftell(InStream) > 65536L )
			PrintToErrOut("Warning: The source binary is over 65,536 bytes in length.");
		}

/* Put the file's position back in the correct place */

	fseek(InStream, FileBaseOffset, SEEK_SET);

/* Read the whole file in one gulp (should be only a small file) */

	*BytesLoaded = fread(*MemoryBase + Z80MemOffset, 1, Z80MEMSIZE - Z80MemOffset, InStream);

	fclose(InStream);	/* Ah, we forgot to close this prior to v1.20! */
	
	return ERR_NONE;	/* Return "no error" */
}


















