/* This software is Copyright 1996 by Karl R. Hakimian
 *
 * datalink: Timex Data Link Watch download Library
 *
 * Written by Karl R. Hakimian 10/3/96
 * 
 * Permission is hereby granted to copy, reproduce, redistribute or otherwise
 * use this software as long as: there is no monetary profit gained
 * specifically from the use or reproduction of this software, it is not
 * sold, rented, traded or otherwise marketed, and this copyright notice is
 * included prominently in any copy made. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. ANY USE OF THIS
 * SOFTWARE IS AT THE USER'S OWN RISK.
 *
 */

#ifndef __DATALINK_H__
#define __DATALINK_H__

typedef struct time_s {
	unsigned char tz_num;
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
	unsigned char month;
	unsigned char day;
	unsigned char year;
	unsigned char dow;	/* Day Of the Week */
	unsigned char hour_fmt;
	unsigned char date_fmt;
	unsigned char download;
	int offset;
	char *label;
} Time, *TimePtr;

typedef struct alarm_s {
	unsigned char alarm_num;
	unsigned char hours;
	unsigned char minutes;
	unsigned char month;
	unsigned char day;
	char *label;
	unsigned char audible;
} Alarm, *AlarmPtr;

typedef struct chron_s {
	unsigned char chron_laps;
	char *label;
} Chron, *ChronPtr;

typedef struct timer_s {
	unsigned char timer_num;
	unsigned char hours;
	unsigned char minutes;
	unsigned char second;
	unsigned char repeat;
	unsigned char chron;
	char *label;
} Timer, *TimerPtr;

typedef struct system_s {
	unsigned char chime;
	unsigned char beep;
} System, *SystemPtr;

typedef struct appointment_s {
	unsigned char month;
	unsigned char day;
	unsigned char time;
	char *label;
} Appointment, *AppointmentPtr;

typedef struct phone_s {
	char *number;
	char *label;
} Phone, *PhonePtr;

typedef struct todo_s {
	unsigned char priority;
	char *label;
} ToDo, *ToDoPtr;

typedef struct anniversary_s {
	unsigned char month;
	unsigned char day;
	char *label;
} Anniversary, *AnniversaryPtr;

typedef struct wristapp_s {
	int len;
	unsigned char *data;
} WristApp, *WristAppPtr;

typedef struct melody_s {
	int len;
	unsigned char *data;
} Melody, *MelodyPtr;

typedef struct item_s {
	struct item_s *next;
	int type;
	union {
		Time time;
		Alarm alarm;
		Chron chron;
		Timer timer;
		System sys;
		Appointment app;
		Phone phone;
		ToDo todo;
		Anniversary anniv;
		System system;
		WristApp wristapp;
		Melody melody;
	} data;
} Item, *ItemPtr;

typedef struct list_s {
	ItemPtr first;
	ItemPtr last;
	int download;
	int count;
} List, *ListPtr;

typedef struct watch_info_s {
	int dl_device;		/* Device to download to. */
	int max_tz;
	int max_alarms;
	int max_apps;
	int max_chrons;
	int max_phones;
	int max_timers;
	int max_todos;
	int max_annivs;
	int max_system;
	int max_wristapp;
	int max_melody;
	int max_tzlen;
	int max_mem;		/* Memory available. */
	int mem_size;		/* Memory used. */
	int max_str;		/* Max string length for this device. */
	int max_alarm_str;	/* Max string length for an alarm on this device. */
	int max_chron_str;	/* Max string length for the cron on this device. */
	int max_phone_str;	/* Max string length for a phone on this device. */
	int max_timer_str;	/* Max string length for the timer on this device. */
	int max_wristapp_len;
	int max_mel_len;
	int pre_notification_time;
	int time_adjust;
	List times;
	List alarms;
	List chron;
	List timers;
	List system;
	List apps;
	List phones;
	List todos;
	List annivs;
	List wristapp;
	List melody;
} WatchInfo, *WatchInfoPtr;

/* defines */

#ifndef NULL
#define NULL 0l
#endif

/* Item types */
#define DL_NO_TYPE			0
#define DL_TIME_TYPE		1
#define DL_ALARM_TYPE		2
#define DL_APP_TYPE			3
#define DL_PHONE_TYPE		4
#define DL_TODO_TYPE		5
#define DL_ANNIV_TYPE		6
#define DL_SYSTEM_TYPE		7
#define DL_WRISTAPP_TYPE	8
#define DL_MELODY_TYPE		9
#define DL_TIMER_TYPE		10
#define DL_CHRON_TYPE		11
#define DL_MAX_TYPE			12

/* Output types */
#define NO_OUTPUT 0
#define SVGA_BLINK 1
#define SER_BLINK 2
#define BLINK_FILE 3

/* Watch types */
#define NO_WATCH 0
#define DATALINK_70 1
#define DATALINK_150 2
#define DATALINK_IRONMAN 3

#define DEF_LPTRANSINFO 1

extern int (*dl_error_proc)(char *);
extern int (*dl_warn_proc)(char *);

/* Function definitions. */

void dl_add_to_list(ListPtr, ItemPtr);
int dl_anniv_by_date(ItemPtr, ItemPtr);
int dl_anniv_by_label(ItemPtr, ItemPtr);
int dl_app_by_datetime(ItemPtr, ItemPtr);
int dl_app_by_label(ItemPtr, ItemPtr);
unsigned short int dl_docrc(unsigned char *);
void dl_fill_pack_ascii(unsigned char *, unsigned char *, int, char);
void dl_free_download(void);
int dl_init_download(WatchInfoPtr, ListPtr, ListPtr, ListPtr, ListPtr,
	ListPtr, ListPtr, ListPtr, ListPtr, ListPtr, ListPtr, ListPtr);
WatchInfoPtr dl_init_watch(int);
int dl_item_ok(WatchInfoPtr, ItemPtr);
ItemPtr dl_new_item(WatchInfoPtr, int);
ListPtr dl_new_list(void);
int dl_pack_ascii(unsigned char *, unsigned char *);
int dl_pack_char(char);
int dl_pack_digit(char);
void dl_pack_phone(unsigned char *, unsigned char *, int);
int dl_pack_size(char *);
int dl_phone_by_label(ItemPtr, ItemPtr);
WatchInfoPtr dl_read_save(char *, int, ListPtr *, ListPtr *, ListPtr *, 
	ListPtr *, ListPtr *, ListPtr *, ListPtr *, ListPtr *, ListPtr *,
	ListPtr *, ListPtr * );
int dl_send_data(WatchInfoPtr, int);
void dl_set_error(int (*)());
void dl_set_warn(int (*)());
int dl_sizeof_item(WatchInfoPtr, ItemPtr);
int dl_sizeof_list(WatchInfoPtr, ListPtr);
int dl_sort(ListPtr, int(*)());
int dl_string_ok(char *, int);
int dl_todo_by_label(ItemPtr, ItemPtr);
int dl_todo_by_prio(ItemPtr, ItemPtr);
int dl_write_save(char *, char *, char *);

#endif /* __DATALINK_H__ */
