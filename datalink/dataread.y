%{
 /*
 *
 * Modified by David Fries <dfries@mail.win.org> 7/11/99
 * - Added support for the Timex Datalink Ironman Triathlon
 */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "datalink.h"

extern int line_num;

#define ALARM 0
#define APP 1
#define PHONE 2
#define TODO 3
#define ANNIV 4
#define SYSTEM 5
#define WRISTAPP 6
#define MELODY 7
#define TIMEZONE 8
#define DATA 9
#define INTVAL 10
#define TIMER 11
#define CHRON 12
#define NUMLISTS 13

int dl_error(char *s);

static ListPtr *lists = NULL;
static ItemPtr ip = NULL;
static char buf[1024];
static int hour, minute, second, month, day, year;
static int repeat, chron;
static int prio, audible, chime, beep, pos;
static int val, tfmt, dfmt;
static char msg[64];
static char phnum[64];
static WatchInfoPtr wi;
static int watch_type;

%}
%token NAME INTEGER STRING TIME DATE BAD

%union {
	char string[1024];
	int integer;
}

%left <string> NAME
%left <string> STRING
%left <integer> INTEGER
%left <string> TIME
%left <string> DATE

%type <integer> value

%%

data		: item
			| data item
			;

item		: NAME '=' value '\n'
{
	int fd;
	char file[1024];
	unsigned char mask;
	int min;
	struct stat sbuf;
	int l;
	int i;

	if (!lists) {
		wi = dl_init_watch(watch_type);

		if ((lists = (ListPtr *)calloc(NUMLISTS, sizeof(ListPtr))) == NULL) {
			dl_error("Could not allocate List list.");
			return(-1);
		}

		for (i = 0; i < NUMLISTS; i++) {

			if (!(lists[i] = dl_new_list())) {
				dl_error("Could not allocate item list.");
				return(-1);
			}

		}

	}

	switch ($3) {
	case ALARM:

		if (strcmp($1, "alarm")) {
			sprintf(buf, "Bad entry, %s = alarm.", $1);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_ALARM_TYPE))) {
			dl_error("Could not allocate alarm item.");
			return(-1);
		}

		ip->data.alarm.alarm_num = pos;
		ip->data.alarm.hours = hour;
		ip->data.alarm.minutes = minute;
		ip->data.alarm.month = month;
		ip->data.alarm.day = day;
		ip->data.alarm.audible = audible;
		l = strlen(msg);

		if (l > wi->max_alarm_str)
			l = wi->max_alarm_str;

		if ((ip->data.alarm.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate alarm label.");
			return(-1);
		}

		strncpy(ip->data.alarm.label, msg, l);
		ip->data.alarm.label[l] = '\0';
		dl_add_to_list(lists[ALARM], ip);
		break;

	case CHRON:
		if (strcmp($1, "chron")) {
			sprintf(buf, "Bad entry, %s = chron.", $1);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_CHRON_TYPE))) {
			dl_error("Could not allocate chron item.");
			return(-1);
		}

		ip->data.chron.chron_laps = pos;
		l = strlen(msg);

		if (l > wi->max_chron_str)
			l = wi->max_chron_str;

		if ((ip->data.chron.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate chron label.");
			return(-1);
		}

		strncpy(ip->data.chron.label, msg, l);
		ip->data.chron.label[l] = '\0';
		dl_add_to_list(lists[CHRON], ip);
		break;

	case TIMER:
		if (strcmp($1, "timer")) {
			sprintf(buf, "Bad entry, %s = timer.", $1);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_TIMER_TYPE))) {
			dl_error("Could not allocate timer item.");
			return(-1);
		}

		ip->data.timer.timer_num = pos;
		ip->data.timer.hours = hour;
		ip->data.timer.minutes = minute;
		ip->data.timer.second = second;
		ip->data.timer.repeat = repeat;
		ip->data.timer.chron = chron;
		l = strlen(msg);

		if (l > wi->max_timer_str)
			l = wi->max_timer_str;

		if ((ip->data.timer.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate timer label.");
			return(-1);
		}

		strncpy(ip->data.timer.label, msg, l);
		ip->data.timer.label[l] = '\0';
		dl_add_to_list(lists[TIMER], ip);
		break;
	case APP:

		if (strcmp($1, "appointment")) {
			sprintf(buf, "Bad entry, %s = appointment.", $1);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_APP_TYPE))) {
			dl_error("Could not allocate appointment item.");
			return(-1);
		}

		ip->data.app.month = month;
		ip->data.app.day = day;
		minute = hour*60+minute;
		ip->data.app.time = 0;

		for (min = 960, mask = 0x40; mask; mask >>= 1, min /= 2) {

			if (minute >= min) {
				ip->data.app.time |= mask;
				minute -= min;
			}

		}

		l = strlen(msg);

		if (l > wi->max_str)
			l = wi->max_str;

		if ((ip->data.app.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate appointment label.");
			return(-1);
		}

		strncpy(ip->data.app.label, msg, l);
		ip->data.app.label[l] = '\0';
		dl_add_to_list(lists[APP], ip);
		break;
	case PHONE:

		if (strcmp($1, "phone")) {
			sprintf(buf, "Bad entry, %s = phone.", $1);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_PHONE_TYPE))) {
			dl_error("Could not allocate phone item.");
			return(-1);
		}

		l = strlen(phnum);

		if (l > wi->max_phone_str)
			l = wi->max_phone_str;

		if ((ip->data.phone.number = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate phone number.");
			return(-1);
		}

		strncpy(ip->data.phone.number, phnum, l);
		ip->data.phone.number[l] = '\0';

		l = strlen(msg);

		if (l > wi->max_str)
			l = wi->max_str;

		if ((ip->data.phone.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate phone label.");
			return(-1);
		}

		strncpy(ip->data.phone.label, msg, l);
		ip->data.phone.label[l] = '\0';
		dl_add_to_list(lists[PHONE], ip);
		break;
	case TODO:

		if (strcmp($1, "todo")) {
			sprintf(buf, "Bad entry, %s = todo.", $1);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_TODO_TYPE))) {
			dl_error("Could not allocate todo item.");
			return(-1);
		}

		ip->data.todo.priority = prio;
		l = strlen(msg);

		if (l > wi->max_str)
			l = wi->max_str;

		if ((ip->data.todo.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate todo label.");
			return(-1);
		}

		strncpy(ip->data.todo.label, msg, l);
		ip->data.todo.label[l] = '\0';
		dl_add_to_list(lists[TODO], ip);
		break;
	case ANNIV:

		if (strcmp($1, "anniversary")) {
			sprintf(buf, "Bad entry, %s = anniversary.", $1);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_ANNIV_TYPE))) {
			dl_error("Could not allocate anniversary item.");
			return(-1);
		}

		ip->data.anniv.month = month;
		ip->data.anniv.day = day;

		l = strlen(msg);

		if (l > wi->max_str)
			l = wi->max_str;

		if ((ip->data.anniv.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate anniversary label.");
			return(-1);
		}

		strncpy(ip->data.anniv.label, msg, l);
		ip->data.anniv.label[l] = '\0';
		dl_add_to_list(lists[ANNIV], ip);
		break;
	case SYSTEM:

		if (strcmp($1, "system")) {
			sprintf(buf, "Bad entry, %s = system.", $1);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_SYSTEM_TYPE))) {
			dl_error("Could not allocate system item.");
			return(-1);
		}

		ip->data.system.chime = chime;
		ip->data.system.beep = beep;
		dl_add_to_list(lists[SYSTEM], ip);
		break;
	case DATA:

		if (*msg == '~')
			sprintf(file, "%s%s", getenv("HOME"), &msg[1]);
		else
			strcpy(file, msg);

		if ((fd = open(file, O_RDONLY)) < 0) {
			sprintf(buf, "Could not open %s for reading %s.", file, $1);
			dl_error(buf);
			return(-1);
		}

		if (fstat(fd, &sbuf) < 0) {
			sprintf(buf, "Could not stat %s for %s.", file, $1);
			dl_error(buf);
			return(-1);
		}

		if (strcmp($1, "wristapp") == 0) {

			if (!(ip = dl_new_item(wi, DL_WRISTAPP_TYPE))) {
				dl_error("Could not allocate system item.");
				return(-1);
			}

		}
		else if (strcmp($1, "melody") == 0) {

			if (!(ip = dl_new_item(wi, DL_MELODY_TYPE))) {
				dl_error("Could not allocate system item.");
				return(-1);
			}

		}
		else {
			sprintf(buf, "Bad entry, %s = string.", $1);
			dl_error(buf);
			return(-1);
		}

		ip->data.wristapp.len = sbuf.st_size;

		if ((ip->data.wristapp.data = (char *)malloc(sbuf.st_size)) == NULL) {
			sprintf(buf, "Could not allocate data for %s.", $1);
			dl_error(buf);
			return(-1);
		}

		if (read(fd, ip->data.wristapp.data, sbuf.st_size) != sbuf.st_size) {
			sprintf(buf, "Could not read data for %s from %s.", $1, file);
			dl_error(buf);
			return(-1);
		}

		if (strcmp($1, "wristapp") == 0)
			dl_add_to_list(lists[WRISTAPP], ip);
		else
			dl_add_to_list(lists[MELODY], ip);

		break;
	case INTVAL:

		if (strcmp($1, "time_adjust") == 0) {
			wi->time_adjust = val;
		}
		else if (strcmp($1, "pre_notification_time") == 0) {
			wi->pre_notification_time = val;
		}
		else {
			sprintf(buf, "Bad entry, %s = intval.", $1);
			dl_error(buf);
			return(-1);
		}

		break;
	case TIMEZONE:

		if (strcmp($1, "timezone")) {
			sprintf(buf, "Bad entry, %s = timezone.", $1);
			dl_error(buf);
			return(-1);
		}

		if (!(ip = dl_new_item(wi, DL_TIME_TYPE))) {
			dl_error("Could not allocate timezone item.");
			return(-1);
		}

		l = strlen(msg);

		if (l > wi->max_tzlen)
			l = wi->max_tzlen;

		if ((ip->data.time.label = (char *)malloc(l + 1)) == NULL) {
			dl_error("Could not allocate timezone label.");
			return(-1);
		}

		strncpy(ip->data.time.label, msg, l);
		ip->data.time.label[l] = '\0';
		ip->data.time.offset = minute;
		ip->data.time.tz_num = pos;
		ip->data.time.month = 1;
		ip->data.time.day = 1;
		ip->data.time.hour_fmt = tfmt;
		ip->data.time.date_fmt = dfmt;
		dl_add_to_list(lists[TIMEZONE], ip);
		break;
	default:
		dl_error("Internal Error, unknown type.");
		return(-1);
	}

}
			| '\n'
            ;

value		: INTEGER ',' DATE ',' TIME ',' STRING ',' INTEGER
			/* Alarm */
			{
				pos = $1;
				sscanf($3, "%d/%d/%d", &month, &day, &year);
				sscanf($5, "%d:%d:%d", &hour, &minute, &second);
				strcpy(msg, $7);
				audible = $9;
				$$ = ALARM;
			}
			
			| INTEGER ',' STRING ',' STRING ',' INTEGER ',' INTEGER
			/* Timer */
			{
				pos = $1;
				sscanf($3, "%d:%d:%d", &hour, &minute, &second);
				strcpy(msg, $5);
				repeat = $7;
				chron = $9;
				$$ = TIMER;
			}
			
			| STRING ',' INTEGER
			/* Chron */
			{
				strcpy(msg, $1);
				pos = $3;
				$$ = CHRON;
			}

			| DATE ',' TIME ',' STRING
			/* Appointment */
			{
				sscanf($1, "%d/%d/%d", &month, &day, &year);
				sscanf($3, "%d:%d:%d", &hour, &minute, &second);
				strcpy(msg, $5);
				$$ = APP;
			}
			| INTEGER ',' STRING
			/* To do */
			{
				prio =  $1;
				strcpy(msg, $3);
				$$ = TODO;
			}
			| STRING ',' STRING
			/* Phone */
			{
				strcpy(phnum, $1);
				strcpy(msg, $3);
				$$ = PHONE;
			}
			| DATE ',' STRING
			/* Anniversary */
			{
				sscanf($1, "%d/%d/%d", &month, &day, &year);
				strcpy(msg, $3);
				$$ = ANNIV;
			}
			| INTEGER ',' INTEGER
			/* System */
			{
				chime =  $1;
				beep =  $3;
				$$ = SYSTEM;
			}
			| INTEGER ',' STRING ',' INTEGER ',' INTEGER ',' INTEGER
			/* Timezone */
			{
				pos = $1;
				strcpy(msg, $3);
				minute = $5;
				tfmt = $7;
				dfmt = $9;
				$$ = TIMEZONE;
			}
			| STRING
			{
				strcpy(msg, $1);
				$$ = DATA;
			}
			| INTEGER
			{
				val = $1;
				$$ = INTVAL;
			}
			;

%%
int dl_error(char *s)
{
	char buf[1024];

	snprintf(buf, sizeof buf,"%s on line %d.",s,line_num);
	(*dl_error_proc)(buf);
	return 0;
}

extern FILE *dl_in;

WatchInfoPtr
dl_read_save(datafile, type, times, alarms, chron, timers, apps, todos,
	phones, annivs, system, wristapp, melody)
char *datafile;
int type;
ListPtr *times;
ListPtr *alarms;
ListPtr *chron;
ListPtr *timers;
ListPtr *apps;
ListPtr *todos;
ListPtr *phones;
ListPtr *annivs;
ListPtr *system;
ListPtr *wristapp;
ListPtr *melody;
{
	char file[1024];
	char buf[1024];
	int r;

	if (*datafile == '~')
		sprintf(file, "%s%s", getenv("HOME"), &datafile[1]);
	else
		strcpy(file, datafile);

	watch_type = type;

	if ((dl_in = fopen(file,"r")) == NULL) {
		sprintf(buf,"Could not open %s for reading.", file);
		(*dl_error_proc)(buf);
		return(NULL);
	}

	r = dl_parse();
	fclose(dl_in);

	if (r)
		return(NULL);

	*alarms = lists[ALARM];
	*chron = lists[CHRON];
	*timers = lists[TIMER];
	*apps = lists[APP];
	*phones = lists[PHONE];
	*todos = lists[TODO];
	*annivs = lists[ANNIV];
	*system = lists[SYSTEM];
	*wristapp = lists[WRISTAPP];
	*melody = lists[MELODY];
	*times = lists[TIMEZONE];
	free(lists);
	return(wi);
}
