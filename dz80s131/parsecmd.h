/* Parse Command Line */

#ifndef _MIPARSE_
#define _MIPARSE_


int ParseCmdLine(int argc, char* argv[]);

int GetAddress(char *Str, WORD *WordPtr);
int GetString(char *Str, char *DestStrPtr);


#endif


