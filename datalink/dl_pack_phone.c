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

void dl_pack_phone(unsigned char *to, const char *from, int len)
{
	int i;
	int l;

	l = strlen(from);

	if (l > len)
		l = len;

	for (i = 0; i < l; i++)
	{

		if (i & 1)
			*to++ |= dl_pack_digit(from[i]) << 4;
		else
			*to = dl_pack_digit(from[i]);

	}

	for (i = l; i < len; i++)
	{

		if (i & 1)
			*to++ |= dl_pack_digit(' ') << 4;
		else
			*to = dl_pack_digit(' ');

	}

}
