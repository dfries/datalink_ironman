/* This software is Copyright 1996 by Karl R. Hakimian
 *
 * datazap: Linux version of Timex/Microsoft SDK for Timex datalink watches.
 *
 * Written by Karl R. Hakimian 10/3/96
 *
 * Modified by David Fries <dfries@mail.win.org> 7/11/99
 * - Added support for the Timex Datalink Ironman Triathlon
 *
 * Modified by Danilo Fiorenzano <danilo@telusplanet.net> 9 Dec. 1999
 * - Added new function "maxPriority()" to switch to SCHED_RR kernel scheduling
 *   policy at maximum priority and lock all pages into real memory, in order
 *   to improve timing accuracy.
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

#include <stdio.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <sys/mman.h>

#define MODEL_70 0
#define MODEL_150 1
#define MODEL_IRONMAN 2

void maxPriority(void);

main(argc, argv)
int argc;
char **argv;
{
	int oldvt;
	char **data;
	char buf[256];
	int fd;
	unsigned char size;
	int n;
	int p = 0;
	int type = MODEL_IRONMAN;

	while (argc > 1 && *argv[1] == '-') {

		if (strcmp(argv[1], "-ironman") == 0 || strcmp(argv[1], "-modelironman") == 0)
			type = MODEL_IRONMAN;
		else
		if (strcmp(argv[1], "-150") == 0 || strcmp(argv[1], "-model150") == 0)
			type = MODEL_150;
		else
		if (strcmp(argv[1], "-70") == 0 || strcmp(argv[1], "-model70") == 0)
			type = MODEL_70;
		else
		{
			fprintf(stderr, "Unknown option %s.\n", argv[1]);
			exit(-1);
		}

		argc--;
		argv++;
	}

	if (argc != 2) {
		fprintf(stderr, "Usage: %s data_file\n", argv[0]);
		exit(-1);
	}

/* Drop down to user privileges to read file. */
	seteuid(getuid());

	if ((data = (char **)calloc(sizeof(char *), 1024)) == NULL) {
		fprintf(stderr, "Could not allocate data array.\n");
		exit(-1);
	}

/* Open data file */
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "Could not open %s for reading.\n", argv[1]);
		exit(-1);
	}

/* Read in packets. */
	while ((n = read(fd, &size, 1)) == 1) {
		size--;

		if ((n = read(fd, buf, size)) != size)
			break;

		size++;

		if ((data[p] = (char *)malloc(size)) == NULL) {
			fprintf(stderr, "Could not allocate data buffer.\n");
			exit(-1);
		}

		*data[p] = size;
		memcpy(&data[p++][1], buf, size - 1);
	};

	if (n != 0) {
		fprintf(stderr, "File error reading from %s\n", argv[1]);
		fprintf(stderr, "%d, %d, %d\n", n, size, p);
		exit(-1);
	}

/* We need to detach from the terminal and our parent may need to wait for us
   time to fork a child. */

	switch(fork()) {
	case 0:
		break;
	case -1:
		perror("fork");
		exit(-1);
	default:
		exit(0);
	}

/* The rest of this needs to be as root, open_vt, will raise privileges */
	
	if ((oldvt = open_vt()) < 0)
		exit(-1);

	maxPriority();
	send_data(type, data, p);
	close_vt(oldvt);
	exit(0);
}


void maxPriority(void) {
    struct sched_param scp;
    
    memset(&scp, '\0', sizeof(scp));
    scp.sched_priority = sched_get_priority_max(SCHED_RR);
    if (sched_setscheduler(0, SCHED_RR, &scp) < 0) 
	fprintf(stderr, "WARNING: Cannot set RR-scheduler.\n");
    if (mlockall(MCL_CURRENT|MCL_FUTURE) < 0)
	fprintf(stderr, "WARNING: Cannot disable paging.\n");
    
}
