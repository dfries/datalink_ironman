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

/* crc lookup table */
static unsigned short int crc16_table[16] = {
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};

unsigned short int dl_docrc(unsigned char *data)
{
	int i;
	unsigned short int t;
	unsigned short int crc = 0;
	int l;

	l = data[0] - 2;

	for (i = 0; i < l; i++)
	{
		t = crc16_table[crc & 0xF];
		crc = (crc >> 4) & 0x0FFF;
		crc = crc ^ t ^ crc16_table[data[i] & 0xF];

		/* upper 4 bits */
		t = crc16_table[crc & 0xF];
		crc = (crc >> 4) & 0x0FFF;
		crc = crc ^ t ^ crc16_table[(data[i] >> 4) & 0xF];
	}

	data[l++] = crc >> 8;
	data[l] = crc & 0xff;
	return (crc);
}
