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

/*
 * SVGA Macros for sending data to the datalink watch.
 *
 * Modified by David Fries <dfries@mail.win.org> 7/11/99
 * - Added support for the Timex Datalink Ironman Triathlon
 * - Modified the screen position and only send half the initial sync signals
 *
 */

#define START1 70
#define START2 255
#define SPACE 15

#define WRITE_BYTE1(sb, byte) { \
	int pos = START1; \
	register int mask = 0x1; \
	if (sb) \
		vga_drawscanline(pos, white); \
	else \
		vga_drawscanline(pos, black); \
	pos += SPACE; \
	while (mask < 0xff) { \
		if (mask&byte) \
			vga_drawscanline(pos, black); \
		else \
			vga_drawscanline(pos, white); \
		pos += SPACE; \
		mask = mask << 1; \
	} \
}

#define WRITE_BYTE2(sb, byte) { \
	int pos = START2; \
	register int mask = 1; \
	if (sb) \
		vga_drawscanline(pos, white); \
	else \
		vga_drawscanline(pos, black); \
	pos += SPACE; \
	while (mask < 0xff) { \
		if (mask&byte) \
			vga_drawscanline(pos, black); \
		else \
			vga_drawscanline(pos, white); \
		pos += SPACE; \
		mask = mask << 1; \
	} \
}

#define END_PACKET { \
	vga_waitretrace(); \
	WRITE_BYTE1(0, 0xff) \
	WRITE_BYTE2(0, 0xff) \
	{int i; for (i = 0; i < (10+((type==DATALINK_IRONMAN)?2:0)); i++) \
		vga_waitretrace();} \
}

/* how many syncs to give to begin with, was 400 */
#define SYNC { \
	register int i; \
	for (i = 0; i < 200; i++) { \
		vga_waitretrace(); \
		WRITE_BYTE1(1, 0x55) \
		WRITE_BYTE2(1, 0x55) \
	} \
	END_PACKET \
}

#define DATASTART { \
	register int i; \
	for (i = 0; i < 50; i++) { \
		vga_waitretrace(); \
		WRITE_BYTE1(1, 0xAA) \
	} \
	END_PACKET \
}
