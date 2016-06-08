# Makefile for the patching procedure
#
#  Scott Lawrence
#
#  $Id: Makefile,v 1.40 2007/11/04 18:03:33 sdl Exp $


# main definitions
VERSION	:= 0.05
THISDIR := pacpatch
ROMSDIR := ~/sw/share/roms

#	TARGET		ROMSET		SRCSET		GFX	ALSO
# 	------		------		------		---	----
#
#	fast		mspacmab	mspacmab.orig		pacman
#	cheat		mspacmab	mspacmab.orig
#	l255fix		pacman		pacman.orig
#
#	ramsnoop	mspacmab	mspacmab.orig		pacman
#	pacsnoop	pacman		pacman.orig
#
#	pacNmap		pacman		pac2.orig (extra maps)
#
#	horizpac	mspacmab	mspacmab.orig	horizpac
#	crazyoto	mspacmab	crazyoto.orig	crazyoto
#
#	mpdmspac	mspacmab	mspacmab.orig	mpdmspac/horizpac mspac2
#	mstimed		mspacmab	mspacmab.orig
#
#	pacgent		mspacmab	mspacmab.orig	pacgent
#
#	shtron		tron		shtron.orig
#	dmtron		tron		dmtron.orig
#
#	stripped	puckman		puckman.orig
#	hello		any		any

# define these four based on the above table.
# the .asm file
TARGET  := stripped
# the source graphics
GFXSET  := puckman
# the source romset
SRCSET	:= puckman
# the destination romset
ROMSET	:= puckman


# GFXSET:
#	pacman		original Pac-Man
#	mspacmab	original Ms. Pac-Man
#	crazyoto	Crazy Otto recreation
#	mpdmspac	Ms. Pac-Man with arrows added for the menu
#	horizpac	rotated Ms. Pac-Man + MPD 
#	pacgent		Ms. Pac as Pac Gentleman

# TARGET:  $(TARGET).asm  $(TARGET).zip
# ROMSET:  mame name that the target runs as
# GFXSET:  *5e *5f copied from gfx/$(GFXSET)
# SRCSET:  selectable for A->B transforms.  (this is A)

# executables
MAKE	:= make
GENROMS := genroms
TURACO	:= turacocl
XMAME	:= xmame
XMAMED	:= xmamed
XMAMEARGS := -snapshot_directory . -rompath . -cheat -scale 2 -nomitshm
XMAMEDARGS := $(XMAMEARGS) -debug -nr -ds 640x480 

# the target.asm file
ASM 	:= $(TARGET).asm

# derivative definitions
RELS	:= $(ASM:%.asm=%.rel)
LSTS	:= $(ASM:%.asm=%.lst)
MAPS	:= $(ASM:%.asm=%.map)
IHX	:= $(ASM:%.asm=%.ihx)

# listings of roms
GENERATEDROMS := $(shell $(GENROMS) auxfiles/$(ROMSET).roms -listall)
ORIGROMS := $(ROMSDIR)/*
ROMFILES := $(GENERATEDROMS:%=$(ROMSET)/%)

################################################################################
# the main target
all: roms

# listing: good for verbose debugging
listing: $(LSTS)

# roms: generate the rom files from the .ihx file via 'genroms'
roms:	$(IHX)
	-mkdir $(ROMSET)
	cp $(ROMSDIR)/$(ROMSET)/* $(ROMSET)
	cd $(ROMSET) ; $(GENROMS) ../auxfiles/$(ROMSET).roms ../$(IHX) -patch . 
	cp gfx/$(GFXSET)/*5* $(ROMSET)/

# test: generate the roms, try them in MAME
# NOTE: we shouldn't need 'clean' in here, but my makefile skillz suck, so
#       we eat the extra 1.1 seconds required to rebuild the romset

test:	mametest

idetest:	clean roms
	-cp mspac2/* /Users/sdl/src/cvs/llemonide/mspac2
	-cd /Users/sdl/src/cvs/llemonide; make test
	
mametest: clean roms
	$(XMAME) $(XMAMEARGS) $(ROMSET)

debug:	clean roms
	$(XMAMED) $(XMAMEDARGS) $(ROMSET)

################################################################################
# figure out a better way to do this later

gfxroms:
	cd gfx/$(GFXSET) ; \
	$(TURACO) -ini ../../auxfiles/$(ROMSET).ini \
		  -rod . \
		  -rom . -bnk 1 -inf IMG
	cd gfx/$(GFXSET) ; \
	$(TURACO) -ini ../../auxfiles/$(ROMSET).ini \
		  -rod . \
		  -rom . -bnk 2 -inf IMG

pcxfiles:
	cd gfx/$(GFXSET) ; \
	$(TURACO) -ini ../../auxfiles/$(ROMSET).ini \
		  -rod . \
		  -rom . -bnk 1 -inf ROM
	cd gfx/$(GFXSET) ; \
	$(TURACO) -ini ../../auxfiles/$(ROMSET).ini \
		  -rod . \
		  -rom . -bnk 2 -inf ROM


################################################################################
# list version of asm files via asz80
%.lst: %.asm
	asz80 -l $<

# ihx file from rel files via aslink
$(IHX): $(RELS)
	aslink -i -m -o $(IHX) $(RELS)

# rels from asms via asz80
%.rel: %.asm
	asz80 $<

################################################################################
# utility targets

# clean: remove extra files
clean:
	rm -rf $(IHX) $(RELS) $(LSTS) $(MAPS) $(ROMFILES)
	rm -rf $(ROMSET)
	rm -f *.ihx *.rel *.lst *.map
.PHONY: clean

# clobber: remove all generated files
clobber: clean
	rm -rf $(PROGROMFILESPATH) $(ROMSET).zip
.PHONY: clobber
	
# backup: tar-gzip the source tree
backup: clean
	cd .. ; tar -cvf $(THISDIR)_$(VERSION).tar $(THISDIR)
	gzip -f ../$(THISDIR)_$(VERSION).tar
.PHONY: backup

# winbackup: zip the source tree
winbackup: clean
	cd .. ; zip -rp $(THISDIR)_$(VERSION).zip $(THISDIR)

dist: roms
	zip -rp $(TARGET).zip $(ROMFILES)
