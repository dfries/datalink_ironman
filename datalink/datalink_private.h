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

#ifndef __DATALINK_PRIVATE_H__
#define __DATALINK_PRIVATE_H__

typedef struct download_s
{
	int num_times;
	TimePtr times;
	int num_alarms;
	AlarmPtr alarms;
	int num_chron;
	int max_chron_str;
	ChronPtr chron;
	int num_timers;
	TimerPtr timers;
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
	int max_timer_str;
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

#endif				/* __DATALINK_PRIVATE_H__ */
