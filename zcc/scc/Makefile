.c$(O):
	$(CC) $(CFLAGS) $(DEBUG) -c $<

COBJSD=	codez80$(O) \
	data$(O) \
	error$(O) \
	expr$(O) \
	function$(O) \
	gen$(O) \
	io$(O) \
	lex$(O) \
	main$(O) \
	preproc$(O) \
	primary$(O) \
	stmt$(O) \
	sym$(O) \
	while$(O)

COBJS=	*$(O)

all:	sccz80$(E)

sccz80$(E):	$(COBJSD)
	$(CC) $(DEBUG) $(OUT) $(COBJS)

$(COBJSD):	data.h defs.h
