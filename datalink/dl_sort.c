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
