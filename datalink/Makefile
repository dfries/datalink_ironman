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
CFLAGS = -O2 -DBINDIR=\"$(BINDIR)\"

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


all: libdatalink.so.1 settime setwatch $(ENGINES)

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

depend: parse.c dl_read_save.c
	makedepend -o.o $(CFLAGS) $(SRC) $(SVGASRC) $(SERSRC)

serblink.o: $(SERSRC)
	$(CC) -c $(CFLAGS) -DPORT=\"$(PORT)\" $(SERSRC)

serblink: $(SEROBJ)
	rm -f $@
	$(CC) $(CFLAGS) -o $@ $(SEROBJ)

svgablink: $(SVGAOBJ)
	rm -f $@
	$(CC) $(CFLAGS) -o $@ $(SVGAOBJ) -lvga

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
	install -o root -g video -m 4750 svgablink $(BINDIR)
	install -o root -g root -m 755 serblink $(BINDIR)
	install -o root -g root -m 755 setwatch $(BINDIR)
	install -o root -g root -m 755 settime $(BINDIR)
	install -o root -g root -m 644 setwatch.1 $(MANDIR)/man1
	install -o root -g root -m 644 settime.1 $(MANDIR)/man1
	install -o root -g root -m 644 svgablink.1 $(MANDIR)/man1
	install -o root -g root -m 644 serblink.1 $(MANDIR)/man1

install-devel: install
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

# DO NOT DELETE

dl_add_to_list.o: datalink.h
dl_anniv_by_date.o: datalink.h
dl_anniv_by_label.o: /usr/include/string.h /usr/include/features.h
dl_anniv_by_label.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_anniv_by_label.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_anniv_by_label.o: datalink.h
dl_app_by_datetime.o: datalink.h
dl_app_by_label.o: /usr/include/string.h /usr/include/features.h
dl_app_by_label.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_app_by_label.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_app_by_label.o: datalink.h
dl_fill_pack_ascii.o: /usr/include/string.h /usr/include/features.h
dl_fill_pack_ascii.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_fill_pack_ascii.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_fill_pack_ascii.o: datalink.h
dl_free_download.o: /usr/include/stdlib.h /usr/include/features.h
dl_free_download.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_free_download.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_free_download.o: /usr/include/string.h datalink.h datalink_private.h
dl_init_download.o: /usr/include/stdio.h /usr/include/features.h
dl_init_download.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_init_download.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_init_download.o: /usr/include/bits/types.h /usr/include/libio.h
dl_init_download.o: /usr/include/_G_config.h /usr/include/wchar.h
dl_init_download.o: /usr/include/bits/wchar.h /usr/include/gconv.h
dl_init_download.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
dl_init_download.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
dl_init_download.o: /usr/include/string.h datalink.h datalink_private.h
dl_init_watch.o: /usr/include/stdio.h /usr/include/features.h
dl_init_watch.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_init_watch.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_init_watch.o: /usr/include/bits/types.h /usr/include/libio.h
dl_init_watch.o: /usr/include/_G_config.h /usr/include/wchar.h
dl_init_watch.o: /usr/include/bits/wchar.h /usr/include/gconv.h
dl_init_watch.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
dl_init_watch.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
dl_init_watch.o: /usr/include/string.h datalink.h datalink_private.h
dl_item_ok.o: /usr/include/stdio.h /usr/include/features.h
dl_item_ok.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_item_ok.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_item_ok.o: /usr/include/bits/types.h /usr/include/libio.h
dl_item_ok.o: /usr/include/_G_config.h /usr/include/wchar.h
dl_item_ok.o: /usr/include/bits/wchar.h /usr/include/gconv.h
dl_item_ok.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
dl_item_ok.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h datalink.h
dl_item_ok.o: datalink_private.h
dl_new_item.o: /usr/include/stdlib.h /usr/include/features.h
dl_new_item.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_new_item.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_new_item.o: /usr/include/string.h datalink.h
dl_new_list.o: /usr/include/stdlib.h /usr/include/features.h
dl_new_list.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_new_list.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_new_list.o: /usr/include/string.h datalink.h
dl_pack_ascii.o: datalink.h
dl_pack_char.o: /usr/include/ctype.h /usr/include/features.h
dl_pack_char.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_pack_char.o: /usr/include/bits/types.h
dl_pack_char.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_pack_char.o: /usr/include/endian.h /usr/include/bits/endian.h
dl_pack_digit.o: /usr/include/ctype.h /usr/include/features.h
dl_pack_digit.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_pack_digit.o: /usr/include/bits/types.h
dl_pack_digit.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_pack_digit.o: /usr/include/endian.h /usr/include/bits/endian.h
dl_pack_phone.o: /usr/include/string.h /usr/include/features.h
dl_pack_phone.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_pack_phone.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_pack_phone.o: datalink.h
dl_pack_size.o: /usr/include/string.h /usr/include/features.h
dl_pack_size.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_pack_size.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_pack_size.o: datalink.h datalink_private.h
dl_phone_by_label.o: /usr/include/string.h /usr/include/features.h
dl_phone_by_label.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_phone_by_label.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_phone_by_label.o: datalink.h
dl_send_data.o: /usr/include/stdlib.h /usr/include/features.h
dl_send_data.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_send_data.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_send_data.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
dl_send_data.o: /usr/include/bits/types.h /usr/include/bits/confname.h
dl_send_data.o: /usr/include/stdio.h /usr/include/libio.h
dl_send_data.o: /usr/include/_G_config.h /usr/include/wchar.h
dl_send_data.o: /usr/include/bits/wchar.h /usr/include/gconv.h
dl_send_data.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
dl_send_data.o: /usr/include/bits/stdio_lim.h /usr/include/fcntl.h
dl_send_data.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
dl_send_data.o: /usr/include/time.h /usr/include/sys/wait.h
dl_send_data.o: /usr/include/signal.h /usr/include/bits/sigset.h
dl_send_data.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
dl_send_data.o: /usr/include/bits/resource.h /usr/include/bits/time.h
dl_send_data.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
dl_send_data.o: /usr/include/string.h datalink.h datalink_private.h
dl_set_error.o: datalink.h
dl_set_warn.o: datalink.h
dl_sizeof_item.o: /usr/include/string.h /usr/include/features.h
dl_sizeof_item.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_sizeof_item.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_sizeof_item.o: datalink.h datalink_private.h
dl_sizeof_list.o: datalink.h datalink_private.h
dl_sort.o: /usr/include/stdlib.h /usr/include/features.h
dl_sort.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_sort.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h datalink.h
dl_string_ok.o: /usr/include/string.h /usr/include/features.h
dl_string_ok.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_string_ok.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_string_ok.o: datalink.h
dl_todo_by_label.o: /usr/include/string.h /usr/include/features.h
dl_todo_by_label.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_todo_by_label.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_todo_by_label.o: datalink.h
dl_todo_by_prio.o: datalink.h
dl_write_save.o: /usr/include/stdio.h /usr/include/features.h
dl_write_save.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
dl_write_save.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
dl_write_save.o: /usr/include/bits/types.h /usr/include/libio.h
dl_write_save.o: /usr/include/_G_config.h /usr/include/wchar.h
dl_write_save.o: /usr/include/bits/wchar.h /usr/include/gconv.h
dl_write_save.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
dl_write_save.o: /usr/include/bits/stdio_lim.h /usr/include/unistd.h
dl_write_save.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
dl_write_save.o: datalink.h datalink_private.h
svgablink.o: /usr/include/unistd.h /usr/include/features.h
svgablink.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
svgablink.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
svgablink.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
svgablink.o: /usr/include/bits/confname.h /usr/include/stdio.h
svgablink.o: /usr/include/libio.h /usr/include/_G_config.h
svgablink.o: /usr/include/wchar.h /usr/include/bits/wchar.h
svgablink.o: /usr/include/gconv.h
svgablink.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
svgablink.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
svgablink.o: /usr/include/sys/fcntl.h /usr/include/fcntl.h
svgablink.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
svgablink.o: /usr/include/time.h /usr/include/signal.h
svgablink.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
svgablink.o: /usr/include/sched.h /usr/include/bits/sched.h
svgablink.o: /usr/include/string.h /usr/include/sys/ioctl.h
svgablink.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
svgablink.o: /usr/include/asm/ioctl.h /usr/include/bits/ioctl-types.h
svgablink.o: /usr/include/sys/ttydefaults.h /usr/include/sys/mman.h
svgablink.o: /usr/include/bits/mman.h datalink.h
vt.o: /usr/include/unistd.h /usr/include/features.h /usr/include/sys/cdefs.h
vt.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
vt.o: /usr/include/bits/types.h
vt.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
vt.o: /usr/include/bits/confname.h /usr/include/stdio.h /usr/include/libio.h
vt.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h
vt.o: /usr/include/gconv.h
vt.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
vt.o: /usr/include/bits/stdio_lim.h /usr/include/sys/vt.h
vt.o: /usr/include/linux/vt.h /usr/include/sys/ioctl.h
vt.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
vt.o: /usr/include/asm/ioctl.h /usr/include/bits/ioctl-types.h
vt.o: /usr/include/sys/ttydefaults.h /usr/include/sys/fcntl.h
vt.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h /usr/include/sys/types.h
vt.o: /usr/include/time.h
send_data.o: /usr/include/unistd.h /usr/include/features.h
send_data.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
send_data.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
send_data.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
send_data.o: /usr/include/bits/confname.h /usr/include/stdio.h
send_data.o: /usr/include/libio.h /usr/include/_G_config.h
send_data.o: /usr/include/wchar.h /usr/include/bits/wchar.h
send_data.o: /usr/include/gconv.h
send_data.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
send_data.o: /usr/include/bits/stdio_lim.h /usr/include/vga.h
send_data.o: /usr/include/sys/types.h /usr/include/time.h
send_data.o: /usr/include/sys/time.h /usr/include/bits/time.h
send_data.o: /usr/include/sys/select.h /usr/include/bits/select.h
send_data.o: /usr/include/bits/sigset.h datalink.h datalink_macros.h
serblink.o: /usr/include/unistd.h /usr/include/features.h
serblink.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
serblink.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
serblink.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h
serblink.o: /usr/include/bits/confname.h /usr/include/stdio.h
serblink.o: /usr/include/libio.h /usr/include/_G_config.h
serblink.o: /usr/include/wchar.h /usr/include/bits/wchar.h
serblink.o: /usr/include/gconv.h
serblink.o: /usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h
serblink.o: /usr/include/bits/stdio_lim.h /usr/include/fcntl.h
serblink.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
serblink.o: /usr/include/time.h /usr/include/stdlib.h /usr/include/termios.h
serblink.o: /usr/include/bits/termios.h /usr/include/string.h
serblink.o: /usr/include/sys/time.h /usr/include/bits/time.h
serblink.o: /usr/include/sys/select.h /usr/include/bits/select.h
serblink.o: /usr/include/bits/sigset.h /usr/include/sys/resource.h
serblink.o: /usr/include/bits/resource.h
