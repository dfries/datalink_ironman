/* settime: Example of Linux SDK for Timex watch.              */
/* Written by Karl R. Hakimian 10/3/96                         */
/*                                                             */
/* Now warranty expressed or implied, use it as you see fit.   */

#include <stdio.h>
#include <time.h>
#include <datalink.h>

extern char *tzname[];

int
set_time(wi)
WatchInfoPtr wi;
{
	time_t now;
	struct tm *now_s;
	ListPtr times;
	ItemPtr tp;
	int isdst;
	int err;

	now = time(NULL) + 9; /* Offset for sending to watch. */
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
	tp->data.time.dow = (now_s->tm_wday + 6)%7;
	tp->data.time.hour_fmt = 1;
	tp->data.time.download = 1;
	tp->data.time.label = tzname[isdst];
	dl_add_to_list(times, tp);

	if (isdst)
		now -= 60*60;
	else
		now += 60*60;

	now_s = localtime(&now);

	tp = dl_new_item(wi, DL_TIME_TYPE);

	tp->data.time.hours = now_s->tm_hour;
	tp->data.time.minutes = now_s->tm_min;
	tp->data.time.seconds = now_s->tm_sec;
	tp->data.time.month = now_s->tm_mon + 1;
	tp->data.time.day = now_s->tm_mday;
	tp->data.time.year = now_s->tm_year;
	tp->data.time.dow = (now_s->tm_wday + 6)%7;
	tp->data.time.hour_fmt = 1;
	tp->data.time.download = 1;
	tp->data.time.label = tzname[1 - isdst];
	dl_add_to_list(times, tp);

	dl_init_download(wi, times, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	return(0);
}

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

main()
{
	WatchInfoPtr wi;
	int ret;
	int err;

/*
 Use my error proc to exit on error and my warn proc to ignore warnings
*/
	dl_set_error(my_error_proc);
	dl_set_warn(my_error_proc);

	wi = dl_init_watch(DATALINK_150);
/*
 Set the time.
*/
	if (set_time(wi))
		exit(-1);
/*
 Send it to the watch
*/
	dl_send_data(wi, SVGA_BLINK);
	exit(0);
}
