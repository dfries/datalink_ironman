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

#include "datalink.h"
#include "datalink_private.h"

int
dl_init_download(wi, times, alarms, chron, timers, apps, todos, phones,
	annivs, system, wristapp, melody)
WatchInfoPtr wi;
ListPtr times;
ListPtr alarms;
ListPtr chron;
ListPtr timers;
ListPtr system;
ListPtr apps;
ListPtr todos;
ListPtr phones;
ListPtr annivs;
ListPtr wristapp;
ListPtr melody;
{
	ItemPtr ip;
	char buf[1024];
	int i;
	int last_warn = 0;

/* Free up old download structure if it exists. */
	dl_free_download();

	dl_download_data.memory = 0;
	dl_download_data.app_size = 0;
	dl_download_data.phone_size = 0;
	dl_download_data.todo_size = 0;
	dl_download_data.anniv_size = 0;
	dl_download_data.pre_notification_time = wi->pre_notification_time;
	dl_download_data.max_alarm_str = wi->max_alarm_str;
	dl_download_data.max_timer_str = wi->max_timer_str;
	dl_download_data.max_phone_str = wi->max_phone_str;

/* Count and verify items. */
	if (times && times->download) {

		for (i = 0, ip = times->first; ip; ip = ip->next, i++) {

			if (!ip->data.time.tz_num)
				ip->data.time.tz_num = i + 1;

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad time item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_tz)
				return((*dl_error_proc)("Too many time items."));

		}

		if (i != times->count) {
			last_warn = (*dl_warn_proc)("Time count value incorrect.");
			times->count = i;
		}

	}

	if (chron && chron->download) {

		for (i = 0, ip = chron->first; ip; ip = ip->next, i++) {

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad chron item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_chrons)
				return((*dl_error_proc)("Too many chron items."));

		}

		if (i != chron->count) {
			last_warn = (*dl_warn_proc)("Chron count value incorrect.");
			chron->count = i;
		}

	}

	if (alarms && alarms->download) {

		for (i = 0, ip = alarms->first; ip; ip = ip->next, i++) {

			if (!ip->data.alarm.alarm_num)
				ip->data.alarm.alarm_num = i + 1;

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad alarm item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_alarms)
				return((*dl_error_proc)("Too many alarm items."));

		}

		if (i != alarms->count) {
			last_warn = (*dl_warn_proc)("Alarm count value incorrect.");
			alarms->count = i;
		}

	}

	if (timers && timers->download) {

		for (i = 0, ip = timers->first; ip; ip = ip->next, i++) {

			if (!ip->data.timer.timer_num)
				ip->data.timer.timer_num = i + 1;

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad timer item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_timers)
				return((*dl_error_proc)("Too many timer items."));

		}

		if (i != timers->count) {
			last_warn = (*dl_warn_proc)("Timer count value incorrect.");
			timers->count = i;
		}

	}

	if (apps && apps->download) {

		for (i = 0, ip = apps->first; ip; ip = ip->next, i++) {

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad appointment item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_apps)
				return((*dl_error_proc)("Too many appointment items."));

			dl_download_data.memory += dl_sizeof_item(wi, ip);
			dl_download_data.app_size += dl_sizeof_item(wi, ip);
		}

		if (i != apps->count) {
			last_warn = (*dl_warn_proc)("Appointment count value incorrect.");
			apps->count = i;
		}

	}

	if (todos && todos->download) {

		for (i = 0, ip = todos->first; ip; ip = ip->next, i++) {

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad todo item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_todos)
				return((*dl_error_proc)("Too many todo items."));

			dl_download_data.memory += dl_sizeof_item(wi, ip);
			dl_download_data.todo_size += dl_sizeof_item(wi, ip);
		}

		if (i != todos->count) {
			last_warn = (*dl_warn_proc)("To Do count value incorrect.");
			todos->count = i;
		}

	}

	if (phones && phones->download) {

		for (i = 0, ip = phones->first; ip; ip = ip->next, i++) {

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad phone item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_phones)
				return((*dl_error_proc)("Too many phone items."));

			dl_download_data.memory += dl_sizeof_item(wi, ip);
			dl_download_data.phone_size += dl_sizeof_item(wi, ip);
		}

		if (i != phones->count) {
			last_warn = (*dl_warn_proc)("Phone count value incorrect.");
			phones->count = i;
		}

	}

	if (annivs && annivs->download) {

		for (i = 0, ip = annivs->first; ip; ip = ip->next, i++) {

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad anniversary item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_annivs)
				return((*dl_error_proc)("Too many anniversary items."));

			dl_download_data.memory += dl_sizeof_item(wi, ip);
			dl_download_data.anniv_size += dl_sizeof_item(wi, ip);
		}

		if (i != annivs->count) {
			last_warn = (*dl_warn_proc)("Anniversary count value incorrect.");
			annivs->count = i;
		}

	}

	if (system && system->download) {

		for (i = 0, ip = system->first; ip; ip = ip->next, i++) {

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad sytem item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_system)
				return((*dl_error_proc)("Too many sytem items."));

		}

		if (i != system->count) {
			last_warn = (*dl_warn_proc)("System count value incorrect.");
			system->count = i;
		}

	}

	if (wristapp && wristapp->download) {

		for (i = 0, ip = wristapp->first; ip; ip = ip->next, i++) {

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad wristapp item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_wristapp)
				return((*dl_error_proc)("Too many wristapp items."));

		}

		if (i != wristapp->count) {
			last_warn = (*dl_warn_proc)("WristApp count value incorrect.");
			wristapp->count = i;
		}

	}

	if (melody && melody->download) {

		for (i = 0, ip = melody->first; ip; ip = ip->next, i++) {

			if (!dl_item_ok(wi, ip)) {
				sprintf(buf, "Bad melody item #%d", i + 1);
				return((*dl_error_proc)(buf));
			}

			if (i >= wi->max_melody)
				return((*dl_error_proc)("Too many melody items."));

		}

		if (i != melody->count) {
			last_warn = (*dl_warn_proc)("Melody count value incorrect.");
			melody->count = i;
		}

	}

/* Check to see if memory usage has been exceeded. */

	if (dl_download_data.memory > wi->max_mem)
		return((*dl_error_proc)("Too much data."));

/* OK. We know we have valid data that will fit in the watch. Allocate
   download structure. */

	if (times && times->download) {

	   if ((dl_download_data.times =
			   (TimePtr)calloc(times->count, sizeof(Time))) == NULL)
			return((*dl_error_proc)("Can't allocate time download data."));

		dl_download_data.num_times = times->count;

		for (i = 0, ip = times->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.times[i],
				(char *)&ip->data.time, sizeof(Time));
		}

	}

	if (chron && chron->download) {

	   if ((dl_download_data.chron =
			   (ChronPtr)calloc(chron->count, sizeof(Chron))) == NULL)
			return((*dl_error_proc)("Can't allocate chron download data."));

		dl_download_data.num_chron = chron->count;

		for (i = 0, ip = chron->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.chron[i],
				(char *)&ip->data.chron, sizeof(Chron));
		}

	}

	if (alarms && alarms->download) {

	   if ((dl_download_data.alarms =
			   (AlarmPtr)calloc(alarms->count, sizeof(Alarm))) == NULL)
			return((*dl_error_proc)("Can't allocate alarm download data."));

		dl_download_data.num_alarms = alarms->count;

		for (i = 0, ip = alarms->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.alarms[i],
				(char *)&ip->data.alarm, sizeof(Alarm));
		}

	}

	if (timers && timers->download) {

	   if ((dl_download_data.timers =
			   (TimerPtr)calloc(timers->count, sizeof(Timer))) == NULL)
			return((*dl_error_proc)("Can't allocate timer download data."));

		dl_download_data.num_timers = timers->count;

		for (i = 0, ip = timers->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.timers[i],
				(char *)&ip->data.timer, sizeof(Timer));
		}

	}

	if (apps && apps->download) {

	   if ((dl_download_data.apps =
			   (AppointmentPtr)calloc(apps->count, sizeof(Appointment))) == NULL)
			return((*dl_error_proc)("Can't allocate appointment download data."));

		dl_download_data.num_apps = apps->count;

		for (i = 0, ip = apps->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.apps[i],
				(char *)&ip->data.app, sizeof(Appointment));
		}

	}

	if (todos && todos->download) {

	   if ((dl_download_data.todos =
			   (ToDoPtr)calloc(todos->count, sizeof(ToDo))) == NULL)
			return((*dl_error_proc)("Can't allocate todo download data."));

		dl_download_data.num_todos = todos->count;

		for (i = 0, ip = todos->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.todos[i],
				(char *)&ip->data.todo, sizeof(ToDo));
		}

	}

	if (phones && phones->download) {

	   if ((dl_download_data.phones =
			   (PhonePtr)calloc(phones->count, sizeof(Phone))) == NULL)
			return((*dl_error_proc)("Can't allocate phone download data."));

		dl_download_data.num_phones = phones->count;

		for (i = 0, ip = phones->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.phones[i],
				(char *)&ip->data.phone, sizeof(Phone));
		}

	}

	if (annivs && annivs->download) {

	   if ((dl_download_data.annivs =
			   (AnniversaryPtr)calloc(annivs->count, sizeof(Anniversary))) == NULL)
			return((*dl_error_proc)("Can't allocate anniversary download data."));

		dl_download_data.num_annivs = annivs->count;

		for (i = 0, ip = annivs->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.annivs[i],
				(char *)&ip->data.anniv, sizeof(Anniversary));
		}

	}

	if (system && system->download) {

	   if ((dl_download_data.system =
			   (SystemPtr)calloc(system->count, sizeof(System))) == NULL)
			return((*dl_error_proc)("Can't allocate system download data."));

		dl_download_data.num_system = system->count;

		for (i = 0, ip = system->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.system[i],
				(char *)&ip->data.system, sizeof(System));
		}

	}

	if (wristapp && wristapp->download) {

	   if ((dl_download_data.wristapp =
			   (WristAppPtr)calloc(wristapp->count, sizeof(WristApp))) == NULL)
			return((*dl_error_proc)("Can't allocate wristapp download data."));

		dl_download_data.num_wristapp = wristapp->count;

		for (i = 0, ip = wristapp->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.wristapp[i],
				(char *)&ip->data.wristapp, sizeof(WristApp));
		}

	}

	if (melody && melody->download) {

	   if ((dl_download_data.melody =
			   (MelodyPtr)calloc(melody->count, sizeof(Melody))) == NULL)
			return((*dl_error_proc)("Can't allocate melody download data."));

		dl_download_data.num_melody = melody->count;

		for (i = 0, ip = melody->first; ip; ip = ip->next, i++) {
			memcpy((char *)&dl_download_data.melody[i],
				(char *)&ip->data.melody, sizeof(Melody));
		}

	}

	return(0);
}
