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

ItemPtr dl_new_item(wi, type)
WatchInfoPtr wi;
int type;
{
	ItemPtr ni;

/* Since there is only one supported device, wi is not used. */

	if (type <= DL_NO_TYPE && type > DL_MAX_TYPE)
	{
		(*dl_error_proc) ("Unknown type, can't allocate.");
		return (NULL);
	}

	if ((ni = (ItemPtr) malloc(sizeof(Item))) == NULL)
	{
		(*dl_error_proc) ("Could not allocate item.");
		return (NULL);
	}

	memset((char *) ni, 0, sizeof(Item));

	ni->type = type;
	return (ni);
}
