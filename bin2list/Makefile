# Makefile for bin2list
#
#  2000-2019
#  yorgle@gmail.com
#  Scott Lawrence
#


################################################################################
# now executable names for compiling...

NAME	:= bin2list
VERSION := 1.0

# 1.0 	initial version

SRCS := \
	main.c


##############################

CC	:= gcc
OBJS    := $(SRCS:%.c=%.o)

TARG	:= $(NAME)

LDFLAGS := -L. 

CFLAGS	:= -g \
		-Wall \
		-funroll-loops -fomit-frame-pointer -ffast-math \
		-D_REENTRANT -DJ_APPNAME=\"$(NAME)\" \
		-DJ_VERSION=\"$(VERSION)\" 


##############################

all: exe
.PHONY: all

exe: $(TARG)

# we strip the execuable because we don't want it to get too proud
# and wear ornate cloting.  Think of the scenes in "The Dark Crystal"
$(TARG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
.SECONDARY: $(OBJS)


test: $(TARG)
	./bin2list ./bin2list -basic900

install: $(TARG)
	-mkdir bin
	cp $(TARG) bin/

################################################################################
# utility targets

clean:
	-rm -f core so_locations
	-rm -f $(TARG) *.so
	-rm -f $(OBJS)
	-rm -rf bin/
	-rm -f ic7 ic8 ic14 ic15 ic20 ic21 ic31 ic32
.PHONY: clean


################################################################################
# some targets for backing up the whole mess, and making tarballs.

FLOPPY   := /mnt/floppy
BAK_NAME := $(NAME)_$(VERSION)

backup: clean
	touch ../$(BAK_NAME).tar.gz
	mv ../$(BAK_NAME).tar.gz ../$(BAK_NAME)_old.tar.gz
	cd .. ; tar -cvf $(BAK_NAME).tar $(NAME)/* ; gzip $(BAK_NAME).tar
.PHONY: backup

floppy:	backup
	mount $(FLOPPY)
	cp ../$(BAK_NAME).tar.gz $(FLOPPY)
	sync
	umount $(FLOPPY)
.PHONY: floppy
