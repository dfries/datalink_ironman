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

void dl_pack_phone(to, from, len)
unsigned char *to;
unsigned char *from;
int len;
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
