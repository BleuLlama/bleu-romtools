#
# Very simple makefile for unix builds
#
#  Modified 2008-12 by Scott Lawrence
#	- files renamed to be lowercase
#	- build flag added to switch between "malloc.h" and "stdlib.h"

#		*Translator Definitions*
CC = cc -g

#		*List Macros*

TARG := dz80

EXE_dependencies =  \
  loadfl32.o \
  parsecmd.o \
  dissz80.o \
  dz80.o

CFLAGS := -DNEW_BUILD -Wall -pedantic

all:	$(TARG) install

install: $(TARG)
	@-mkdir bin
	cp $(TARG) bin/
.PHONY: install
	 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


#		*Explicit Rules*
$(TARG): $(EXE_dependencies)
	$(CC) -o $(TARG) $(EXE_dependencies)

clean:
	-rm -rf $(TARG) $(EXE_dependencies) bin

