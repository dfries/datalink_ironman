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
#include "datalink.h"

int dl_sort(ListPtr list, int (*cmp_proc) ())
{
	ListPtr nl;
	ItemPtr ip;
	ItemPtr ni;
	ItemPtr si;
	ItemPtr pi;

	if ((nl = dl_new_list()) == NULL)
		return ((*dl_error_proc) ("List alloc error."));

	for (ip = list->first; ip; ip = ni)
	{
		ni = ip->next;

		for (pi = NULL, si = nl->first; si; si = si->next)
		{

			if ((*cmp_proc) (ip, si) >= 0)
			{
				pi = si;
				continue;
			}

			if (pi)
			{
				ip->next = pi->next;
				pi->next = ip;
			} else
			{
				ip->next = nl->first;
				nl->first = ip;
			}

			nl->count++;

			if (!nl->last)
			{
				nl->last = ip;
				ip->next = NULL;
			}

			break;

		}

		if (!si)
		{
			dl_add_to_list(nl, ip);
			ip->next = NULL;
		}

	}

	list->first = nl->first;
	list->last = nl->last;
	free(nl);
	return 0;
}
