/* setwatch: Example of Linux SDK for Timex watch.             */
/* Written by Karl R. Hakimian 10/3/96                         */
/*                                                             */
/* Now warranty expressed or implied, use it as you see fit.   */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <datalink.h>

my_error_proc(msg)
char *msg;
{
	fprintf(stderr, "ERROR: %s\n", msg);
	exit(-1);
}

my_warn_proc(msg)
char *msg;
{
	fprintf(stderr, "WARNING: %s\n", msg);
	return(0);
}

ListPtr
set_time(wi, times)
WatchInfoPtr wi;
ListPtr times;
{
	time_t now;
	time_t tztime;
	struct tm *time_s;
	struct timezone tz;
	ItemPtr tp;
	int isdst;
	char buf[1024];
	int i;

	now = time(NULL) + wi->time_adjust; /* Offset for sending to watch. */
	time_s = localtime(&now);
	isdst = (time_s->tm_isdst) ? 1 : 0;

	if (!times->count) {
		tp = dl_new_item(wi, DL_TIME_TYPE);
		tp->data.time.label = tzname[isdst];
		tp->data.time.offset = timezone/60;
		tp->data.time.offset = (isdst) ? (timezone/60) - 60 : (timezone/60);
		tp->data.time.hour_fmt = 1;
		dl_add_to_list(times, tp);
		tp = dl_new_item(wi, DL_TIME_TYPE);
		tp->data.time.label = tzname[1 - isdst];
		tp->data.time.offset = (isdst) ? (timezone/60) : (timezone/60) - 60;
		tp->data.time.hour_fmt = 1;
		dl_add_to_list(times, tp);
	}

	for (i = 0, tp = times->first; i < times->count; i++, tp = tp->next) {

		if (!*tp->data.time.label) {
			tp->data.time.label = tzname[isdst];
			tp->data.time.offset = timezone/60;
		}

		tztime = now - tp->data.time.offset*60;
		time_s = gmtime(&tztime);
		tp->data.time.hours = time_s->tm_hour;
		tp->data.time.minutes = time_s->tm_min;
		tp->data.time.seconds = time_s->tm_sec;
		tp->data.time.month = time_s->tm_mon + 1;
		tp->data.time.day = time_s->tm_mday;
		tp->data.time.year = time_s->tm_year;
		tp->data.time.dow = (time_s->tm_wday + 6)%7;
		tp->data.time.download = 1;
	}

	return(times);
}

#define TIME 0x001
#define ALARM 0x002
#define APP 0x004
#define TODO 0x008
#define PHONE 0x010
#define ANNIV 0x020
#define SYSTEM 0x040
#define WRISTAPP 0x080
#define MELODY 0x100

#define ALL 0x1FF
#define DB 0x03C
/*
#define DEFAULT 0x03F
*/
#define DEFAULT 0

main(argc, argv)
int argc;
char **argv;
{
	char *prog = argv[0];
	WatchInfoPtr wi;
	ListPtr times, alarms, apps, todos, phones, annivs;
	ListPtr system, wristapp, melody;
	char datafile[1024];
	int output = SVGA_BLINK;
	int flags = DEFAULT;
	int (*app_sort)() = dl_app_by_datetime;
	int (*todo_sort)() = dl_todo_by_prio;
	int (*phone_sort)() = dl_phone_by_label;
	int (*anniv_sort)() = dl_anniv_by_date;
	int type = DATALINK_IRONMAN;

	while (argc > 1 && (*argv[1] == '-' || *argv[1] == '+')) {

		if (strcmp("-all", argv[1]) == 0)
			flags = ALL;
		else if (strcmp("-db", argv[1]) == 0)
			flags = DB;
		else if (strcmp("+db", argv[1]) == 0)
			flags |= DB;
		else if (strcmp("-time", argv[1]) == 0)
			flags = TIME;
		else if (strcmp("+time", argv[1]) == 0)
			flags |= TIME;
		else if (strcmp("-alarm", argv[1]) == 0)
			flags = ALARM;
		else if (strcmp("+alarm", argv[1]) == 0)
			flags |= ALARM;
		else if (strcmp("-wristapp", argv[1]) == 0)
			flags = WRISTAPP;
		else if (strcmp("+wristapp", argv[1]) == 0)
			flags |= WRISTAPP;
		else if (strcmp("-app", argv[1]) == 0)
			flags = WRISTAPP;
		else if (strcmp("+app", argv[1]) == 0)
			flags |= WRISTAPP;
		else if (strcmp("-melody", argv[1]) == 0)
			flags = MELODY;
		else if (strcmp("+melody", argv[1]) == 0)
			flags |= MELODY;
		else if (strcmp("-system", argv[1]) == 0)
			flags = SYSTEM;
		else if (strcmp("+system", argv[1]) == 0)
			flags |= SYSTEM;
		else if (strcmp("-reset", argv[1]) == 0) {
			flags = 0;
			app_sort = todo_sort = phone_sort = anniv_sort = NULL;
		}
		else if (strcmp("-sort-app-by-datetime", argv[1]) == 0)
			app_sort = dl_app_by_datetime;
		else if (strcmp("-sort-app-by-label", argv[1]) == 0)
			app_sort = dl_app_by_label;
		else if (strcmp("-sort-todo-by-label", argv[1]) == 0)
			todo_sort = dl_todo_by_label;
		else if (strcmp("-sort-todo-by-prio", argv[1]) == 0)
			todo_sort = dl_todo_by_prio;
		else if (strcmp("-sort-phone", argv[1]) == 0)
			phone_sort = dl_phone_by_label;
		else if (strcmp("-sort-anniv-by-date", argv[1]) == 0)
			anniv_sort = dl_anniv_by_date;
		else if (strcmp("-sort-anniv-by-label", argv[1]) == 0)
			anniv_sort = dl_anniv_by_label;
		else if (strcmp("-file", argv[1]) == 0)
			output = BLINK_FILE;
		else if (strcmp("-serial", argv[1]) == 0)
			output = SER_BLINK;
		else if (strcmp("-model70", argv[1]) == 0)
			type = DATALINK_70;
		else if (strcmp("-70", argv[1]) == 0)
			type = DATALINK_70;
		else if (strcmp("-ironman", argv[1]) == 0)
			type = DATALINK_IRONMAN;
		else {
			fprintf(stderr, "%s: Unknown option (%s).\n", prog, argv[1]);
			exit(-1);
		}

		argc--;
		argv++;
	}

	if (argc == 2)
		strcpy(datafile, argv[1]);
	else if (argc > 2) {
		fprintf(stderr, "Usage: %s [datafile]\n", argv[0]);
		exit(-1);
	}
	else {
		strcpy(datafile, getenv("HOME"));
		strcat(datafile, "/");
		strcat(datafile, ".datalink/datafile");
	}
/*
 Use my error proc to exit on error and my warn proc to ignore warnings
*/
	dl_set_error(my_error_proc);
	dl_set_warn(my_warn_proc);

	wi = dl_read_save(datafile, type, &times, &alarms, &apps, &todos, &phones,
		&annivs, &system, &wristapp, &melody);

/* Mark for download. */

	if ((flags&ALARM) && alarms->count)
		alarms->download = 1;

	if ((flags&APP) && apps->count) {
		apps->download = 1;

		if (app_sort)
			dl_sort(apps, app_sort);

	}

	if ((flags&TODO) && todos->count) {
		todos->download = 1;

		if (todo_sort)
			dl_sort(todos, todo_sort);

	}

	if ((flags&PHONE) && phones->count) {
		phones->download = 1;

		if (phone_sort)
			dl_sort(phones, phone_sort);

	}

	if ((flags&ANNIV) && annivs->count) {
		annivs->download = 1;

		if (anniv_sort)
			dl_sort(annivs, anniv_sort);

	}

	if ((flags&SYSTEM) && system->count)
		system->download = 1;

	if ((flags&WRISTAPP) && wristapp->count)
		wristapp->download = 1;

	if ((flags&MELODY) && melody->count)
		melody->download = 1;

	if ((flags&TIME)) {
		times = set_time(wi, times);
		times->download = 1;
	}
	else
		times = NULL;
/*
 Send it to the watch
*/
	dl_init_download(wi, times, alarms, apps, todos, phones, annivs, system,
		wristapp, melody);
	dl_send_data(wi, output);
	exit(0);
}
