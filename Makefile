VERSION=beta0.0.1
OUTDIR=/usr/local/

all: lib.h datastruct.c main.c

clean: 
		rm -f main.o

install: all
		mkdir $(OUTDIR)/hdb/
		install -m755 main.c $(OUTDIR)/hdb/hdbmain

uninstall: all
		rm -rf $(OUTDIR)/hdb/
