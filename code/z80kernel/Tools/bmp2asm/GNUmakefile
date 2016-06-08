#
#  Makefile for bmp2asm
#
#	for:
#		http://umlautllama.com/project.php?ShortName=Notebook
#		http://umlautllama.com/projects/Notebook/fmv_pac/
#
#  (c)2007 Scott Lawrence
#

TARGET  := bmp2asm

SRCS	:= bmp2asm.c
OBJS	:= $(SRCS:%.c=%.o)

CFLAGS += -g -Wall -pedantic

all: $(TARGET)


$(TARGET): $(OBJS)
	@echo Linking...
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

$.o:	%.c
	@echo CC $<
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning
	@-rm *.o $(TARGET)

install:	$(TARGET)
	cp $(TARGET) ~/sw/bin/

test1:	install
	~/sw/bin/$(TARGET) SCAN JFS_42.bmp

test2:	install
	~/sw/bin/$(TARGET) ASM JFS_42.bmp

