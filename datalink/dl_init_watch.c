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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datalink.h"
#include "datalink_private.h"

int (*dl_error_proc) (char *);
int (*dl_warn_proc) (char *);

Download dl_download_data = { 0 };

int dl_default_error(char *msg)
{
	fprintf(stderr, "ERROR: %s\n", msg);
	return (-1);
}

int dl_default_warn(char *msg)
{
	fprintf(stderr, "WARNING: %s\n", msg);
	return (1);
}

WatchInfoPtr dl_init_watch(int type)
{
	WatchInfoPtr result;

	if (!dl_error_proc)
		dl_error_proc = dl_default_error;

	if (!dl_warn_proc)
		dl_warn_proc = dl_default_warn;

	if ((result = (WatchInfoPtr) malloc(sizeof(WatchInfo))) == NULL)
	{
		(*dl_error_proc) ("Could not allocate WatchInfo structure.");
		return (NULL);
	}

	memset((char *) result, 0, sizeof(WatchInfo));

	switch (type)
	{
	case DATALINK_70:
		result->dl_device = type;
		result->max_tz = 2;
		result->max_alarms = 5;
		result->max_chrons = 0;
		result->max_chron_str = 0;
		result->max_apps = 255;
		result->max_phones = 255;
		result->max_timers = 0;
		result->max_todos = 255;
		result->max_annivs = 255;
		result->max_system = 1;
		result->max_wristapp = 0;
		result->max_melody = 0;
		result->max_tzlen = 3;
		result->max_mem = 850;
		result->mem_size = 0;
		result->max_str = 15;
		result->max_alarm_str = 8;
		result->max_phone_str = 12;
		result->max_timer_str = 0;
		result->max_wristapp_len = 0;
		result->max_mel_len = 0;
		result->pre_notification_time = 0;
		result->time_adjust = 9;
		break;
	case DATALINK_150:
	case DATALINK_150S:
		result->dl_device = type;
		result->max_tz = 2;
		result->max_alarms = 5;
		result->max_chrons = 0;
		result->max_chron_str = 0;
		result->max_apps = 255;
		result->max_phones = 255;
		result->max_timers = 0;
		result->max_todos = 255;
		result->max_annivs = 255;
		result->max_system = 1;
		result->max_wristapp = 1;
		result->max_melody = 1;
		result->max_tzlen = 3;
		result->max_mem = 2048;
		result->mem_size = 0;
		result->max_str = 31;
		result->max_alarm_str = 8;
		result->max_phone_str = 12;
		result->max_timer_str = 0;
		result->max_wristapp_len = 742;
		result->max_mel_len = 64;
		result->pre_notification_time = 0;
		result->time_adjust = 9;
		break;
	case DATALINK_IRONMAN:
		result->dl_device = type;
		result->max_tz = 2;
		result->max_alarms = 10;
		result->max_chrons = 1;
		result->max_apps = 0;
		result->max_phones = 255;
		result->max_timers = 5;
		result->max_todos = 0;
		result->max_annivs = 0;
		result->max_system = 1;
		result->max_wristapp = 0;
		result->max_melody = 0;
		result->max_tzlen = 3;
		result->max_mem = 850;
		result->mem_size = 0;
		result->max_str = 15;
		result->max_alarm_str = 16;
		result->max_chron_str = 8;
		result->max_phone_str = 12;
		result->max_timer_str = 8;
		result->max_wristapp_len = 0;
		result->max_mel_len = 0;
		result->pre_notification_time = 0;
		result->time_adjust = 9;
		break;
	default:
		(*dl_error_proc) ("Unknown watch type.");
		return (NULL);
	}

	return (result);
}
