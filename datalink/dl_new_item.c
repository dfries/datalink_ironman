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

#include <stdlib.h>
#include <string.h>
#include "datalink.h"

ItemPtr dl_new_item(WatchInfoPtr wi, int type)
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
