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

#include <unistd.h>
#include <stdio.h>
#include <vga.h>
#include <string.h>
#include <time.h>
#include "datalink.h"
#include "datalink_macros.h"

int send_data(int type, unsigned char **packets, int npckts)
{
	int i;
	register int j;
	register unsigned char byte;
	unsigned char white[640];
	unsigned char black[640];
	int inc = (type == DATALINK_70) ? 1 : 2;

	memset(white, vga_white(), sizeof(white));
	memset(black, 0, sizeof(black));

/* Become root to set vga mode. */
	seteuid(0);
	vga_init();

/* This is redundant - vga_init() should do this itself... */
/* but there's no harm in being sure */
	seteuid(getuid());

#ifdef MACH64_HACK
/* For some reason, the latest mach 64 svga driver requires root on the
   next call even though vga_init gave up root privileges. */
	seteuid(0);
#endif
	if(!vga_hasmode(G640x480x16))
	{
		fprintf(stderr, "G640x480x2 graphics mode isn't supported, "
			"please report it\n");
		return 1;
	}
	if(vga_setmode(G640x480x16))
	{
		fprintf(stderr, "Couldn't set G640x480x2 even though it "
			"claimed to support it\n");
		return 2;
	}
#ifdef MACH64_HACK
	seteuid(getuid());
#endif

	#if 1
	/* write out the sync bytes */
	WRITE_BYTE1(1, 0x55);
	WRITE_BYTE2(1, 0x55);
	//sync(); // try to sync the disks now so they aren't active later
	{
		/* Sleep for 200 frames worth of output. */
		int syncframes=200;
		struct timespec tv={ syncframes/60,
			(syncframes%60)*1000000/60};
		nanosleep( &tv, 0);
	}
	#else
	/* Or just write and busywait 200 frames */
	SYNC
	#endif
	DATASTART for (i = 0; i < npckts; i++)
	{

		for (j = 0; j <= *packets[i]; j += inc)
		{
			vga_waitretrace();
			byte = packets[i][j];
			WRITE_BYTE1(1, byte) if (type == DATALINK_70)
				continue;

			if (j + 1 == packets[i][0])
				WRITE_BYTE2(0, 0xff)
				else
			{
				byte = packets[i][j + 1];
			WRITE_BYTE2(1, byte)}

		}

	END_PACKET}

#ifdef MACH64_HACK
/* For some reason, the latest mach 64 svga driver requires root on the
   next call even though vga_init gave up root privileges. */
	seteuid(0);
#endif
	vga_setmode(TEXT);
#ifdef MACH64_HACK
	seteuid(getuid());
#endif

	return 0;
}
