.c$(O):
	$(CC) $(CFLAGS) $(DEBUG) -c $<

AOBJSD=	asdata$(O) \
	asexpr$(O) \
	aslex$(O) \
	aslist$(O) \
	asmain$(O) \
	asout$(O) \
	assubr$(O) \
	assym$(O) $(G)

AOBJS=	as*$(O) $(G)

ZOBJSD=	z80adr$(O) \
	z80ext$(O) \
	z80mch$(O) \
	z80pst$(O)

ZOBJS=	z8*$(O)

IOBJSD=	i85ext$(O) \
	i85mch$(O) \
	i85pst$(O)

IOBJS=	i8*$(O)

all:	asz80$(E) as8085$(E)

asz80$(E):	$(AOBJSD) $(ZOBJSD)
	$(CC) $(DEBUG) $(OUT) $(AOBJS) $(ZOBJS)

as8085$(E):	$(AOBJSD) $(IOBJSD)
	$(CC) $(DEBUG) $(OUT) $(AOBJS) $(IOBJS)

$(AOBJSD):	alloc.h asm.h

$(ZOBJSD):	z80.h

$(IOBJSD):	i8085.h
