/* Header file for dZ80 */

#ifndef __MIDZ80__
#define	__MIDZ80__


#define	LOADBUFSIZE	4096	// Load the source binary in 4K chunks

/* Error return codes */

#define	ERR_NONE		0
#define	ERR_OUTOFMEM	1
#define	ERR_LOADFILE	2

typedef struct
{
	char *Extension;
	WORD FileHeaderSize;
	WORD BaseOffset;
} DISFILE;

extern 	char	SrcFileName[_MAX_PATH], DestFileName[_MAX_PATH];
extern	char	OpMapFileName[_MAX_PATH], RefFileName[_MAX_PATH];
extern 	WORD	StartAddr, EndAddr, BaseOffset, FileBaseOffset, DisFlags;
extern	WORD	MaxReferences;
extern	int		NumPasses;
extern	int		SpecifiedEnd, SpecifiedStart;

void	ShowUsage(void);
void	ScanFilenameForPresets(void);
void	ParseFilenames(void);


/* LoadFL16/32 */

int 	LoadFile(char *FileName, DWORD *BytesLoaded, BYTE HUGE **MemoryBase, WORD Z80MemOffset, DWORD FileBaseOffset);
void 	FreeZ80Mem(BYTE HUGE **MemoryBase);


#endif
