
CFLAGS += -Wimplicit-function-declaration 

.c$(O):
	$(CC) $(CFLAGS) $(DEBUG) -c $<

OBJS=	cpp1$(O) cpp2$(O) cpp3$(O) cpp4$(O) cpp5$(O) cpp6$(O)

dcpp$(E):	$(OBJS)
	$(CC) $(DEBUG) $(OUT) $(OBJS)

clean:
	rm -f *$(O)

clobber:	clean
	rm -f core cpp

$(OBJS):	cpp.h cppdef.h
