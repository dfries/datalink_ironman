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

#ifndef __DATALINK_PRIVATE_H__
#define __DATALINK_PRIVATE_H__

typedef struct download_s {
	int num_times;
	TimePtr times;
	int num_alarms;
	AlarmPtr alarms;
	int memory;
	int num_apps;
	int app_size;
	int pre_notification_time;
	AppointmentPtr apps;
	int num_todos;
	int todo_size;
	ToDoPtr todos;
	int max_alarm_str;
	int max_phone_str;
	int num_phones;
	int phone_size;
	PhonePtr phones;
	int num_annivs;
	int anniv_size;
	AnniversaryPtr annivs;
	int num_system;
	SystemPtr system;
	int num_wristapp;
	WristAppPtr wristapp;
	int num_melody;
	MelodyPtr melody;
} Download, *DownloadPtr;

extern Download dl_download_data;

#endif /* __DATALINK_PRIVATE_H__ */
