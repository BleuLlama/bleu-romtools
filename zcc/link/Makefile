.c$(O):
	$(CC) $(CFLAGS) $(DEBUG) -c $<

LOBJSD=	lkarea$(O) \
	lkdata$(O) \
	lkeval$(O) \
	lkhead$(O) \
	lkihx$(O) \
	lklex$(O) \
	lklist$(O) \
	lkmain$(O) \
	lkrloc$(O) \
	lks19$(O) \
	lksym$(O) 

LOBJS=	lk*$(O)

all:	aslink$(E)

$(LOBJSD):	aslink.h

aslink$(E):	$(LOBJSD)
	$(CC) $(DEBUG) $(OUT) $(LOBJS)
