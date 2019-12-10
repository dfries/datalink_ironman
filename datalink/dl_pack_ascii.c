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

#include "datalink.h"

int dl_pack_ascii(unsigned char *to, const char *from)
{
	int c;
	int p = 0;
	int o = 0;

	while (*from)
	{

		if (o == 0)
		{
			to[p] = dl_pack_char(*from);
			o = 6;
			from++;
			continue;
		}

		c = dl_pack_char(*from);
		to[p] |= c << o;
		to[++p] = c >> (8 - o);
		o = (o + 6) % 8;
		from++;
	}

	if (o)
	{
		to[p] |= 0x3f << o;
		to[++p] = 0x3f >> (8 - o);
		o = (o + 6) % 8;
		if (o)
			p++;
	} else
		to[p++] = 0x3f;

	return (p);
}
