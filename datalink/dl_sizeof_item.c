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

#include <string.h>
#include "datalink.h"
#include "datalink_private.h"

int dl_sizeof_item(WatchInfoPtr wi, ItemPtr item)
{
	int size;
	int l;

	switch (item->type)
	{
	case DL_TIME_TYPE:
		size = 0;
		break;
	case DL_ALARM_TYPE:
		size = 0;
		break;
	case DL_APP_TYPE:
		l = strlen(item->data.app.label);

		if (l > wi->max_str)
		{


			(void) (*dl_warn_proc) ("Appointment label too long.");
			l = wi->max_str;
		}

		size = dl_pack_size(item->data.app.label) + 4;
		break;
	case DL_PHONE_TYPE:
		l = strlen(item->data.app.label);

		if (l > wi->max_str)
		{
			(void) (*dl_warn_proc) ("Phone label too long.");
			l = wi->max_str;
		}

		size = dl_pack_size(item->data.app.label) + 1;
		size += wi->max_phone_str / 2;
		break;
	case DL_TODO_TYPE:
		l = strlen(item->data.app.label);

		if (l > wi->max_str)
		{
			(void) (*dl_warn_proc) ("Todo label too long.");
			l = wi->max_str;
		}

		size = dl_pack_size(item->data.app.label) + 2;
		break;
	case DL_ANNIV_TYPE:
		l = strlen(item->data.app.label);

		if (l > wi->max_str)
		{


			(void) (*dl_warn_proc) ("Anniversary label too long.");
			l = wi->max_str;
		}

		size = dl_pack_size(item->data.app.label) + 4;
		break;
	case DL_SYSTEM_TYPE:
		size = 0;
		break;
	case DL_WRISTAPP_TYPE:
		size = 0;
		break;
	case DL_MELODY_TYPE:
		size = 0;
		break;
	default:
		size = 0;
		(void) (*dl_error_proc) ("Unknown type");
	}

	return (size);
}
