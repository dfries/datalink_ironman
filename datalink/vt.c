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

	if ((fd = open("/dev/tty", O_RDWR)) < 0)
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

	if ((fd = open("/dev/tty0", O_RDWR)) < 0)
	{
		perror("open");
		return (-1);
	}

/* Get info on current vt. */
	if (ioctl(fd, VT_GETSTATE, &vts) < 0)
	{
		perror("VT_GETSTATE");
		return (-1);
	}

/* Open a new vt. */
	if (ioctl(fd, VT_OPENQRY, &newvt) < 0)
	{
		perror("VT_OPENQRY");
		return (-1);
	}

	if (newvt < 1)
	{
		fprintf(stderr, "No free vts to open\n");
		return (-1);
	}

/* Make the new vt, the active vt. */
	if (ioctl(fd, VT_ACTIVATE, newvt) < 0)
	{
		perror("VT_ACTIVATE");
		return (-1);
	}

	if (ioctl(fd, VT_WAITACTIVE, newvt) < 0)
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

	return (vts.v_active);
}

void close_vt(int oldvt)
{
	int fd;
	struct vt_stat vts;
	int vt;

	if (!oldvt)
		return;

/* Need to become root again to deal with vt's */
	seteuid(0);

/* Get info on current vt. */
	if (ioctl(0, VT_GETSTATE, &vts) < 0)
	{
		perror("VT_GETSTATE");
		return;
	}

	vt = vts.v_active;

/* Switch back to previous vt. */

	if (ioctl(0, VT_ACTIVATE, oldvt) < 0)
	{
		perror("VT_ACTIVATE");
		return;
	}

	if (ioctl(0, VT_WAITACTIVE, oldvt) < 0)
	{
		perror("VT_WAITACTIVE");
		return;
	}

	close(0);
	close(1);
	close(2);
	setsid();

/* Open current vt. */
	if ((fd = open("/dev/tty0", O_RDWR)) < 0)
	{
		perror("open");
		return;
	}

/* Free up our old vt. */
	if (ioctl(fd, VT_DISALLOCATE, vt) < 0)
	{
		perror("VT_DISALLOCATE");
		return;
	}

	close(fd);
}
