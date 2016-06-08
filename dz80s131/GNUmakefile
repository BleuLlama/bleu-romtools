#
# Very simple makefile for unix builds
#
#  Modified 2008-12 by Scott Lawrence
#	- files renamed to be lowercase
#	- build flag added to switch between "malloc.h" and "stdlib.h"

#		*Translator Definitions*
CC = cc -g

#		*List Macros*

EXE_dependencies =  \
  loadfl32.o \
  parsecmd.o \
  dissz80.o \
  dz80.o

CFLAGS := -g\
	-DNEW_BUILD
	 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


#		*Explicit Rules*
dz80: $(EXE_dependencies)
	$(CC) -o dz80 $(EXE_dependencies)

clean:
	-rm -f dz80 $(EXE_dependencies)
