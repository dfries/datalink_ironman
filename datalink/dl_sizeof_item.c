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
dl_sizeof_item(wi, item)
WatchInfoPtr wi;
ItemPtr item;
{
	int last_warn = 0;
	int size;
	int l;

	switch (item->type) {
	case DL_TIME_TYPE:
		size = 0;
		break;
	case DL_ALARM_TYPE:
		size = 0;
		break;
	case DL_APP_TYPE:
		l = strlen(item->data.app.label);

		if (l > wi->max_str) {
			(void)(*dl_warn_proc)("Appointment label too long.");
			l = wi->max_str;
		}

		size = dl_pack_size(item->data.app.label) + 4;
		break;
	case DL_PHONE_TYPE:
		l = strlen(item->data.app.label);

		if (l > wi->max_str) {
			(void)(*dl_warn_proc)("Phone label too long.");
			l = wi->max_str;
		}

		size = dl_pack_size(item->data.app.label) + 1;
		size += wi->max_phone_str/2;
		break;
	case DL_TODO_TYPE:
		l = strlen(item->data.app.label);

		if (l > wi->max_str) {
			(void)(*dl_warn_proc)("Todo label too long.");
			l = wi->max_str;
		}

		size = dl_pack_size(item->data.app.label) + 2;
		break;
	case DL_ANNIV_TYPE:
		l = strlen(item->data.app.label);

		if (l > wi->max_str) {
			(void)(*dl_warn_proc)("Anniversary label too long.");
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
		(void)(*dl_error_proc)("Unknown type");
	}

	return(size);
}
