# Unix
CC=	gcc
CFLAGS=	-O -I. $(DEBUG) -Wall
DEBUG=
O=	.o
OUT=	-o $@
MFLAGS=	CC=$(CC) CFLAGS='$(CFLAGS)' DEBUG='$(DEBUG)' O=$(O) OUT='$(OUT)'
RM=	rm -f
cd=	cd
submake=@true

# Borland
# also look in submake.bat if you change any of the following
#CC=	tcc
#CFLAGS=	-O
#O=	.obj
#E=	.exe
#OUT=	-e$<
#RM=	del
#cd=	@rem
#submake=submake

.c$(O):
	$(CC) $(CFLAGS) -c $<

all:	dcpp$(E) sccz80$(E) copt$(E) asz80$(E) aslink$(E) zcc$(E)

install:	all
	mkdir -p bin
	cp pp/dcpp bin
	cp scc/sccz80 bin
	cp pho/copt bin
	cp as/asz80 bin
	cp link/aslink bin
	cp zcc bin


# Stupid DOS del won't handle more than one argument though
clean:
	$(RM) pp/dcpp$(E) scc/sccz80$(E) pho/copt$(E) as/asz80$(E) link/aslink$(E) zcc$(E)
	$(RM) */*.o
	$(RM) -r bin

dcpp$(E):
	$(cd) pp; make $(MFLAGS) dcpp$(E)
	$(submake) pp dcpp$(E)

sccz80$(E):
	$(cd) scc; make $(MFLAGS) sccz80$(E)
	$(submake) scc sccz80$(E)

copt$(E):
	$(cd) pho; make $(MFLAGS) copt$(E)
	$(submake) pho copt$(E)

asz80$(E):
	$(cd) as; make $(MFLAGS) asz80$(E)
	$(submake) as asz80$(E)

aslink$(E):
	$(cd) link; make $(MFLAGS) aslink$(E)
	$(submake) link aslink$(E)

zcc$(E):	zcc.c
	$(CC) $(CFLAGS) $(OUT) zcc.c
