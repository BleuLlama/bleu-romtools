#
#  Makefile for comPACtor
#
#  (c)2007 Scott Lawrence
#
#	free for whatever use
#

TARGET  := comPACtor

SRCS	:= main.c palette.c
OBJS	:= $(SRCS:%.c=%.o)

CFLAGS += -g -Wall -pedantic $(shell sdl-config --cflags)
LDFLAGS += $(shell sdl-config --libs) -lSDL_image

all: $(TARGET)


$(TARGET): $(OBJS)
	@echo Linking...
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

$.o:	%.c
	@echo CC $<
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning
	@-rm *.o $(TARGET)

install:	$(TARGET)
	cp $(TARGET) ~/sw/bin/

test:	install
	~/sw/bin/$(TARGET) -Scan -Image JFS_42.bmp

test2:	install
	~/sw/bin/$(TARGET) -Asm -Image JFS_42.bmp
