                         dZ80 Version 1.31 Source Code

                       Copyright 1996-1998 Mark Incley.

                       E-mail: dz80@inkland.demon.co.uk
                        http://www.inkland.demon.co.uk


Serious Bit
-----------

I have made this source code available so that it may be compiled on platforms
other than MS-DOS. You may compile it and distribute the resulting executable
only if no monies are charged for it.

       ** YOU ARE NOT ALLOWED TO DISTRIBUTE THIS SOFTWARE COMMERICIALLY **


Not So Serious Bit
------------------

If you make any feature modifications to the dZ80 source code, please let me
know, so that I can make them to my source too. I didn't intend for dZ80 to
grow into an all singing and dancin' disassembler, but, if features are added,
I would like to add them to the Messy-Dos version as well.


Source Code Notes
-----------------

File            Purpose
----            -------
TYPES.H         Sets up the typedefs
DZ80.C/H        Main entry point   
DISSZ80.C/H     The disassembler
PARSECMD.C/H    Command line parser
LOADFL16.C      File loader and memory allocator (16-bit DOS only - see below)
LOADFL32.C      File loader and memory allocator (standard C)
DZ80.MAK        GNU makefile for 32-bit version of dZ80
MAKEFILE        Borland makefile for 16-bit version of dZ80
MAKEFILE.UNX    Simple makefile for unix builds

SKIP.C          A self-contained dZ80 opcode map file generator by
                Raffaele Sena

As I intended for dZ80 to work on any PC (maybe 'cos I have a 286 portable :)
I originally wrote it as a 16-bit DOS app. These type of DOS apps are a major
pain when allocating and manipulating blocks of memory >= 64K, so, as you can
see, to make the source more portable, I have two versions of the memory
allocating and file loading module, named LOADFLxx.C where xx is either 16 or
32. LOADFL16 is for the 16-bit DOS version only, and uses non-standard Borland
macros to manipulate the >= 64K blocks, and it's supplied only for reference.
All other versions will use LOADFL32 which uses standard C stuff.

You may see the HUGE modifier used in other modules. This allows me to access
these large (well, to DOS, anyway!) memory blocks. The TYPES.H file will
define "HUGE" as nothing for platforms other than 16-bit DOS, so they should
ignore this modifer.

If you have any problems, just e-mail me on dz80@inkland.demon.co.uk

Cheers,

        Mark
