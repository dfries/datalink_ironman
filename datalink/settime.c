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

/* settime: Example of Linux SDK for Timex watch.              */
/* Written by Karl R. Hakimian 10/3/96                         */
/*                                                             */
/* Now warranty expressed or implied, use it as you see fit.   */
 /*
    *
    * Modified by David Fries <dfries@mail.win.org> 7/11/99
    * - Added support for the Timex Datalink Ironman Triathlon
  */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "datalink.h"

extern char *tzname[];

int set_time(WatchInfoPtr wi)
{
	time_t now;
	struct tm *now_s;
	ListPtr times;
	ItemPtr tp;
	int isdst;

	now = time(NULL) + 9;	/* Offset for sending to watch. */
	now_s = localtime(&now);
	isdst = (now_s->tm_isdst > 0) ? 1 : 0;

	times = dl_new_list();
	times->download = 1;
	tp = dl_new_item(wi, DL_TIME_TYPE);

	tp->data.time.hours = now_s->tm_hour;
	tp->data.time.minutes = now_s->tm_min;
	tp->data.time.seconds = now_s->tm_sec;
	tp->data.time.month = now_s->tm_mon + 1;
	tp->data.time.day = now_s->tm_mday;
	tp->data.time.year = now_s->tm_year;
/* TODO: investigate, see change log */
	tp->data.time.dow = (now_s->tm_wday + 6) % 7;
	tp->data.time.hour_fmt = 1;
	tp->data.time.download = 1;
	tp->data.time.label = tzname[isdst];
	dl_add_to_list(times, tp);

	if (isdst)
		now -= 60 * 60;
	else
		now += 60 * 60;

	now_s = localtime(&now);

	tp = dl_new_item(wi, DL_TIME_TYPE);

	tp->data.time.hours = now_s->tm_hour;
	tp->data.time.minutes = now_s->tm_min;
	tp->data.time.seconds = now_s->tm_sec;
	tp->data.time.month = now_s->tm_mon + 1;
	tp->data.time.day = now_s->tm_mday;
	tp->data.time.year = now_s->tm_year;
/* TODO: investigate, see change log */
	tp->data.time.dow = (now_s->tm_wday + 6) % 7;
	tp->data.time.hour_fmt = 1;
	tp->data.time.download = 1;
	tp->data.time.label = tzname[1 - isdst];
	dl_add_to_list(times, tp);

	dl_init_download(wi, times, NULL, NULL, NULL, NULL, NULL, NULL,
			 NULL, NULL, NULL, NULL);
	return (0);
}

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

int main( int argc, char ** argv)
{
	WatchInfoPtr wi;

/*
 Use my error proc to exit on error and my warn proc to ignore warnings
*/
	dl_set_error(my_error_proc);
	dl_set_warn(my_error_proc);

	if(argc==2)
	{
		if(!strcmp("-model70",argv[1])||!strcmp("-70",argv[1]))
			wi = dl_init_watch(DATALINK_70);
		else
		if(!strcmp("-model150",argv[1])||!strcmp("-150",argv[1]))
			wi = dl_init_watch(DATALINK_150);
		else
		if(!strcmp("-model150s",argv[1])||!strcmp("-150s",argv[1]))
			wi = dl_init_watch(DATALINK_150S);
		else
		if(!strcmp("-ironman",argv[1]))
			wi = dl_init_watch(DATALINK_IRONMAN);
		else
		{
			printf("Usage: %s [ -model70 | -70 | -model70 | "
				"-150 | -model150 | -model150s | -150s |"
				" -ironman]\n", argv[0]);
			printf("The default is -model150 "
				"is none is specified\n");
			exit(1);
		}
	}
	else
	{
		wi = dl_init_watch(DATALINK_IRONMAN);
	}
/*
 Set the time.
*/
	if (set_time(wi))
		exit(-1);
/*
 Send it to the watch
*/
	dl_send_data(wi, SVGA_BLINK);
	return 0;
}
