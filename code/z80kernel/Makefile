# Makefile for the z80 kernel
#
#  	Scott Lawrence
#	yorgle@gmail.com
#
#	http://umlautllama.com


# main definitions
VERSION	:= 0.07
VERS    := 007

# 0.07 has updates for version numbers for tasks
# 0.06 was a mid-version 2012 for Ludum Dare 23
# 0.05 was stagnant for years

# ROMSET can be mspac2 or mspacmab
ROMSET	:= mspac2
THISDIR	:= z80kernel

MAKE	:= gmake
GENROMS	:= genroms

# for testing and debugging
XMAME	:= xmame
XMAMEARGS := -snapshot_directory . -rompath .  -cheat -scale 2 -nomitshm
XMAMED	:= xmamed
XMAMEDARGS := $(XMAMEARGS) -cheat -debug -nr -ds 640x480 -nomitshm


##############################
# source ASM files

# first, the tasks

MUSICPLAYER :=	\
		Tasks/MusicPlayer/pactracker.asm \
		Tasks/MusicPlayer/songs.asm

ABSURD	:= \
		Tasks/Absurd/absurd.asm	\
		Tasks/Absurd/abmaps.asm

MOVIEPLAYER := \
		Tasks/MoviePlayer/movie.asm \
		Core/highrom.asm \
		Tasks/MoviePlayer/journey.asm

QUADBLOK := \
		Tasks/QuadBlok/quadblok.asm

SCREENSAVER := \
		Tasks/ScreenSaver/ssaver.asm

TESTMODE := \
		Tasks/TestMode/testmode.asm

MUXSPRITE := \
		Tasks/MuxSprite/muxsprite.asm

EXAMPLE := \
		Tasks/Example/example.asm
	
TASKS   := 	Tasks/tasklist.asm\
		\
		$(MUXSPRITE) \
		$(ABSURD) \
		$(QUADBLOK) \
		$(SCREENSAVER) \
		$(TESTMODE) \
		\
		$(MUSICPLAYER) \
		$(MOVIEPLAYER)

# all of the .asm files, in order...
# the order of really just the "header" is critical, the remaining ones
# /can/ be moved around, but it works, so why bother, eh?
ASMS 	:=	\
		Core/header.asm		\
                Core/hardware.asm       \
		Core/settings.asm	\
                Core/ram.asm            \
                \
                Core/core.asm           \
		version.asm		\
		Core/tasks.asm		\
		Core/dips.asm		\
		Core/input.asm		\
                Core/memset.asm         \
		Core/clear.asm		\
                Core/at.asm		\
		Core/bcdmath.asm	\
                Core/rand.asm           \
                Core/sine.asm           \
		Core/sprites.asm	\
		Core/sleep.asm		\
		Core/text.asm		\
		Core/rle.asm		\
		\
		Core/coresize.asm	\
		\
		Core/rletest.asm 	\
		\
		Core/cflscott.asm	\
		Core/title.asm		\
		Core/main.asm		\
		\
		$(TASKS)

# drop this in to define where the MsPac highrom starts
# it is basically just an:  .org 0x8000
NOTUSED2 :=	\
		Core/highrom.asm

# ROMFILES: this should really be dumped out from 'genroms' with a switch there
ROMFILES := $(shell $(GENROMS) AuxFiles/$(ROMSET).roms -listall)
ROMFILESPATH := $(ROMFILES:%=$(ROMSET)/%)

PROGROMFILES := $(shell $(GENROMS) AuxFiles/$(ROMSET).roms -list)
PROGROMFILESPATH := $(PROGROMFILES:%=$(ROMSET)/%)

# derivative definitions
IHX	:= $(ROMSET).ihx
GENASM	:= $(ROMSET).asm
RELS	:= $(GENASM:%.asm=%.rel)
LSTS	:= $(GENASM:%.asm=%.lst)
MAPS	:= $(GENASM:%.asm=%.map)


################################################################################
# the main target
all:	fullRomSet

# listing: good for verbose debugging
listing: $(LSTS)

# test: generate the roms, try them in MAME
# NOTE: we shouldn't need 'clean' in here, but my makefile skillz0rz suck, so
#       we eat the extra 1.1 seconds required to rebuild the romset
test:	fullRomSet
	@echo - Mame $(ROMSET)
	@$(XMAME) $(XMAMEARGS) $(ROMSET)
.PHONY: test

debug:	fullRomSet
	@echo - Mame Debug $(ROMSET)
	@$(XMAMED) $(XMAMEDARGS) $(ROMSET)
.PHONY: testd


################################################################################
# version.asm ; our generated version string
# this can then be used within the romset

version.asm: Makefile
	@echo Generating version.asm
	@echo "VERSION:" > version.asm
	@echo "	.byte	20" >> version.asm
	@echo "	.ascii	\"BL Game Kernel v$(VERSION)\"" >> version.asm


################################################################################
# list version of asm files via asz80
%.lst: %.asm
	@echo + Generating listing file $@
	@asz80 -l $<

# ihx file from rel files via aslink
$(IHX): $(RELS)
	@echo + aslink
	@aslink -i -m -o $(IHX) $(RELS)

# generated asm file from asm files via cat
$(GENASM): $(ASMS)
	@echo + Combining ASMS into $(GENASM)
	@cat $(ASMS) > $(GENASM)

# rels from asms via asz80
%.rel: %.asm
	@echo + asz80 $<
	@asz80 $<

%.asm: %.abm
	@echo Generating Map Data ASM file
	@perl Tools/abm2asm.pl $< $@


################################################################################
# base romset

# baseroms: the initial 0-filled roms, proms, etc.
baseroms:
	@echo + Generating foundation romset $(ROMSET)
	@-rm -rf $(ROMSET)
	@-mkdir $(ROMSET)
	@cp BaseRoms/$(ROMSET)/* $(ROMSET)
.PHONY: baseroms


# kernelRoms: generate the rom files from the .ihx file via 'genroms'
kernelRoms:	$(IHX)
	@echo + Generating $(ROMSET) rom set from $(IHX)
	@cd $(ROMSET) ; $(GENROMS) ../AuxFiles/$(ROMSET).roms ../$(IHX)


# graphics - via turaco
GRCMDS	:=	-ini ../AuxFiles/mspacmab.ini \
                -rom ../$(ROMSET) \
		-rod ../$(ROMSET) \
		-inf IMG

gfxRoms: baseroms
	@echo + Generating $(ROMSET) Graphics Roms
	@echo + Building 5e
	@cd Graphics; turacocl $(GRCMDS) -bnk 1 >/dev/null
	@echo + Building 5f
	@cd Graphics; turacocl $(GRCMDS) -bnk 2 >/dev/null
.PHONY: gfxRoms


fullRomSet: baseroms gfxRoms kernelRoms
.PHONY: fullRomSet

size: listing
	@echo Size listing:
	@perl Tools/sizes.pl

################################################################################
# utility targets

# clean: remove extra files
clean:
	@echo - Removing zip, bin, generated asm, etc.
	@-rm -rf $(ROMSET)
	@rm -f $(THISDIR)_$(VERSION)M.zip $(ROMSET)_$(VERS)M.zip
	@rm -f *.bin *.png version.asm Tasks/Absurd/abmaps.asm
	@rm -rf $(IHX) $(RELS) $(LSTS) $(MAPS) $(GENASM)
.PHONY: clean

# clobber: remove all generated files
clobber: clean
.PHONY: clobber
	

DATEFILE := $(THISDIR)_$(VERSION)_$(shell date +"%Y-%m-%d_%H.%M.%S")

# backup: tar-gzip the source tree
backup: clean
	@echo + Backing up to ../$(DATEFILE).tar.gz
	@cd .. ; tar -cf $(DATEFILE).tar $(THISDIR)
	@gzip -f ../$(DATEFILE).tar
.PHONY: backup


# back up everything to zip files
winbackup: clean
	@echo + Backing up to ../$(THISDIR)_$(VERSION)M.zip
	@cd .. ; zip -rp $(THISDIR)_$(VERSION)M.zip $(THISDIR)
.PHONY: winbackup

# make a distribution of the current romset
dist: fullRomSet listing
	@echo + Making zip file of source and roms
	@cp $(ROMSET).lst readme.txt whatsnew.txt $(ROMSET)
	@zip $(ROMSET)_$(VERS)M.zip $(ROMSET)/*
.PHONY: dist

web: dist
	@echo + uploading to patsy.cis.rit.edu...
	@scp $(ROMSET)_$(VERS)M.zip p:public_html/Software/Absurd/romsets/
	#  http://yorgle.cis.rit.edu/Software/Absurd/romsets/
.PHONY: dist
