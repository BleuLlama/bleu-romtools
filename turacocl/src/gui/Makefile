# Makefile for gui
#	- test app and gui library
#  yorgle@gmail.com
#


NAME    := testapp
DIRNAME := gui

# version number gets bumped when:
#  A) a release of some kind is made
#  B) another 'round' of development has started
#  C) a significant amount of changes have been made

VERSNUM := 011
VERSSTR := "2008.02.24 r011"

# 011 2008-02-24  integration into the google code project - last standalone
# 010 2008-02-21  a-Z frq sorting
# 009 2008-02-20  menu/status pulled out, file requestor (fed-style)
# 008 2008-02-17  popup_requestor, llama, mirror
# 007 2008-02-16  new bits structure (pointer), conway, swap display
# 006 2008-02-14  bitmap structure work
# 005 2008-02-12  bitmap and color editor, slow? refresh, bitmap functions
# 004 2008-02-12  broke out color, bitmap, multiplatform builds
# 003 2008-02-10  handle passed around, generalized a bit
# 002 2008-02-08  function names commonized, global struct created
# 001 2008-02-05  initial version, everything global, rough proof of concept


##########################################################
# this next bit makes more sense in the TuracoCL makefile

#ARCH := MSDOS
ARCH := NetBSD


##########################################################
# set up some main variables

TARGET  := $(NAME)
DNCURSES := -DUSE_CURSES
LIBS += -lcurses


##########################################################
# source files

SRCS := test.c \
	app.c   \
	bits.c   \
	popup.c  \
	menu.c   \
	status.c \
	filereq.c \
	tmgui.c

SAMPLES := 

OBJS := $(SRCS:%.c=%.o)


##########################################################
# main target

all:	$(TARGET)

$(TARGET): $(SRCS)


##########################################################
# stuff for various helpful targets

tidy:
	@echo Cleaning up...
	rm -f $(TARGET).core
	rm -f $(TARGET)
	rm -f $(OBJS)

clean:	tidy


backup: clean
	@echo Making backup...
	-cd .. ; cp $(TARGET)-$(VERSNUM).tgz $(TARGET)-$(VERSNUM)_bak.tgz 
	cd .. ; tar -zcvf $(TARGET)-$(VERSNUM).tgz $(DIRNAME)

floppy: 
	cp ../$(TARGET)-$(VERSNUM).tgz /mnt/msdos/
	sync

floppy2: 
	@echo Copying backup to second CF card
	-mount /mnt/msdos_left
	-cp ../$(TARGET)-$(VERSNUM).tgz /mnt/msdos_left/
	sync
	-umount /mnt/msdos_left
	-rm -f /mnt/msdos_left/$(TARGET)-$(VERSNUM).tgz
	@mount | grep msdos2
	@echo Done.


##########################################################
# test targets

test: all
	./$(NAME)


##########################################################
# build targets (important stuff)

CC  := gcc
CXX := gcc


COMMONFLAGS := -O -Wall -pedantic -g \
	-D$(ARCH) \
	-DBL_VERSION=\"$(VERSSTR)\" \
	-DBL_DATE=\"\" \
	-DBL_PNAME=\"TestApp\" \
	$(DNCURSES) 

CXXFLAGS := $(COMMONFLAGS) -mt -instances=static 

CFLAGS := $(COMMONFLAGS) -DTEST_APP

LDFLAGS := -g 

NLIBS := 


##########################################################
# build
.c.o:
	@echo $(CC) $<
	@$(CC) $(CFLAGS) -o $@ -c $<


##########################################################
# link!

$(TARGET):	$(OBJS)
	@echo $(CXX) -o $@
	@$(CXX) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)


##########################################################

.PHONY:	all
.PHONY:	tidy
.PHONY:	clean
