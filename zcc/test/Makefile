hello.ihx:	hello.c 0crt.rel
	PATH="../bin:$$PATH" zcc -O -v -b_CODE=0x8000 -b_BSS=0x9000 -b_STACK=0x9f00 hello.c

x.ihx:	x.c 0crt.rel
	PATH="../bin:$$PATH" zcc -O -v -b_CODE=0x8000 -b_BSS=0x9000 -b_STACK=0x9f00 x.c

0crt.rel:	0crt.asm
	../bin/asz80 0crt.asm

flashled.rel:	flashled.asm
	../bin/asz80 -c flashled.asm

clean:
	rm -f 0crt.rel \
	hello.asm \
	hello.i \
	hello.ihx \
	hello.map \
	hello.opt \
	hello.rel
