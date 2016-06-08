# Makefile for genroms
#
#  2000-2004
#  yorgle@gmail.com
#  Scott Lawrence
#


################################################################################
# now executable names for compiling...

NAME	:= genroms
VERSION := 1.9

# 1.9  additional output
# 1.8  fixed an addressing bug
# 1.7a text for stats changed

SRCS := \
	io_util.c \
	romreg.c \
	readihx.c \
	romsread.c \
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
	./genroms sample.roms sample.ihx

################################################################################
# utility targets

clean:
	rm -f core so_locations
	rm -f $(TARG) *.so
	rm -f $(OBJS)
	rm -f ic7 ic8 ic14 ic15 ic20 ic21 ic31 ic32
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
