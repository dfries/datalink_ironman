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
#include "datalink.h"
#include "datalink_private.h"


int dl_item_ok(WatchInfoPtr wi, ItemPtr ip)
{
	char buf[1024];

	switch (ip->type)
	{
	case DL_TIME_TYPE:

		if (ip->data.time.tz_num < 1
		    || ip->data.time.tz_num > wi->max_tz)
		{
			sprintf(buf, "Bad time zone number #%d",
				ip->data.time.tz_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.time.hours > 23)
		{
			sprintf(buf, "Bad hour in time #%d",
				ip->data.time.tz_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.time.minutes > 59)
		{
			sprintf(buf, "Bad minute in time #%d",
				ip->data.time.tz_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.time.month < 1 || ip->data.time.month > 12)
		{
			sprintf(buf, "Bad month in time #%d",
				ip->data.time.tz_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.time.day < 1 || ip->data.time.day > 31)
		{
			sprintf(buf, "Bad day in time #%d",
				ip->data.time.tz_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (!dl_string_ok(ip->data.time.label, wi->max_str))
		{
			sprintf(buf, "Bad characters in label in time #%d",
				ip->data.time.tz_num);
			(*dl_warn_proc) (buf);
		}

		break;
	case DL_ALARM_TYPE:

		if (ip->data.alarm.alarm_num < 1 ||
		    ip->data.alarm.alarm_num > wi->max_alarms)
		{
			sprintf(buf, "Bad alarm number #%d",
				ip->data.alarm.alarm_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.alarm.hours > 23)
		{
			sprintf(buf, "Bad hour in alarm #%d",
				ip->data.alarm.alarm_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.alarm.minutes > 59)
		{
			sprintf(buf, "Bad minute in alarm #%d",
				ip->data.alarm.alarm_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.alarm.month > 12)
		{
			sprintf(buf, "Bad month in alarm #%d",
				ip->data.alarm.alarm_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.alarm.day > 31)
		{
			sprintf(buf, "Bad day in alarm #%d",
				ip->data.alarm.alarm_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (!dl_string_ok(ip->data.alarm.label, wi->max_alarm_str))
		{
			sprintf(buf, "Bad string in alarm #%d",
				ip->data.alarm.alarm_num);
			(*dl_warn_proc) (buf);
		}

		break;
	case DL_TIMER_TYPE:

		if (ip->data.timer.timer_num < 1 ||
		    ip->data.timer.timer_num > wi->max_timers)
		{
			sprintf(buf, "Bad timer number #%d",
				ip->data.timer.timer_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.timer.hours > 99)
		{
			sprintf(buf, "Bad hour in timer #%d",
				ip->data.timer.timer_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.timer.minutes > 59)
		{
			sprintf(buf, "Bad minute in timer #%d",
				ip->data.timer.timer_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.timer.second > 59)
		{
			sprintf(buf, "Bad second in timer #%d",
				ip->data.timer.timer_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.timer.repeat != 0
		    && ip->data.timer.repeat != 1)
		{
			sprintf(buf, "Bad repeat in timer #%d",
				ip->data.timer.timer_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.timer.chron != 0 && ip->data.timer.chron != 1)
		{
			sprintf(buf, "Bad chron in timer #%d",
				ip->data.timer.timer_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.timer.chron && ip->data.timer.repeat)
		{
			sprintf(buf,
				"Can't do repeat and chron at the end of a timer, in timer #%d",
				ip->data.timer.timer_num);
			(*dl_error_proc) (buf);
			return (0);
		}

		if (!dl_string_ok(ip->data.timer.label, wi->max_timer_str))
		{
			sprintf(buf, "Bad string in timer #%d",
				ip->data.timer.timer_num);
			(*dl_warn_proc) (buf);
		}

		break;
	case DL_APP_TYPE:

		if (ip->data.app.month < 1 || ip->data.app.month > 12)
		{
			sprintf(buf, "Bad month in appointment.");
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.app.day < 1 || ip->data.app.day > 31)
		{
			sprintf(buf, "Bad day in appointment.");
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.app.time > 0x7f)
		{
			sprintf(buf, "Bad time in appointment.");
			(*dl_error_proc) (buf);
			return (0);
		}

		if (!dl_string_ok(ip->data.app.label, wi->max_str))
		{
			sprintf(buf,
				"Bad characters in label in appointment.");
			(*dl_warn_proc) (buf);
		}

		break;
	case DL_CHRON_TYPE:

		if (!dl_string_ok(ip->data.chron.label, wi->max_chron_str))
		{
			sprintf(buf, "Bad characters in label in chron.");
			(*dl_warn_proc) (buf);
		}

		break;
	case DL_PHONE_TYPE:

		if (!dl_string_ok
		    (ip->data.phone.number, wi->max_phone_str))
		{
			sprintf(buf, "Bad characters in phone number.");
			(*dl_warn_proc) (buf);
		}

		if (!dl_string_ok(ip->data.phone.label, wi->max_str))
		{
			sprintf(buf, "Bad characters in label in phone.");
			(*dl_warn_proc) (buf);
		}

		break;
	case DL_TODO_TYPE:

		if (!dl_string_ok(ip->data.todo.label, wi->max_str))
		{
			sprintf(buf, "Bad characters in label in todo.");
			(*dl_warn_proc) (buf);
		}

		break;
	case DL_ANNIV_TYPE:

		if (ip->data.anniv.month < 1 || ip->data.anniv.month > 12)
		{
			sprintf(buf, "Bad month in anniversary.");
			(*dl_error_proc) (buf);
			return (0);
		}

		if (ip->data.anniv.day < 1 || ip->data.anniv.day > 31)
		{
			sprintf(buf, "Bad day in anniversary.");
			(*dl_error_proc) (buf);
			return (0);
		}

		if (!dl_string_ok(ip->data.anniv.label, wi->max_str))
		{
			sprintf(buf,
				"Bad characters in label in anniversary.");
			(*dl_warn_proc) (buf);
		}

		break;
	case DL_SYSTEM_TYPE:
		break;
	case DL_WRISTAPP_TYPE:
		break;
	case DL_MELODY_TYPE:
		break;
	default:
		(*dl_error_proc) ("Unknown type");
		return (0);
	}

	return (1);
}
