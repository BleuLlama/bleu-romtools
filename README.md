# bleu-romtools

A collection of tools I've created or compiled to help you edit,
hack, and work with arcade ROM files.

This is currently release 2.00 of the bleu-romtools package.  This
version is updated from the google-code repository.  It is in the 
process of being updated to build cleanly for modern systems.

The initial checkin of this reposotory is the exact same content as
what was last on google-code.

The Bleu-Romtools are a set of tools that I've created over the
years that have helped me create and edit ROMs for Arcade game
hardware. Some of these tools will work with other systems as well.
(Mainly Z80 based systems)

## Build...

To build this, you will need to have a gcc-based toolchain installed,
gnu make, gcc, or llvm, etc... 

in this main directory you should be able to just type "make" and it
will build all of the tools, not including Turaco since it's an old DOS
based application, although Turcao CL will build. 

All of the tools will be copied into the bin/ directory, which you can
either add to your path, or copy to a bin directory of your choice thats
already in your path.

Do note that turaco may require setting up some environment variables
to operate properly.


## Genroms

Genroms will take the output from your assembler/compiler as an
Intel Hex File (IHX/HEX), and pad it out into a defined ROM space.
It will also pre-load the ROM space with ROM image files, so that
you can make quick, reproducible patches for ROMs

## TuracoCL

TuracoCL is a portable command line reimplementation of the
TuracoClassic tool. It has no graphical interface (yet), and is
meant to be run in an automated fashion from the command line or a
makefile, which was not possible with older versions of this codebase.
It loads and saves PCX and ROM file formats. Current development
efforts will add an optional curses-based user interface, to allow
for ROM editing from shell windows.

## zcc / asz80

ZCC is a small-c based compiler. It works reasonably for the platform,
but the main item I use out of it is its included assembler, "asz80"

## dz80

dz80 is a Z80 binary disassembler.  It's a great tool for comparing
two ROMs or as a starting point for a new project for generating
reference.

## Turaco

TuracoClassic is a DOS-based sprite image editor for arcade graphics
ROMs. It is quite old, and only works on DOS. It is highly based
on AGE, but mostly re-implemented from scratch with future expansion
and flexibility in mind. TuracoClassic requires MS-DOS libraries
and has no command-line interface to control it. Note: this does
work really well under DosBox or Boxer emulation.

## AGE

AGE is an even older DOS-based sprite image editor, like Turaco.
It has a primitive graphical interface, and a Pac-Man map editor.
It has its shortcomings, but works. (Untested in DosBox/Boxer.)
