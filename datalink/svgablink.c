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
 * Linux version of Timex/Microsoft SDK for Timex datalink watches making use
 * of the svga library to select a graphic mode and draw on it.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include "datalink.h"

static void maxPriority();

int main(int argc, char **argv)
{
	int oldvt;
	unsigned char **data;
	char buf[256];
	int fd;
	unsigned char size;
	int n;
	int p = 0;
	int type = DATALINK_IRONMAN;

/* Drop down to user privileges until we need root privs. */
	seteuid(getuid());

	while (argc > 1 && *argv[1] == '-')
	{

		if (strcmp(argv[1], "-ironman") == 0
		    || strcmp(argv[1], "-modelironman") == 0)
			type = DATALINK_IRONMAN;
		else
		    if (strcmp(argv[1], "-150") == 0
			|| strcmp(argv[1], "-model150") == 0)
			type = DATALINK_150;
		else
		    if (strcmp(argv[1], "-150s") == 0
			|| strcmp(argv[1], "-model150s") == 0)
			type = DATALINK_150S;
		else
		    if (strcmp(argv[1], "-70") == 0
			|| strcmp(argv[1], "-model70") == 0)
			type = DATALINK_70;
		else
		{
			fprintf(stderr, "Unknown option %s.\n", argv[1]);
			fprintf(stderr, "Usage: %s [ -ironman | -modelironman | -150 | -model150 | -150s | \n"
				"\t-model150s | -70 | -model70 ] datafile\n",
				argv[0]);
			exit(-1);
		}

		argc--;
		argv++;
	}

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s data_file\n", argv[0]);
		exit(-1);
	}

	if ((data = (unsigned char **) calloc(sizeof(unsigned char *),
		1024)) == NULL)
	{
		fprintf(stderr, "Could not allocate data array.\n");
		exit(-1);
	}

/* Open data file */
	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		fprintf(stderr, "open datafile %s failed: %s\n", argv[1],
			strerror(errno));
		exit(-1);
	}

/* Read in packets. */
	while ((n = read(fd, &size, 1)) == 1)
	{
		size--;

		if ((n = read(fd, buf, size)) != size)
			break;

		size++;

		if ((data[p] = (char *) malloc(size)) == NULL)
		{
			fprintf(stderr,
				"Could not allocate data buffer.\n");
			exit(-1);
		}

		*data[p] = size;
		memcpy(&data[p++][1], buf, size - 1);
	};

	if (n != 0)
	{
		fprintf(stderr, "File error reading from %s\n", argv[1]);
		fprintf(stderr, "%d, %d, %d\n", n, size, p);
		exit(-1);
	}

/* We need to detach from the terminal and our parent may need to wait for us
   time to fork a child. */

	switch (fork())
	{
	case 0:
		break;
	case -1:
		perror("fork");
		exit(-1);
	default:
		exit(0);
	}

/* The rest of this needs to be as root, open_vt, will raise privileges */

	if ((oldvt = open_vt()) == -1)
		exit(-1);

	maxPriority();
	send_data(type, data, p);
	close_vt(oldvt);
	return 0;
}


static void maxPriority()
{
	struct sched_param scp;

	memset(&scp, '\0', sizeof(scp));
	scp.sched_priority = sched_get_priority_max(SCHED_RR);
	if (sched_setscheduler(0, SCHED_RR, &scp) < 0)
		fprintf(stderr, "WARNING: Cannot set RR-scheduler.\n");
	if (mlockall(MCL_CURRENT | MCL_FUTURE) < 0)
		fprintf(stderr, "WARNING: Cannot disable paging.\n");

}
