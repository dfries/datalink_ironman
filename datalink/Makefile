# Do not modify this - modify PREFIX (below) instead.  This is used by
# package manages such as that for Debian.
DESTDIR = 


# Edit the parameters below for your needs.

PREFIX=$(DESTDIR)/usr/local
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib
MANDIR=$(PREFIX)/man
INCDIR=$(PREFIX)/include

CC = gcc
#CFLAGS = -Wall -g -DBINDIR=\"$(BINDIR)\"
CFLAGS = -Wall -O2 -DBINDIR=\"$(BINDIR)\"

PORT = /dev/ttyS1

ENGINES = svgablink serblink

RANLIB = echo
#RANLIB = ranlib

# Do not edit below here, no user serviceable parts.

SRC = dl_add_to_list.c dl_anniv_by_date.c dl_anniv_by_label.c \
	dl_app_by_datetime.c dl_app_by_label.c dl_docrc.c dl_fill_pack_ascii.c \
	dl_free_download.c dl_init_download.c dl_init_watch.c dl_item_ok.c \
	dl_new_item.c dl_new_list.c dl_pack_ascii.c dl_pack_char.c \
	dl_pack_digit.c dl_pack_phone.c dl_pack_size.c dl_phone_by_label.c \
	dl_send_data.c dl_set_error.c dl_set_warn.c dl_sizeof_item.c \
	dl_sizeof_list.c dl_sort.c dl_string_ok.c dl_todo_by_label.c \
	dl_todo_by_prio.c dl_write_save.c

OBJ = dl_add_to_list.o dl_anniv_by_date.o dl_anniv_by_label.o \
	dl_app_by_datetime.o dl_app_by_label.o dl_docrc.o dl_fill_pack_ascii.o \
	dl_free_download.o dl_init_download.o dl_init_watch.o dl_item_ok.o \
	dl_new_item.o dl_new_list.o dl_pack_ascii.o dl_pack_char.o \
	dl_pack_digit.o dl_pack_phone.o dl_pack_size.o dl_phone_by_label.o \
	dl_send_data.o dl_set_error.o dl_set_warn.o dl_sizeof_item.o \
	dl_sizeof_list.o dl_sort.o dl_string_ok.o dl_todo_by_label.o \
	dl_todo_by_prio.o dl_write_save.o \
	parse.o dl_read_save.o

SVGASRC = svgablink.c vt.c send_data.c
SERSRC = serblink.c

SVGAOBJ = svgablink.o vt.o send_data.o
SEROBJ = serblink.o

YACC = bison -y -p dl_ -oy.tab.c
LEX = flex -Pdl_ -olex.yy.c

.PHONY: all clean

all: depend libdatalink.so.1 settime setwatch $(ENGINES)

libdatalink.a: $(OBJ)
	rm -f $@
	ar qv $@ $(OBJ)

libdatalink.so.1: $(OBJ)
	rm -f $@
	$(CC) -shared -o $@ $(OBJ) $(CFLAGS)
	$(RANLIB) $@

dl_read_save.c: dataread.y
	$(YACC) -d dataread.y
	mv y.tab.c $@

parse.c: dataread.l dl_read_save.c y.tab.h
	$(LEX) dataread.l
	mv lex.yy.c $@

depend: Makefile
	makedepend -f- -o.o $(CFLAGS) $(SRC) $(SVGASRC) $(SERSRC) > depend
	@echo "Dependencies have been made, re-run make to build"
	@false

serblink.o: $(SERSRC)
	$(CC) -c $(CFLAGS) -DPORT=\"$(PORT)\" $(SERSRC)

serblink: $(SEROBJ)
	rm -f $@
	$(CC) $(CFLAGS) -o $@ $(SEROBJ)

svgablink: $(SVGAOBJ)
	rm -f $@
	$(CC) $(CFLAGS) -o $@ $(SVGAOBJ) -lvga -lm

settime.o: settime.c
	$(CC) $(CFLAGS) -I. -c settime.c 

setwatch.o: setwatch.c
	$(CC) $(CFLAGS) -I. -c setwatch.c 

settime: settime.o libdatalink.a
	$(CC) $(CFLAGS) -o $@ settime.o -L. -ldatalink

setwatch: setwatch.o libdatalink.a
	$(CC) $(CFLAGS) -o $@ setwatch.o -L. -ldatalink

cvt: cvt.o
	$(CC) $(CFLAGS) -o $@

install: all
	mkdir -p $(BINDIR) $(MANDIR)/man1
	install -o root -g video -m 4750 svgablink $(BINDIR)
	install -o root -g root -m 755 serblink $(BINDIR)
	install -o root -g root -m 755 setwatch $(BINDIR)
	install -o root -g root -m 755 settime $(BINDIR)
	install -o root -g root -m 644 setwatch.1 $(MANDIR)/man1
	install -o root -g root -m 644 settime.1 $(MANDIR)/man1
	install -o root -g root -m 644 svgablink.1 $(MANDIR)/man1
	install -o root -g root -m 644 serblink.1 $(MANDIR)/man1

install-devel: install
	mkdir -p $(LIBDIR) $(INCDIR)
	install -o root -g root -m 555 libdatalink.so.1 $(LIBDIR)
	install -o root -g bin -m 555 libdatalink.a $(LIBDIR)
	install -o root -g bin -m 444 datalink.h $(INCDIR)

uninstall:
	rm -f $(BINDIR)/svgablink
	rm -f $(BINDIR)/serblink
	rm -f $(BINDIR)/setwatch
	rm -f $(BINDIR)/settime
	rm -f $(MANDIR)/man1/setwatch.1
	rm -f $(MANDIR)/man1/settime.1
	rm -f $(MANDIR)/man1/svgablink.1
	rm -f $(MANDIR)/man1/serblink.1
	rm -f $(LIBDIR)/libdatalink.so.1
	rm -f $(LIBDIR)/libdatalink.a
	rm -f $(INCDIR)/datalink.h

clean:
	rm -f $(OBJ) libdatalink.a libdatalink.so.1 Makefile.bak
	rm -f $(SVGAOBJ) $(SEROBJ)
	rm -f parse.c dl_read_save.c y.tab.h
	rm -f settime.o setwatch.o cvt.o
	rm -f settime setwatch cvt $(ENGINES)
	rm -f depend

# The next line was used for makedepend
include depend
# DO NOT DELETE
