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

/* crc lookup table */
static unsigned short int crc16_table[16]={
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};

unsigned short int
dl_docrc(data, alligned)
unsigned char *data;
int alligned;	/* With most Ironman packets the crc checks are alligned
		 * so they are sent in the same frame, not all packets are */
{
	int i;
	unsigned short int t;
	unsigned short int crc = 0;
	int l;

	l = data[0] - 2;

	for (i = 0; i < l; i++) {
		t = crc16_table[crc & 0xF];
		crc = (crc >> 4) & 0x0FFF;
		crc = crc ^ t ^ crc16_table[data[i] & 0xF];

		/* upper 4 bits */
		t = crc16_table[crc & 0xF];
		crc = (crc >> 4) & 0x0FFF;
		crc = crc ^ t ^ crc16_table[(data[i] >> 4) & 0xF];
	}


	/* if it needs to be alligned and it is odd, add one, otherwise
	 * lave it as it is */
	if( alligned && l%2 )
	{
		l++;
	}
	data[l++] = crc>>8;
	data[l] = crc&0xff;
	return(crc);
}
