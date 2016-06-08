// dZ80 Parse Command Line
//
// Written by Mark Incley

#include <stdio.h>
#ifdef NEW_BUILD
#include <string.h>
#endif

#include "types.h"
#include "dz80.h"
#include "dissz80.h"
#include "parsecmd.h"

int ParseCmdLine(int argc, char* argv[])
{
int	i;

	for (i = 1; i < argc; i++)
		{
		if (argv[i][0] == '/' || argv[i][0] == '-')
			{
			switch (tolower(argv[i][1]))
				{
				case 'l':
					DisFlags |= DISFLAG_LABELLED;
					continue;

				case 'h':
					if (GetAddress(&argv[i][2], &FileBaseOffset))
						return TRUE;
					continue;

				case 'm':
					if (GetAddress(&argv[i][2], &BaseOffset))
						return TRUE;
					continue;

				case 's':
					if (GetAddress(&argv[i][2], &StartAddr))
						return TRUE;
					SpecifiedStart = TRUE;
					continue;

				case 'e':
					if (GetAddress(&argv[i][2], &EndAddr))
						return TRUE;
					SpecifiedEnd = TRUE;
					continue;

				case 'p':
					if (GetString(&argv[i][2], &OpMapFileName[0]) )
						return TRUE;
					continue;

				case 'r':
					if (GetString(&argv[i][2], &RefFileName[0]) )
						return TRUE;
					continue;

				case 'c':
					DisFlags |= DISFLAG_CONSOLE;
					continue;

				case 'a':
					DisFlags &= ~DISFLAG_ADDRDUMP;
					continue;

				case 'n':
					DisFlags &= ~DISFLAG_OPCODEDUMP;
					continue;

				case 'u':
					DisFlags |= DISFLAG_UPPER;
					continue;

				case 'q':
					DisFlags |= DISFLAG_QUIET;
					continue;

				case 'z':
					DisFlags |= DISFLAG_EXCLUDEZ180;
					continue;

				case 'b':
					DisFlags &= ~DISFLAG_USELABELADDRS;
					continue;

				case 'v':
					DisFlags |=	DISFLAG_NOLINECMDS;
					continue;

				case 'x':
					switch (tolower(argv[i][2]))
						{
						case 'o':
							DisFlags |= DISFLAG_REFOUTPORT;
							break;

						case 'i':
							DisFlags |= DISFLAG_REFINPORT;
							break;

						case 'a':
							DisFlags |= DISFLAG_REFADDR;
							break;

						case 'n':
							DisFlags |= DISFLAG_REFINDIRECT;
							break;

						case 'r':
							DisFlags |= DISFLAG_REFLIMITRANGE;
							break;

						case 'm':
							if (GetAddress(&argv[i][3], &MaxReferences))
								return TRUE;
							if (MaxReferences==0)
								{
								printf("Can't have zero maximum references!\n");
								return TRUE;
								}
							break;

						default:
							printf("Unknown reference type \"%c\". Valid types are o, i, a, r and m.\n",  argv[i][2]);
							return TRUE;
						}
					continue;

				case '?':
					ShowUsage();
					return TRUE;

				default:
					printf("Don't know what to do with \"%s\".\n\n", argv[i]);
					printf("  Type dZ80 /? for help.\n");
					return TRUE;
				}
			}

		if (SrcFileName[0])
			{
			if (DestFileName[0])
				{
				printf("Already have a filename.\n");
				return(1);
				}
			else
				strcpy(DestFileName, argv[i]);
			}
		else
			{
			strcpy(SrcFileName, argv[i]);
			}
		}

	return FALSE;
}


int GetAddress(char *Str, WORD *WordPtr)
{
char *EndPtr;

	if (Str[0] == '=')
		Str++;

	*WordPtr = strtol(Str, &EndPtr, 10);

	if (EndPtr == Str)
		{
		printf("Invalid number \"%s\". Type dZ80 /? for help.\n", Str);
		return TRUE;
		}

	return FALSE;
}


int GetString(char *Str, char *DestStrPtr)
{
	if (Str[0] == '=')
		Str++;

	strcpy(DestStrPtr,Str);
	return (strlen(DestStrPtr) == 0) ? TRUE : FALSE;
}

