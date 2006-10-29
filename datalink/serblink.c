/* 
 * Copyright 1996-2002 - Tommy Johnson <tjohnson@bobdbob.com>
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
 * Unix implementation of the Timex Datalink protocol using an LED on a serial
 * port.  (see datalink.ps)
 *
 * See http://csgrad.cs.vt.edu/~tjohnson/ for more info.
 */

/* This section by David Fries <david@fries.net>
 * Using a photo cell hooked up directly to the microphone input of a
 * computer it is easy to catch the output waveform of either the CRT
 * blinking or the LED serial port blinking.
 *
 * The sample rate of one CRT capture was 44100 stereo (it's an old capture,
 * I've since found that the system does better with 48000 recordings).
 * A complete retrace took 1477.5 stereo samples / 2 = 738.75 mono samples.
 * 738.75/44100=.016751 seconds per retrace
 * 44100/738.75=59.695 frames per second, which is as expected.
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

#include <sys/time.h>
#include <sys/resource.h>

#ifdef linux
#define cfsetspeed cfsetospeed
#endif

#ifndef PORT
#define PORT "/dev/ttyd1"
#endif

void sendbyte(int fil, unsigned char dat)
#define ZERO "\0xff\0\0\0\0\0"
#define ONE "\0\0\0\0\0\0"
#define LEN 6
{
	static int frame = 0;
	char buff[1024];
	int c = 0;
	int i;
	memcpy(&buff[c], ZERO, LEN);
	c += LEN;
	for (i = 0; i < 8; i++)
	{
		memcpy(&buff[c], (dat & 1) ? ONE : ZERO, LEN);
		c += LEN - (i & 1);	/* CHEESY! the baud rate dosn't quite  */
		/* match, so alternate too long and too */
		/* short.    */
		dat >>= 1;	/* and... IT WORKS...  (shudder)       */
	}
	for (i = 0; i < (frame ? 12 : 4); i++)
	{
		memcpy(&buff[c], ONE, LEN);
		c += LEN;
	}
	frame = !frame;		/* a frame is 2 words, which would have been on */
	/* a single CRT frame (different inter-word pause) */

	write(fil, buff, c);
}

/* For pauses between packets: */
static void __pause(int fil, int count)
{
	char buff[8192];
	int i, sent;
	memset(buff, ZERO[1], sizeof(buff));
	for(i=0; i<count*LEN; i+=sent)
	{
		sent=count*LEN-i;
		if(sent>sizeof(buff))
			sent=sizeof(buff);
		write(fil, buff, sent);
	}
}

#undef ZERO
#undef ONE
#undef LEN

void Usage(const char *prog)
{
	fprintf(stderr,
		"%s [-d device] [-f file]\n"
		"Transmitt a file to a Timex Datalink watch useing an LED on a serial\n"
		"port.  Timex does not support this product, DO NOT ask them questions about\n"
		"it.  See http://csgrad.cs.vt.edu/~tjohnson/ for more info.\n"
		"Version $Id$\n",
		prog);
	exit(0);
}

int main(int argc, char **argv)
{
	int port;
	int data;
	struct termios old, new;
	int i, j, len, plen;
	unsigned char buff[4096];
	char fil[1024];
	char device[1024];

	setpriority(PRIO_PROCESS, 0, -20);	/* pauses will screwup the timing */

	strcpy(fil, "DEBUGOUTPUT");
	strcpy(device, PORT);	/* defaults */

#ifdef OTHERCMDLINE
	while ((c = getopt(argc, argv, "h?d:f:")) != -1)
	{
		switch (c)
		{
		case 'h':
		case '?':
			Usage(argv[0]);
			break;
		case 'd':
			strcpy(device, optarg);
			break;
		case 'f':
			strcpy(fil, optarg);
			break;
		}
	}
#else
	if (argc == 2)
		strcpy(fil, argv[1]);
	else
	{
		Usage(argv[0]);
	}
#endif

	data = open(fil, O_RDONLY);
	if (!data)
	{
		perror("open datafile failed:");
		exit(1);
	}
	len = read(data, buff, sizeof(buff));
	close(data);
	printf("data length:%d\n", len);

	port = open(device, O_RDWR);
	if (port < 0)
	{
		perror("open of device failed:");
		exit(1);
	}

	if (tcgetattr(port, &old) < 0)
	{
		perror("tcgetattr failed:");
		exit(1);
	}
	new = old;
	cfsetspeed(&new, B115200);
	if (tcsetattr(port, TCSANOW, &new) < 0)
	{
		perror("tcsetattr failed:");
		exit(1);
	}
#if 0
	while (1)
		__pause(port, 20);	/* used for hardware debugging */
#endif
	printf("sync1\n");
	for (i = 0; i < 500; i++)
	{
		sendbyte(port, 0x55);
		if (!(i % 100))
			printf("%d\n", 5 - (i / 100));
	}
	for (i = 0; i < 50; i++)
		sendbyte(port, 0xAA);
	printf("sync2\n");
	i = 0;
	do
	{
		plen = buff[i];
		printf("%d\n", plen);
		for (j = 0; j < plen; j++)
			sendbyte(port, buff[i++]);
		if (plen & 1)
			sendbyte(port, 0);
		__pause(port, 240);
	}
	while (i < len);

	__pause(port, 240);
	tcsetattr(port, TCSADRAIN, &new);
	sleep(1);
	close(port);
	return 0;
}
