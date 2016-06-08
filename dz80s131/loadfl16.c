// LoadFile. 16-Bit Borland C++ MS-DOS version.
//
// Allocates the Z80's address space and loads in the source file.
//
// Written by Mark Incley


#include <stdio.h>
#include <mem.h>
#include <alloc.h>

#include "types.h"
#include "dz80.h"
#include "dissz80.h"

void FreeZ80Mem(BYTE HUGE **MemoryBase)
{
	farfree((BYTE far *)*MemoryBase);
}


int LoadFile(char *FileName, DWORD *BytesLoaded, BYTE HUGE **MemoryBase, WORD Z80MemOffset, DWORD FileBaseOffset)
{
BYTE 	*FileBufStart;
DWORD	FileOffset;
WORD 	ChunkSize, ChunkLoaded;
FILE 	*InStream;
char 	Buffer[128];

	FileOffset = Z80MemOffset;	// We're not necessarily starting at 0
	*BytesLoaded = 0;				// We've not loaded anything

	*MemoryBase = (BYTE HUGE *)farmalloc(Z80MEMSIZE);

	if (*MemoryBase == NULL)
		{
		PrintToErrOut("dZ80 couldn't allocate memory for the Z80 program.\n");
		exit(ERR_OUTOFMEM);
		}

// Another non standard library function to clear far pointer memory.

	_fmemset((BYTE far *)*MemoryBase, 0, Z80MEMSIZE/2);
	_fmemset((BYTE far *)(*MemoryBase + (Z80MEMSIZE / 2)), 0, Z80MEMSIZE/2);

	FileBufStart = (BYTE *)malloc(LOADBUFSIZE);

	if (FileBufStart == NULL)
		{
		sprintf(Buffer, "Couldn't allocate %d bytes of memory for load buffer\n", LOADBUFSIZE);
		PrintToErrOut(Buffer);
		return (ERR_LOADFILE);
		}

	InStream=fopen(FileName, "rb");
	if (InStream == NULL)
		{
		sprintf(Buffer, "Couldn't open the source file \"%s\"\n", FileName);
		PrintToErrOut(Buffer);
		free(FileBufStart);
		return(ERR_LOADFILE);
		}

// See if it's a whopping great file
//
	if (fseek(InStream, 0, SEEK_END))
		{
		sprintf(Buffer, "Couldn't determine the size of the file %s\n", FileName);
		PrintToErrOut(Buffer);
		}
	else
		{
		if (ftell(InStream) > 65536L )
			PrintToErrOut("Warning: The source binary is over 65,536 bytes in length.\n"
							  "         dZ80 will ignore the remainder of the file.\n\n");
		}

// Put the file's position back in the correct place.
//
	fseek(InStream, FileBaseOffset, SEEK_SET);

	while   (FileOffset < Z80MEMSIZE)
		{
		ChunkSize = (FileOffset < (Z80MEMSIZE - LOADBUFSIZE) ) ? LOADBUFSIZE : Z80MEMSIZE - FileOffset;
		ChunkLoaded = fread(FileBufStart, 1, ChunkSize, InStream);

		if (ChunkLoaded == 0)
			break;

		_fmemcpy((BYTE far *)(*MemoryBase+FileOffset), FileBufStart, ChunkLoaded);

		FileOffset += ChunkLoaded;
		*BytesLoaded += ChunkLoaded;
		}

	fclose(InStream);

	free(FileBufStart);

	return ERR_NONE;	// Return "no error"
}


















