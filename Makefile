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

OBJECTMAN = \
					$(DOCDIR)/hdb.man

all: hdb 

%(OUTPUT)/%.o: $(SRCDIR)/%.c
		@mkdir -p $(OUTDIR)
		$(CC) -o

hdb: $(OBJECTS)
		$(CC) -o $@ $^ -llua -ldl -lm

clean:
		rm -f hdbcli.o 

install: all
		install -m755 hdb $(BINDIR)/hdb
		mkdir -p $(MANDIR)/man1
		cp $(OBJECTMAN) $(MANDIR)/man1/hdb.1

uninstall: all
		rm $(BINDIR)/hdb
		rm $(MANDIR)/man1/hdb.1
