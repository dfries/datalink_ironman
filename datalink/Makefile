# Edit the parameters below for your needs.

PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib
INCDIR=$(PREFIX)/include

CFLAGS = -g -DBINDIR=\"$(BINDIR)\"
#CFLAGS = -O2 -DBINDIR=\"$(BINDIR)\"

PORT = /dev/ttyS0

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


all: libdatalink.a libdatalink.so.1 settime setwatch $(ENGINES)

libdatalink.a: $(OBJ)
	rm -f $@
	ar qv $@ $(OBJ)

libdatalink.so.1: $(OBJ)
	rm -f $@
	$(CC) -shared -o $@ $(OBJ)
	$(RANLIB) $@

dl_read_save.c: dataread.y
	$(YACC) -d dataread.y
	mv y.tab.c $@

parse.c: dataread.l dl_read_save.c y.tab.h
	$(LEX) dataread.l
	mv lex.yy.c $@

depend: parse.c dl_read_save.c
	makedepend -o.o $(CFLAGS) $(SRC) $(SVGASRC) $(SERSRC)

serblink.o: $(SERSRC)
	cc -c $(CFLAGS) -DPORT=\"$(PORT)\" $(SERSRC)

serblink: $(SEROBJ)
	rm -f $@
	$(CC) $(CFLAGS) -o $@ $(SEROBJ)

svgablink: $(SVGAOBJ)
	rm -f $@
	$(CC) $(CFLAGS) -o $@ $(SVGAOBJ) -lvga

settime.o: settime.c
	$(CC) -I. -c settime.c 

setwatch.o: setwatch.c
	$(CC) -I. -c setwatch.c 

settime: settime.o libdatalink.a
	$(CC) $(CFLAGS) -o $@ settime.o -L. -ldatalink

setwatch: setwatch.o libdatalink.a
	$(CC) $(CFLAGS) -o $@ setwatch.o -L. -ldatalink

cvt: cvt.o
	$(CC) $(CFLAGS) -o $@

install: all
	install -o root -g bin -m 555 libdatalink.so.1 $(LIBDIR)
	install -o root -g bin -m 555 libdatalink.a $(LIBDIR)
	install -o root -g bin -m 444 datalink.h $(INCDIR)
	install -o root -g bin -m 4555 svgablink $(BINDIR)
	install -o root -g bin -m 555 serblink $(BINDIR)
	install -o root -g bin -m 555 setwatch $(BINDIR)

clean:
	rm -f $(OBJ) libdatalink.a libdatalink.so.1 Makefile.bak
	rm -f $(SVGAOBJ) $(SEROBJ)
	rm -f parse.c dl_read_save.c y.tab.h
	rm -f settime.o setwatch.o cvt.o
	rm -f settime setwatch cvt $(ENGINES)

# DO NOT DELETE

dl_add_to_list.o: datalink.h
dl_anniv_by_date.o: datalink.h
dl_anniv_by_label.o: datalink.h
dl_app_by_datetime.o: datalink.h
dl_app_by_label.o: datalink.h
dl_free_download.o: datalink.h datalink_private.h
dl_init_download.o: datalink.h datalink_private.h
dl_init_watch.o: /usr/include/stdio.h /usr/include/features.h
dl_init_watch.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_init_watch.o: /usr/include/bits/types.h /usr/include/libio.h
dl_init_watch.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
dl_init_watch.o: datalink.h datalink_private.h
dl_item_ok.o: datalink.h datalink_private.h
dl_new_item.o: datalink.h
dl_new_list.o: datalink.h
dl_pack_size.o: datalink.h datalink_private.h
dl_phone_by_label.o: datalink.h
dl_send_data.o: /usr/include/fcntl.h /usr/include/features.h
dl_send_data.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_send_data.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
dl_send_data.o: /usr/include/bits/types.h /usr/include/time.h
dl_send_data.o: /usr/include/endian.h /usr/include/bits/endian.h
dl_send_data.o: /usr/include/sys/select.h /usr/include/bits/select.h
dl_send_data.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
dl_send_data.o: /usr/include/sys/wait.h /usr/include/bits/waitflags.h
dl_send_data.o: /usr/include/bits/waitstatus.h /usr/include/bits/siginfo.h
dl_send_data.o: datalink.h datalink_private.h
dl_set_error.o: datalink.h
dl_set_warn.o: datalink.h
dl_sizeof_item.o: datalink.h datalink_private.h
dl_sizeof_list.o: datalink.h datalink_private.h
dl_sort.o: datalink.h
dl_string_ok.o: datalink.h
dl_todo_by_label.o: datalink.h
dl_todo_by_prio.o: datalink.h
dl_write_save.o: /usr/include/stdio.h /usr/include/features.h
dl_write_save.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_write_save.o: /usr/include/bits/types.h /usr/include/libio.h
dl_write_save.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
dl_write_save.o: datalink.h datalink_private.h
svgablink.o: /usr/include/stdio.h /usr/include/features.h
svgablink.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
svgablink.o: /usr/include/bits/types.h /usr/include/libio.h
svgablink.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
svgablink.o: /usr/include/sys/fcntl.h /usr/include/fcntl.h
svgablink.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
svgablink.o: /usr/include/time.h /usr/include/endian.h
svgablink.o: /usr/include/bits/endian.h /usr/include/sys/select.h
svgablink.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
svgablink.o: /usr/include/sys/sysmacros.h /usr/include/signal.h
svgablink.o: /usr/include/bits/signum.h /usr/include/bits/siginfo.h
svgablink.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
svgablink.o: /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h
vt.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
vt.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h /usr/include/libio.h
vt.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
vt.o: /usr/include/sys/vt.h /usr/include/linux/vt.h /usr/include/sys/fcntl.h
vt.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h /usr/include/sys/types.h
vt.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
vt.o: /usr/include/sys/select.h /usr/include/bits/select.h
vt.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
send_data.o: /usr/include/stdio.h /usr/include/features.h
send_data.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
send_data.o: /usr/include/bits/types.h /usr/include/libio.h
send_data.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
send_data.o: /usr/include/vga.h /usr/include/sys/types.h /usr/include/time.h
send_data.o: /usr/include/endian.h /usr/include/bits/endian.h
send_data.o: /usr/include/sys/select.h /usr/include/bits/select.h
send_data.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
send_data.o: /usr/include/sys/time.h /usr/include/bits/time.h
send_data.o: datalink_macros.h
serblink.o: /usr/include/stdio.h /usr/include/features.h
serblink.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
serblink.o: /usr/include/bits/types.h /usr/include/libio.h
serblink.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
serblink.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
serblink.o: /usr/include/sys/types.h /usr/include/time.h
serblink.o: /usr/include/endian.h /usr/include/bits/endian.h
serblink.o: /usr/include/sys/select.h /usr/include/bits/select.h
serblink.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
serblink.o: /usr/include/stdlib.h /usr/include/alloca.h
serblink.o: /usr/include/termios.h /usr/include/bits/termios.h
serblink.o: /usr/include/sys/ttydefaults.h /usr/include/sys/time.h
serblink.o: /usr/include/bits/time.h /usr/include/sys/resource.h
serblink.o: /usr/include/bits/resource.h /usr/include/asm/resource.h
