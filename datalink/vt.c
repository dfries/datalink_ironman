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
 * Linux version of Timex/Microsoft SDK for Timex datalink watches.
 * SVGA needs a terminal when going to graphics mode.  This will take
 * care of that.
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/vt.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>

#define VTFMT "/dev/tty%d"

int open_vt()
{
	struct vt_stat vts;
	int fd;
	int newvt;
	char buf[1024];

/* Need to become root again to deal with vt's */
	seteuid(0);

	if ((fd = open("/dev/tty", O_RDWR)) == -1)
	{
		perror("open");
		return (-1);
	}

/* See if we are on a VT. */
	if (ioctl(fd, VT_GETSTATE, &vts) == 0)
	{
		close(fd);
		return (0);
	}

/* We are not on a VT, switch to one. */
	close(fd);

	if ((fd = open("/dev/tty0", O_RDWR)) == -1)
	{
		perror("open");
		return (-1);
	}

/* Get info on current vt. */
	if (ioctl(fd, VT_GETSTATE, &vts) == -1)
	{
		perror("VT_GETSTATE");
		return (-1);
	}

/* Open a new vt. */
	if (ioctl(fd, VT_OPENQRY, &newvt) == -1)
	{
		perror("VT_OPENQRY");
		return (-1);
	}

	if (newvt == -1)
	{
		fprintf(stderr, "No free vts to open\n");
		return (-1);
	}

/* Make the new vt, the active vt. */
	if (ioctl(fd, VT_ACTIVATE, newvt) == -1)
	{
		perror("VT_ACTIVATE");
		return (-1);
	}

	if (ioctl(fd, VT_WAITACTIVE, newvt) == -1)
	{
		perror("VT_WAITACTIVE");
		return (-1);
	}

	close(fd);
	close(2);
	close(1);
	close(0);
/*
   Detach from controlling terminal here so that we can get a new
   controlling terminal.
*/
	setsid();
	sprintf(buf, VTFMT, newvt);
	(void) open(buf, O_RDWR);
	(void) open(buf, O_RDWR);
	(void) open(buf, O_RDWR);

/* No longer need root privs - drop them*/
	seteuid(getuid());

	return (vts.v_active);
}

void close_vt(int oldvt)
{
	int fd;
	struct vt_stat vts;
	struct vt_mode VT;
	int vt;

	if (!oldvt)
		return;

/* Need to become root again to deal with vt's */
	seteuid(0);

/* SVGA lib sets the tty change mode to require a call back.
 * We aren't in graphics mode, so set it back to auto.
 */
	if( ioctl(0, VT_GETMODE, &VT) == -1)
	{
		perror("VT_GETMODE");
		return;
	}

	VT.mode = VT_AUTO;
	if( ioctl(0, VT_SETMODE, &VT)== -1)
	{
		perror("VT_SETMODE");
		return;
	}

/* Get info on current vt. */
	if (ioctl(0, VT_GETSTATE, &vts) == -1)
	{
		perror("VT_GETSTATE");
		return;
	}

	vt = vts.v_active;

/* Switch back to previous vt. */

	if (ioctl(0, VT_ACTIVATE, oldvt) == -1)
	{
		perror("VT_ACTIVATE");
		return;
	}

	if (ioctl(0, VT_WAITACTIVE, oldvt) == -1)
	{
		perror("VT_WAITACTIVE");
		return;
	}

	close(0);
	close(1);
	close(2);
	setsid();

/* Open current vt. */
	if ((fd = open("/dev/tty0", O_RDWR)) == -1)
	{
		perror("open");
		return;
	}

/* Free up our old vt. */
	if (ioctl(fd, VT_DISALLOCATE, vt) == -1)
	{
		perror("VT_DISALLOCATE");
		return;
	}

	close(fd);

/* No longer need root privs - drop them*/
	seteuid(getuid());
}
