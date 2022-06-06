VERSION=beta0.0.1
CC=gcc
DESTDIR=
PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin
MANDIR?=$(PREFIX)/share/man
OUTDIR=.build
SRCDIR=./src
LIBDIR=./lib

.PHONY: all clean install uninstall

OBJECTS = \
					$(SRCDIR)/hdbcli.c

OBJECTSCLIENT = \
					$(SRCDIR)/hdbclient.c

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

uninstall: all
		rm $(BINDIR)/hdb
		rm $(BINDIR)/hdb-client
