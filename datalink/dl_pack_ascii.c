/* This software is Copyright 1996 by Karl R. Hakimian
 *
 * datazap: Linux version of Timex/Microsoft SDK for Timex datalink watches.
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

int dl_pack_ascii(to, from)
unsigned char *to;
unsigned char *from;
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
