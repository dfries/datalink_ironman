/* 
 * Copyright 1996-2002 - Karl R. Hakimian and David Fries
 *
 * This file is part of datalink.
 *
 * Datalink is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Datalink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with datalink (see COPYING); if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
 * setwatch: Example of Linux SDK for Timex watch.
 * Written by Karl R. Hakimian 10/3/96
 *
 * Modified by David Fries <dfries@mail.win.org> 7/11/99
 * - Added support for the Timex Datalink Ironman Triathlon
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <datalink.h>

int my_error_proc(char *msg)
{
	fprintf(stderr, "ERROR: %s\n", msg);
	exit(-1);
}

int my_warn_proc(char *msg)
{
	fprintf(stderr, "WARNING: %s\n", msg);
	return (0);
}

ListPtr set_time(WatchInfoPtr wi, ListPtr times)
{
	time_t now;
	time_t tztime;
	struct tm *time_s;
	ItemPtr tp;
	int isdst;
	int i;

	now = time(NULL) + wi->time_adjust;	/* Offset for sending to watch. */
	time_s = localtime(&now);
	isdst = (time_s->tm_isdst) ? 1 : 0;

	if (!times->count)
	{
		tp = dl_new_item(wi, DL_TIME_TYPE);
		tp->data.time.label = tzname[isdst];
		tp->data.time.offset = timezone / 60;
		tp->data.time.offset =
		    (isdst) ? (timezone / 60) - 60 : (timezone / 60);
		tp->data.time.hour_fmt = 1;
		dl_add_to_list(times, tp);
		tp = dl_new_item(wi, DL_TIME_TYPE);
		tp->data.time.label = tzname[1 - isdst];
		tp->data.time.offset =
		    (isdst) ? (timezone / 60) : (timezone / 60) - 60;
		tp->data.time.hour_fmt = 1;
		dl_add_to_list(times, tp);
	}

	for (i = 0, tp = times->first; i < times->count;
	     i++, tp = tp->next)
	{

		if (!*tp->data.time.label)
		{
			tp->data.time.label = tzname[isdst];
			tp->data.time.offset = timezone / 60;
		}

		tztime = now - tp->data.time.offset * 60;
		time_s = localtime(&tztime);
		tp->data.time.hours = time_s->tm_hour;
		tp->data.time.minutes = time_s->tm_min;
		tp->data.time.seconds = time_s->tm_sec;
		tp->data.time.month = time_s->tm_mon + 1;
		tp->data.time.day = time_s->tm_mday;
		tp->data.time.year = time_s->tm_year % 100;
	/* TODO: investigate, see change log about day of week */
		tp->data.time.dow = (time_s->tm_wday + 6) % 7;
		tp->data.time.download = 1;
#ifdef DEBUGGING
		printf("now %d, offset %d, dz %d, timezone %d\n", now,
		       tp->data.time.offset, time_s->tm_isdst, timezone);
		printf("%d:%d:%d %d/%d/%d DOW %d\n", time_s->tm_hour,
		       time_s->tm_min, time_s->tm_sec, time_s->tm_mon + 1,
		       time_s->tm_mday, time_s->tm_year,
		       (time_s->tm_wday + 6) % 7);
#endif

	}

	return (times);
}

void Usage()
{
	printf("DataLink Library Karl Hakimian <hakimian@eecs.wsu.edu>\n");
	printf
	    ("\tIronman support added by David Fries <dfries@mail.win.org>\n");
	printf("Usage: datalink [watch type] [options] [datafile]\n");
	printf
	    ("watchtype (ironman is default, must be specified before other options)\n");
	printf("  -model70\t use if you have this watch\n");
	printf("  -70\t use if you have this watch\n");
	printf("  -model70\t use if you have this watch\n");
	printf("  -150\t use if you have this watch\n");
	printf("  -model150\t use if you have this watch\n");
	printf("  -150s\t use if you have this watch\n");
	printf("  -model150s\t use if you have this watch\n");
	printf("  -ironman\t use if you have this watch\n");
	printf("options:\n");
	printf("\tNot all options are available for all watches\n");
	printf
	    ("\tIf you specify any of the send only... options following\n");
	printf("\twill affect what other data is also sent\n");
	printf("\tYou must specify at least one option to send data.\n");
	printf("  -all\tsend all data to watch\n");
	printf("  -db\tsend only database information\n");
	printf("  -db\talso send database information\n");
	printf("  -time\tonly send time information\n");
	printf("  +time\talso send time information\n");
	printf("  -alarm\tonly send alarm information\n");
	printf("  +alarm\talso send alarm information\n");
	printf("  -timer\tonly send timer information\n");
	printf("  +timer\talso send timer information\n");
	printf("  -wristapp\tonly send wristapp information\n");
	printf("  +wristapp\talso send wristapp information\n");
	printf("  -app\tonly send appointment information\n");
	printf("  +app\talso send appointment information\n");
	printf("  -melody\tonly send melody information\n");
	printf("  +melody\talso send melody information\n");
	printf("  -phone\tonly send phone information\n");
	printf("  +phone\talso send phone information\n");
	printf("  -chron\tonly send chronograph information\n");
	printf("  +chron\talso send chronograph information\n");
	printf
	    ("  -system\tonly send system (hour/button beep) information\n");
	printf
	    ("  +system\talso send system (hour/button beep) information\n");
	printf("  -reset\n");
	printf("  +reset\n");
	printf("  -sort-app-by-datetime\n");
	printf("  -sort-app-by-label\n");
	printf("  -sort-todo-by-label\n");
	printf("  -sort-todo-by-prio\n");
	printf("  -sort-phone\n");
	printf("  -sort-anniv-by-date\n");
	printf("  -sort-anniv-by-label\n");
	printf("  -file\t dump data to DEBUGOUTPUT and do not display\n");
	printf("  -serial\t send with the serial link\n");
	printf
	    ("datafile: If not specified ~/.datalink/datafile is used\n");
	printf
	    ("  If any option is after the datafile, it will be ignored\n");
}

// ability to set the time
#define TIME		0x001
// ability to send alarms
#define ALARM		0x002
// ability to set appointments
#define APP		0x004
// ability to send todo 
#define TODO		0x008
// ability to send phone list
#define PHONE		0x010
// ability to set anniversary
#define ANNIV		0x020
// ability to set key beep and hour beep
#define SYSTEM		0x040
// ability to send a program
#define WRISTAPP	0x080
// ability to set the beep sound
#define MELODY		0x100
// ability to preset timers and label
#define TIMER		0x200
// ability to set the lap count and label
#define CHRON		0x400

/* All would be 0x755 */
#define ALL70		0x07F
#define ALL150		0x1FF
#define ALLIRONMAN	0x653
#define DB		0x03C
/*
#define DEFAULT 0x03F
*/
#define DEFAULT 0

int main(int argc, char **argv)
{
	char *prog = argv[0];
	WatchInfoPtr wi;
	ListPtr times, alarms, chron, timers, apps, todos, phones, annivs;
	ListPtr system, wristapp, melody;
	char datafile[1024];
	int output = SVGA_BLINK;
	int flags = DEFAULT;
	int (*app_sort) () = dl_app_by_datetime;
	int (*todo_sort) () = dl_todo_by_prio;
	int (*phone_sort) () = dl_phone_by_label;
	int (*anniv_sort) () = dl_anniv_by_date;
	int type = DATALINK_IRONMAN;

	while (argc > 1 && (*argv[1] == '-' || *argv[1] == '+'))
	{

		if (strcmp("-all", argv[1]) == 0)
		{
			if (type == DATALINK_70)
				flags = ALL70;
			if (type == DATALINK_150 || type == DATALINK_150S)
				flags = ALL150;
			if (type == DATALINK_IRONMAN)
				flags = ALLIRONMAN;
		} else if (strcmp("-db", argv[1]) == 0)
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
		else if (strcmp("-timer", argv[1]) == 0)
			flags = TIMER;
		else if (strcmp("+timer", argv[1]) == 0)
			flags |= TIMER;
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
		/* on IRONMAN to send phone information we must
		 * also send chron information
		 */
		else if (strcmp("-phone", argv[1]) == 0)
			flags = PHONE | CHRON;
		else if (strcmp("+phone", argv[1]) == 0)
			flags |= PHONE | CHRON;
		else if (strcmp("-chron", argv[1]) == 0)
			flags = CHRON;
		else if (strcmp("+chron", argv[1]) == 0)
			flags |= CHRON;
		else if (strcmp("-system", argv[1]) == 0)
			flags = SYSTEM;
		else if (strcmp("+system", argv[1]) == 0)
			flags |= SYSTEM;
		else if (strcmp("-reset", argv[1]) == 0)
		{
			flags = 0;
			app_sort = todo_sort = phone_sort = anniv_sort =
			    NULL;
		} else if (strcmp("-sort-app-by-datetime", argv[1]) == 0)
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
		else if (strcmp("-model150", argv[1]) == 0)
			type = DATALINK_150;
		else if (strcmp("-150", argv[1]) == 0)
			type = DATALINK_150;
		else if (strcmp("-model150s", argv[1]) == 0)
			type = DATALINK_150S;
		else if (strcmp("-150s", argv[1]) == 0)
			type = DATALINK_150S;
		else if (strcmp("-ironman", argv[1]) == 0)
			type = DATALINK_IRONMAN;
		else if (strcmp("--help", argv[1]) == 0)
		{
			Usage();
			exit(-1);
		} else if (strcmp("-h", argv[1]) == 0)
		{
			Usage();
			exit(-1);
		} else
		{
			fprintf(stderr, "%s: Unknown option (%s).\n", prog,
				argv[1]);
			Usage();
			exit(-1);
		}

		argc--;
		argv++;
	}

	if (flags&ALARM && !(flags&PHONE))
	{
		printf("Warning: Sending alarm information has been known to "
			"corrupt the phone list on the watch, please check "
			"and add +phone if you find this is the case.\n");
	}

	if (flags == DEFAULT)
	{
		Usage();
		printf("**************** ERROR ****************\n");
		printf
		    ("Error: the given options specify that no data be sent to the watch\n");
		exit(-1);
	}

	if (argc == 2)
		strcpy(datafile, argv[1]);
	else if (argc > 2)
	{
		fprintf(stderr, "Usage: %s [datafile]\n", argv[0]);
		exit(-1);
	} else
	{
		strcpy(datafile, getenv("HOME"));
		strcat(datafile, "/");
		strcat(datafile, ".datalink/datafile");
	}
/*
 Use my error proc to exit on error and my warn proc to ignore warnings
*/
	dl_set_error(my_error_proc);
	dl_set_warn(my_warn_proc);

	wi = dl_read_save(datafile, type, &times, &alarms, &chron, &timers,
			  &apps, &todos, &phones, &annivs, &system,
			  &wristapp, &melody);

/* Mark for download. */

	if ((flags & CHRON) && chron->count)
		chron->download = 1;

	if ((flags & ALARM) && alarms->count)
		alarms->download = 1;

	if ((flags & TIMER) && timers->count)
		timers->download = 1;

	if ((flags & APP) && apps->count)
	{
		apps->download = 1;

		if (app_sort)
			dl_sort(apps, app_sort);

	}

	if ((flags & TODO) && todos->count)
	{
		todos->download = 1;

		if (todo_sort)
			dl_sort(todos, todo_sort);

	}

	if ((flags & PHONE) && phones->count)
	{
		phones->download = 1;

		if (phone_sort)
			dl_sort(phones, phone_sort);

	}

	if ((flags & ANNIV) && annivs->count)
	{
		annivs->download = 1;

		if (anniv_sort)
			dl_sort(annivs, anniv_sort);

	}

	if ((flags & SYSTEM) && system->count)
		system->download = 1;

	if ((flags & WRISTAPP) && wristapp->count)
		wristapp->download = 1;

	if ((flags & MELODY) && melody->count)
		melody->download = 1;

	if ((flags & TIME))
	{
		times = set_time(wi, times);
		times->download = 1;
	} else
		times = NULL;
/*
 Send it to the watch
*/
	dl_init_download(wi, times, alarms, chron, timers, apps, todos,
			 phones, annivs, system, wristapp, melody);
	dl_send_data(wi, output, argv[0]);
	return 0;
}
