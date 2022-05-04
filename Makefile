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
					$(SRCDIR)/main.c

all: hdb

%(OUTPUT)/%.o: $(SRCDIR)/%.c
		@mkdir -p $(OUTDIR)	
		$(CC) -o 

hdb: $(OBJECTS)
		$(CC) -o $@ $^

clean: 
		rm -f main.o

install: all
		install -m755 hdb $(BINDIR)/hdb

uninstall: all
		rm -rf $(BINDIR)/hdb/
