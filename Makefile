VERSION=beta0.0.1
CC=gcc
DESTDIR=
PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin
MANDIR?=$(PREFIX)/share/man
OUTDIR=.build
SRCDIR=./src
LIBDIR=./lib
DOCDIR=./docs

.PHONY: all clean install uninstall

OBJECTS = \
					$(SRCDIR)/hdbcli.c

OBJECTSCLIENT = \
					$(SRCDIR)/hdbclient.c

OBJECTMAN = \
					$(DOCDIR)/hdb.man

all: hdb hdb-client

%(OUTPUT)/%.o: $(SRCDIR)/%.c
		@mkdir -p $(OUTDIR)	
		$(CC) -o 

hdb: $(OBJECTS)
		$(CC) -o $@ $^

hdb-client: $(OBJECTSCLIENT)
		$(CC) -o $@ $^

clean: 
		rm -f hdbcli.o hdbclient.o

install: all
		install -m755 hdb $(BINDIR)/hdb
		install -m755 hdb-client $(BINDIR)/hdb-client
		cp $(OBJECTMAN) $(MANDIR)/man1/hdb.1

uninstall: all
		rm $(BINDIR)/hdb
		rm $(BINDIR)/hdb-client
