/*
 *	Driver for dcpp, sccz80, copt, asz80 and aslink
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define	PREPROCESSOR	"dcpp"
#define	COMPILER	"sccz80"
#define	ASSEMBLER	"asz80"
#define	COPT		"copt"
#define	COPTRULES	"rules"
#define	LINKER		"aslink"
#define	RTSTARTUP	"0crt.rel"

enum iostyle { outimplied, outspecified, filter };

extern void	*malloc();

int	compileonly	= 0;
int	verbose		= 0;
int	peepholeopt	= 0;
int	ncppargs	= 0;
char	**cpparglist;
int	nfiles		= 0;
char	**filelist;
int	nparms		= 0;
char	**parmlist;

void *mustmalloc(n)
	int	n;
{
	void	*p;

	if ((p = malloc(n)) == 0)
	{
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}
	return (p);
}

int hassuffix(name, suffix)
	char	*name, *suffix;
{
	int	nlen, slen;

	nlen = strlen(name);
	slen = strlen(suffix);

	if (slen > nlen)
		return (0);
	return (strcmp(&name[nlen-slen], suffix) == 0);
}

char *changesuffix(name, suffix)
	char	*name, *suffix;
{
	char	*p, *r;

	if ((p = strrchr(name, '.')) == 0)
	{
		r = mustmalloc(strlen(name) + strlen(suffix) + 1);
		sprintf(r, "%s%s", name, suffix);
	}
	else
	{
		r = mustmalloc(p - name + strlen(suffix) + 1);
		r[0] = '\0';
		strncat(r, name, p - name);
		strcat(r, suffix);
	}
	return (r);
}

int process(suffix, nextsuffix, processor, extraargs, ios)
	char	*suffix, *nextsuffix, *processor, *extraargs;
	enum iostyle	ios;
{
	int	i, status, errs;
	char	*buffer = NULL;
	char 	*outname = NULL;

	errs = 0;
	for (i = 0; i < nfiles; ++i)
	{
		if (!hassuffix(filelist[i], suffix))
			continue;
		switch (ios)
		{
		case outimplied:
			buffer = mustmalloc(strlen(processor) + strlen(extraargs)
				+ strlen(filelist[i]) + 3);
			sprintf(buffer, "%s %s %s", processor, extraargs,
				filelist[i]);
			break;
		case outspecified:
			outname = changesuffix(filelist[i], nextsuffix);
			buffer = mustmalloc(strlen(processor) + strlen(extraargs)
				+ strlen(filelist[i]) + strlen(outname) + 4);
			sprintf(buffer, "%s %s %s %s", processor, extraargs,
				filelist[i], outname);
			free(outname);
			break;
		case filter:
			outname = changesuffix(filelist[i], nextsuffix);
			buffer = mustmalloc(strlen(processor) + strlen(extraargs)
				+ strlen(filelist[i]) + strlen(outname) + 8);
			sprintf(buffer, "%s %s < %s > %s", processor, extraargs,
				filelist[i], outname);
			free(outname);
		}
		if (verbose)
			puts(buffer);
		status = system(buffer);
		if ((status >> 8) != 0)
			errs = 1;
		else
			filelist[i] = changesuffix(filelist[i], nextsuffix);
		free(buffer);
	}
	return (errs);
}

int linkthem(linker)
	char	*linker;
{
	int	i, n, status;
	char	*output, *p;

	n = strlen(linker) + 1;
	n += sizeof("-i -m -o");
	output = changesuffix(filelist[0], ".ihx");
	n += strlen(output) + 1;
	for (i = 0; i < nparms; ++i)
		n += strlen(parmlist[i]) + 1;
	n += sizeof(RTSTARTUP);
	for (i = 0; i < nfiles; ++i)
	{
		if (hassuffix(filelist[i], ".rel"))
			n += strlen(filelist[i]) + 1;
	}
	p = mustmalloc(n);
	sprintf(p, "%s -i -m -o %s", linker, output);
	for (i = 0; i < nparms; ++i)
	{
		strcat(p, " ");
		strcat(p, parmlist[i]);
	}
	strcat(p, " ");
	strcat(p, RTSTARTUP);
	for (i = 0; i < nfiles; ++i)
	{
		if (hassuffix(filelist[i], ".rel"))
		{
			strcat(p, " ");
			strcat(p, filelist[i]);
		}
	}
	if (verbose)
		printf("%s\n", p);
	status = system(p);
	free(p);
	return ((status >> 8) != 0);
}

int main(argc, argv)
	int	argc;
	char	**argv;
{
	int	i, n;
	char	*cpparg;

	/* allocate enough pointers for all files, slight overestimate */
	filelist = (char **)mustmalloc(sizeof(char *) * argc);
	/* ditto for -Ddef=val, -Udef, -I, huge overestimate */
	cpparglist = (char **)mustmalloc(sizeof(char *) * argc);
	/* ditto for -bX=Y and -gX=Y, huge overestimate */
	parmlist = (char **)mustmalloc(sizeof(char *) * argc);
	for (i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'b':
			case 'g':
				parmlist[nparms++] = argv[i];
				break;
			case 'c':
				compileonly = 1;
				break;
			case 'D':
			case 'I':
			case 'U':
				cpparglist[ncppargs++] = argv[i];
				break;
			case 'O':
				peepholeopt = 1;
				break;
			case 'v':
				verbose = 1;
				break;
			}
		}
		else
			filelist[nfiles++] = argv[i];
	}
	n = 1;		/* guarantee that we'll malloc something */
	for (i = 0; i < ncppargs; ++i)
		n += strlen(cpparglist[i]) + 1;
	cpparg = mustmalloc(n);
	cpparg[0] = '\0';
	for (i = 0; i < ncppargs; ++i)
	{
		strcat(cpparg, cpparglist[i]);
		if (i < ncppargs - 1)
			strcat(cpparg, " ");
	}
	if (nfiles <= 0)
		exit(0);
	if (process(".c", ".i", PREPROCESSOR, cpparg, outspecified))
		exit(1);
	if (peepholeopt)
	{
		if (process(".i", ".asm", COMPILER, "-x", outimplied))
			exit(1);
		if (process(".asm", ".opt", COPT, COPTRULES, filter))
			exit(1);
		if (process(".opt", ".rel", ASSEMBLER, "", outimplied))
			exit(1);
	}
	else
	{
		if (process(".i", ".asm", COMPILER, "", outimplied))
			exit(1);
		if (process(".asm", ".rel", ASSEMBLER, "", outimplied))
			exit(1);
	}
	if (compileonly)
		exit(0);
	if (linkthem(LINKER))
		exit(1);
	exit(0);
}
