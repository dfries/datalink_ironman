/* 
 * Copyright 1997-2002 - Harold Zatz <hzatz@alumni.caltech.edu>
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
 * sgiglblink: SGI / Open GL program from writing to Timex datalink watches
 * Note the Open GL implimentation must support switching the buffers durring
 * the vertical retrace.  This program is SGI specific in the way it selects a
 * 60Hz screen refresh.  Alternate code just doesn't exist.
 */

#include <stdio.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/gl.h>

struct packet
{
	char *data;
	unsigned char size;
	struct packet *next;
};

static int attribList[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };

Display *display;
Window window;
int width;
int height;

#define SPACE 28

send(int sync0, unsigned char b0, int sync1, unsigned char b1)
{
	unsigned int mask = 1, scanline = 150;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);

	if (sync0)
	{
		glVertex2d(0, height - scanline);
		glVertex2d(width - 1, height - scanline++);
		glVertex2d(0, height - scanline);
		glVertex2d(width - 1, height - scanline++);
	} else
		scanline += 2;

	scanline += SPACE;

	while (mask != 256)
	{
		if (!(b0 & mask))
		{
			glVertex2d(0, height - scanline);
			glVertex2d(width - 1, height - scanline++);
			glVertex2d(0, height - scanline);
			glVertex2d(width - 1, height - scanline++);
		} else
			scanline += 2;

		mask <<= 1;
		scanline += SPACE;
	}

	mask = 1;
	scanline = 598;

	if (sync1)
	{
		glVertex2d(0, height - scanline);
		glVertex2d(width - 1, height - scanline++);
		glVertex2d(0, height - scanline);
		glVertex2d(width - 1, height - scanline++);
	} else
		scanline += 2;

	scanline += SPACE;

	while (mask != 256)
	{
		if (!(b1 & mask))
		{
			glVertex2d(0, height - scanline);
			glVertex2d(width - 1, height - scanline++);
			glVertex2d(0, height - scanline);
			glVertex2d(width - 1, height - scanline++);
		} else
			scanline += 2;

		mask <<= 1;
		scanline += SPACE;
	}

	glEnd();

	glXSwapBuffers(display, window);
}

main(int argc, char **argv)
{
	int fd, status, i;
	unsigned char size;
	struct packet *last = 0, *cur, *first = 0;
	XVisualInfo *visualInfo;
	XEvent event;
	XSetWindowAttributes attribs;
	GLXContext context;

	/* Check proper number of args. */
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s data_file\n", argv[0]);
		exit(-1);
	}

	/* Open the data file. */
	if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		fprintf(stderr, "Could not open %s for reading.\n",
			argv[1]);
		exit(-1);
	}

	/* Build some packets. */
	while (status = read(fd, &size, 1) == 1)
	{
		cur = (struct packet *) malloc(sizeof(struct packet));

		if (cur == 0)
		{
			fprintf(stderr,
				"Could not allocate packet memory.\n");
			exit(-1);
		}

		cur->data = (char *) malloc(size);
		cur->data[0] = size;
		if (read(fd, cur->data + 1, size - 1) != size - 1)
		{
			fprintf(stderr, "Could not read packet data.\n");
			exit(-1);
		}

		if (cur == 0)
		{
			fprintf(stderr,
				"Could not allocate packet data memory.\n");
			exit(-1);
		}

		cur->size = size;
		cur->next = 0;

		if (last)
			last->next = cur;
		else
			first = cur;

		last = cur;
	}

	if (status != 0)
	{
		fprintf(stderr, "File error reading from %s.\n", argv[1]);
		exit(-1);
	}

	close(fd);

	/* Build a window. */
	display = XOpenDisplay(0);
	if (!display)
	{
		fprintf(stderr, "Can't open display\n");
		exit(-1);
	}

	/* Set the monitor to a 60 Hz scanrate. */
	system("/usr/gfx/setmon -n 60HZ");

	/* Get a visual. */
	visualInfo = glXChooseVisual(display, 0, attribList);

	/* Get a colormap for that visual. */
	attribs.colormap = XCreateColormap(display,
					   DefaultRootWindow(display),
					   visualInfo->visual, AllocNone);

	/* We want no borders. */
	attribs.border_pixel = 0;

	/* We don't want the window manager mucking with us. */
	attribs.override_redirect = 1;

	width = XDisplayWidth(display, 0);
	height = XDisplayHeight(display, 0);

	/* Build a window filling the display in that visual. */
	window = XCreateWindow(display, DefaultRootWindow(display),
			       0, 0, width, height, 0,
			       visualInfo->depth, InputOutput,
			       visualInfo->visual,
			       CWBorderPixel | CWColormap |
			       CWOverrideRedirect, &attribs);

	/* Build a gl context. */
	context = glXCreateContext(display, visualInfo, 0, True);
	glXMakeCurrent(display, window, context);
	glOrtho(0, width - 1, 0, height - 1, 0, 1);

	/* Map and raise the window. */
	XMapRaised(display, window);
	XFlush(display);

	/* Send sync stuff... */
	for (i = 0; i < 400; i++)
		send(1, 0x55, 1, 0x55);

	for (i = 0; i < 10; i++)
		send(0, 0xff, 0, 0xff);

	for (i = 0; i < 50; i++)
		send(1, 0xaa, 0, 0xff);

	for (i = 0; i < 10; i++)
		send(0, 0xff, 0, 0xff);

	/* Send the packets. */
	for (cur = first; cur; cur = cur->next)
	{
		for (i = 0; i < cur->size; i += 2)
			if (i - 1 == cur->size)
				send(1, cur->data[i], 0, 0xff);
			else
				send(1, cur->data[i], 1, cur->data[i + 1]);

		for (i = 0; i < 10; i++)
			send(0, 0xff, 0, 0xff);
	}

	/* Set the monitor back to a 72 Hz scanrate. */
	system("/usr/gfx/setmon -n 72HZ");
}
