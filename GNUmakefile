# Makefile for romtools


SUBDIRS := \
        dz80s131 \
        genroms \
		bin2list \
	turacocl \
	zcc 

BINDIR := bin

all:    bindir tools bincollect

bindir:
	@echo Creating bin directory...
	@-mkdir $(BINDIR)

tools: bindir
	@echo "Building tools..."
	for dir in $(SUBDIRS); do \
               $(MAKE) -C $$dir install ; \
             done

bincollect: tools
	@echo "Copying to bin..."
	for dir in $(SUBDIRS); do \
		cp $$dir/bin/* $(BINDIR); \
             done

clean:
	@echo "Cleaning up all products"
	@-rm -rf $(BINDIR)
	for dir in $(SUBDIRS); do \
               $(MAKE) -C $$dir clean; \
             done
