/* This software is Copyright 1996 by Karl R. Hakimian
 *
 * datazap: Linux version of Timex/Microsoft SDK for Timex datalink watches.
 *
 * Written by Karl R. Hakimian 10/3/96
 *
 * Modified by David Fries <dfries@mail.win.org> 7/11/99
 * - Added support for the Timex Datalink Ironman Triathlon
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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "datalink.h"
#include "datalink_private.h"

#define MAX_PCKT 38

#define TIME_70 0x30
#define DSTART_70 0x60
#define DATA_70 0x61
#define DEND_70 0x62

static unsigned char start1[] = {7, 0x20, 0, 0, 3, 0, 0};
static unsigned char datablock1[] = {	0x20, 0x70, 0x02, 0x40,
					0x05, 0xa9, 0x22, 0x5f,
					0xe6, 0xb2, 0xe8, 0xbb,
					0xe7, 0xb2, 0xe8, 0xbb,
					0xe7, 0xbb, 0xe8, 0xb2,
					0xe7, 0xb2, 0x5c, 0xa3,
					0x09, 0x26, 0xed, 0x15,
					0xa9, 0x01, 0x00, 0x00 };
static unsigned char datablock2[] = {	0x14, 0x70, 0x02, 0x5a,
					0xa9, 0x02, 0x14, 0xa9,
					0xb6, 0xa9, 0xa4, 0x07,
					0x47, 0xb7, 0xa9, 0xcc,
					0x74, 0x6f, 0x00, 0x00};
static unsigned char time[] = {17, 0x32, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0,
							    1, 0, 0, 0};
static unsigned char dstart[] = {5, 0x93, 0, 0, 0};
static unsigned char dinfo[] = {20, 0x90, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0,
							     0, 0, 0, 0, 0, 0, 0};
static unsigned char dspace[] = {4, 0x91, 0, 0};
static unsigned char dend[] = {5, 0x92, 0, 0, 0};
static unsigned char alarm[] = {18, 0x50, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0,
							     0, 0, 0, 0, 0};
static unsigned char timer[] = { 0x11, 0x43, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0 };
static unsigned char sysinfo[] = {6, 0x71, 0, 0, 0, 0};
static unsigned char end1[] = {4, 0x21, 0, 0};

static unsigned char pre60[] = {6, 0x23, 2, 0x40, 0, 0};
static unsigned char numdatapackets[] = {5, 0x60, 0, 0, 0};

_write_data(fd, buf, data, size, pnum, type, wi)
int fd;
unsigned char *buf;
unsigned char *data;
int size;
int *pnum;
int type;
WatchInfoPtr wi;
{
	int bytes_left;

	while (*buf + size > MAX_PCKT - 2) {
		bytes_left = *buf + size - MAX_PCKT + 2;
		memcpy(&buf[buf[0]], data, size - bytes_left);
		buf[0] = MAX_PCKT;
		dl_docrc(buf);

		if (write(fd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write data to tmp file."));

		data += size - bytes_left;
		size = bytes_left;

		memcpy(buf, dspace, *dspace);

		if (wi->dl_device == DATALINK_70)
			buf[1] = DATA_70;

		buf[2] = type;
		buf[3] = (*pnum)++;
	}

	if (!size)
		return(0);

	memcpy(&buf[buf[0]], data, size);
	buf[0] += size;
	return(1);
}

dl_send_data(wi, type)
WatchInfoPtr wi;
int type;
{
	char fname[1024];
	unsigned char buf[64];
	unsigned char data[64];
	unsigned short addr = 0x0236;
	AppointmentPtr ap;
	ToDoPtr tp;
	PhonePtr pp;
	AnniversaryPtr anp;
	WristAppPtr wristapp;
	SystemPtr sys;
	MelodyPtr melody;
	int ofd;
	int i;
	int pnum;
	int pid;
	int status;
	int ret;
	int p;

	if (type == BLINK_FILE)
		strcpy(fname, "DEBUGOUTPUT");
	else if (!tmpnam(fname))
		return((*dl_error_proc)("Can't create tmp file."));

	if ((ofd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1) {
		sprintf(buf, "Can't open %s for writing.", fname);
		return((*dl_error_proc)(buf));
	}

	memcpy(buf, start1, *start1);

	if (wi->dl_device == DATALINK_70)
		buf[4] = 1;
	if (wi->dl_device == DATALINK_IRONMAN)
	{
		buf[4] = 9;
	}

	dl_docrc(buf);
 
	if (write(ofd, buf, *buf) != *buf)
		return((*dl_error_proc)("Can't write start to tmp file."));

	if( wi->dl_device == DATALINK_IRONMAN )
	{
		memcpy(buf, datablock1, *datablock1);
		dl_docrc(buf);
		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)(
				"Can't write start block1 to tmp file."));

		memcpy(buf, datablock2, *datablock1);
		dl_docrc(buf);
		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)(
				"Can't write start block2 to tmp file."));
	}


	for (i = 0; i < dl_download_data.num_times; i++) {
		memcpy(buf, time, *time);
		p = 2;
		if( wi->dl_device == DATALINK_IRONMAN )
		{
			buf[0] = 0x0e;
			buf[1] = 0x30;
		}

		buf[p++] = dl_download_data.times[i].tz_num;

		if (wi->dl_device == DATALINK_150 )
			buf[p++] = dl_download_data.times[i].seconds;

		if (wi->dl_device == DATALINK_IRONMAN )
		{
			buf[p++] = dl_download_data.times[i].hours;
			buf[p++] = dl_download_data.times[i].minutes;
			buf[p++] = dl_download_data.times[i].month;
			buf[p++] = dl_download_data.times[i].day;
			buf[p++] = dl_download_data.times[i].year;
			buf[p++] = dl_download_data.times[i].dow;
			buf[p++] = dl_download_data.times[i].seconds;
		}
		else
		{
			buf[p++] = dl_download_data.times[i].hours;
			buf[p++] = dl_download_data.times[i].minutes;
			buf[p++] = dl_download_data.times[i].month;
			buf[p++] = dl_download_data.times[i].day;
			buf[p++] = dl_download_data.times[i].year;
			buf[p++] = dl_pack_char(dl_download_data.times[i].label[0]);
			buf[p++] = dl_pack_char(dl_download_data.times[i].label[1]);
			buf[p++] = dl_pack_char(dl_download_data.times[i].label[2]);
			buf[p++] = dl_download_data.times[i].dow-1;
		}

		if (wi->dl_device == DATALINK_150 || wi->dl_device == DATALINK_IRONMAN)
		{
			buf[p++] = dl_download_data.times[i].hour_fmt;
			buf[p++] = dl_download_data.times[i].date_fmt&0xFF;
		}

		if (wi->dl_device == DATALINK_70)
			buf[1] = TIME_70;

		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write time to tmp file."));

	}

	if (wi->dl_device != DATALINK_IRONMAN && dl_download_data.memory) {
		memcpy(buf, dstart, *dstart);
		buf[2] = 1;
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write dstart to tmp file."));

		memcpy(buf, dinfo, *dinfo);
		buf[2] = 1;
		buf[3] = dl_download_data.memory/(MAX_PCKT - 6);

		if (dl_download_data.memory%(MAX_PCKT - 6))
			buf[3]++;

		buf[12] = dl_download_data.num_apps;
		buf[13] = dl_download_data.num_todos;
		buf[14] = dl_download_data.num_phones;
		buf[15] = dl_download_data.num_annivs;
		buf[4] = (addr>>8)&0xff;
		buf[5] = addr&0xff;
		addr += dl_download_data.app_size;
		buf[6] = (addr>>8)&0xff;
		buf[7] = addr&0xff;
		addr += dl_download_data.todo_size;
		buf[8] = (addr>>8)&0xff;
		buf[9] = addr&0xff;
		addr += dl_download_data.phone_size;
		buf[10] = (addr>>8)&0xff;
		buf[11] = addr&0xff;
		buf[16] = 0x62;
		buf[17] = dl_download_data.pre_notification_time/5;
		if (!buf[17]) buf[17] = 0xff;

		if (wi->dl_device == DATALINK_70)
			buf[1] = DATA_70;

		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write dinfo to tmp file."));

		pnum = 1;
		memcpy(buf, dspace, *dspace);

		if (wi->dl_device == DATALINK_70)
			buf[1] = DATA_70;

		buf[2] = 1;
		buf[3] = pnum++;

		for (i = 0; i < dl_download_data.num_apps; i++) {
			ap = &dl_download_data.apps[i];
			data[0] = dl_pack_size(ap->label);
			data[1] = ap->month;
			data[2] = ap->day;
			data[3] = ap->time;

			if (data[0] != dl_pack_ascii(&data[4], ap->label))
				return((*dl_error_proc)("ERROR Bad pack_ascii.\n"));

			*data += 4;

			if (!_write_data(ofd, buf, data, *data, &pnum, 1, wi))
				return((*dl_error_proc)("Can't write to tmp file."));

		}

		for (i = 0; i < dl_download_data.num_todos; i++) {
			tp = &dl_download_data.todos[i];
			data[0] = dl_pack_size(tp->label);
			data[1] = tp->priority;

			if (data[0] != dl_pack_ascii(&data[2], tp->label))
				return((*dl_error_proc)("ERROR Bad pack_ascii.\n"));

			*data += 2;

			if (!_write_data(ofd, buf, data, *data, &pnum, 1, wi))
				return((*dl_error_proc)("Can't write to tmp file."));

		}

		for (i = 0; i < dl_download_data.num_phones; i++) {
			pp = &dl_download_data.phones[i];
			data[0] = dl_pack_size(pp->label);
			dl_pack_phone(&data[1], pp->number, dl_download_data.max_phone_str);

			if (data[0] != dl_pack_ascii(&data[7], pp->label)) {
				printf("ERROR Bad pack_ascii.\n");
				exit(-1);
			}

			*data += dl_download_data.max_phone_str/2;
			(*data)++;

			if (!_write_data(ofd, buf, data, *data, &pnum, 1, wi))
				return((*dl_error_proc)("Can't write to tmp file."));

		}

		for (i = 0; i < dl_download_data.num_annivs; i++) {
			anp = &dl_download_data.annivs[i];
			data[0] = dl_pack_size(anp->label);
			data[1] = anp->month;
			data[2] = anp->day;

			if (data[0] != dl_pack_ascii(&data[3], anp->label)) {
				printf("ERROR Bad pack_ascii.\n");
				exit(-1);
			}

			*data += 4;

			if (!_write_data(ofd, buf, data, *data, &pnum, 1, wi))
				return((*dl_error_proc)("Can't write to tmp file."));

		}

		*buf += 2;
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

		memcpy(buf, dend, *dend);
		buf[2] = 1;
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

	}

	if( wi->dl_device == DATALINK_IRONMAN && dl_download_data.chron )
	{
		/* The memdata is a where all the data is stored and when all the
		 * information to be sent that inclues the chron label and laps, and the
		 * phone data.  The information is broken up into packets and sent one at
		 * a time.
		 */
		/* should be overkill, don't know the right value*/
		#define maxdatasize 4096
		char memdata[maxdatasize] = { 0 };
		int p = 0;
		int labelsize;
		int packets;
		int offset = 0;
		int size;

		for (i = 0; i < dl_download_data.num_chron; i++) {
			memdata[p++] = 0;
			memdata[p++] = 0xe;
			memdata[p++] = 0;
			#warning has dl_download_data.chron[i].memused been initalized
			memdata[p++] = dl_download_data.chron[i].memused;
			memdata[p++] = dl_download_data.chron[i].chron_laps;
			if ( dl_download_data.num_phones )
				memdata[p++] = dl_download_data.num_phones;
			else
				memdata[p++] = 0;
			dl_fill_pack_ascii(&memdata[p], dl_download_data.chron[i].label,
				dl_download_data.max_chron_str, ' ');
			p += 8;
		}

		for (i = 0; i < dl_download_data.num_phones; i++) {
			pp = &dl_download_data.phones[i];
			labelsize = dl_pack_size(pp->label);
			/* This is the size taken up by this phone entry,
			 * the size of the label + 6 for the digits and
			 * one for this byte
			 */
			memdata[p++] = labelsize + 7; 
			dl_pack_phone(&memdata[p], pp->number, dl_download_data.max_phone_str);
			p += 6;

			if (labelsize != dl_pack_ascii(&memdata[p], pp->label)) {
				printf("ERROR Bad pack_ascii.\n");
				exit(-1);
			}
			p += labelsize;
		}

		if (p > maxdatasize )
		{
			printf("Error buffer overflow, too much data or too small of buffer\n");
			exit(-1);
		}

		/* set how many packets it will take to send the data, there are up
		 * to 32 bytes per packet, 27 of those are data bytes */
		packets = p/27 + (p%27? 1 : 0);

		/* packet before the one that lists how many data packets to send */
		memcpy(buf, pre60, *pre60);
		dl_docrc(buf);
		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

		/* send the packet that lists how many data packets are to follow */
		memcpy(buf, numdatapackets, *numdatapackets);
		buf[2] = packets;
		dl_docrc(buf);
		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

		for( i = 0; i < packets; i++ )
		{
			if( i < packets-1 )
				buf[0] = 32;
			else
				buf[0] = p-27*(packets-1) + 5;
			size = buf[0] - 5;
			buf[1] = 0x61;
			buf[2] = i+1;
			buf[3] = i;
			memcpy(&buf[3], &memdata[offset], size);
			dl_docrc(buf);
			if (write(ofd, buf, *buf) != *buf)
				return((*dl_error_proc)("Can't write to tmp file."));
			offset += size;
		}

		buf[0] = 4;
		buf[1] = 0x62;
		dl_docrc(buf);
		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

		/*
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));
		*/
		for( i = 0; i < p; i++ )
		{
			#ifdef DEBUGGING_FILE
			printf(" 0x%0.2x", 0xff&((unsigned int)memdata[i]) );
			#endif /* DEBUGGING_FILE */
			if( !p%8 )
				printf("\n");
		}
	}

	/* timezone label packet for IRONMAN watch */
	if( wi->dl_device == DATALINK_IRONMAN )
	for (i = 0; i < dl_download_data.num_times; i++) {
		p = 0;
		buf[p++] = 8;
		buf[p++] = 0x31;

		buf[p++] = dl_download_data.times[i].tz_num;
		buf[p++] = dl_pack_char(dl_download_data.times[i].label[0]);
		buf[p++] = dl_pack_char(dl_download_data.times[i].label[1]);
		buf[p++] = dl_pack_char(dl_download_data.times[i].label[2]);
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write timezone label to tmp file."));
	}



	for (i = 0; i < dl_download_data.num_alarms; i++) {
		memcpy(buf, alarm, *alarm);
		buf[2] = dl_download_data.alarms[i].alarm_num;
		buf[3] = dl_download_data.alarms[i].hours;
		buf[4] = dl_download_data.alarms[i].minutes;
		buf[5] = dl_download_data.alarms[i].month;
		buf[6] = dl_download_data.alarms[i].day;
		if( wi->dl_device == DATALINK_IRONMAN)
		{
			buf[7] = dl_download_data.alarms[i].audible;
			dl_fill_pack_ascii(&buf[8], dl_download_data.alarms[i].label,
				dl_download_data.max_alarm_str, ' ');
			buf[0] = 0x1a;
			buf[1] = 0x50;
		}
		else
		{
		dl_fill_pack_ascii(&buf[7], dl_download_data.alarms[i].label,
			dl_download_data.max_alarm_str, ' ');
		buf[15] = dl_download_data.alarms[i].audible;
		}
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

	}

	for (i = 0; i < dl_download_data.num_timers; i++) {
		memcpy(buf, timer, *timer);
		buf[2] = dl_download_data.timers[i].timer_num;
		buf[3] = dl_download_data.timers[i].hours;
		buf[4] = dl_download_data.timers[i].minutes;
		buf[5] = dl_download_data.timers[i].second;
		buf[6] = dl_download_data.timers[i].repeat;
		buf[6] |= dl_download_data.timers[i].chron << 1;

		dl_fill_pack_ascii(&buf[7], dl_download_data.timers[i].label,
			dl_download_data.max_timer_str, ' ');
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

	}

	if (dl_download_data.num_wristapp) {
		wristapp = dl_download_data.wristapp;
		memcpy(buf, dstart, *dstart);
		buf[2] = 2;
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

		memcpy(buf, dinfo, *dinfo);
		buf[2] = 2;
		buf[3] = wristapp->len/(MAX_PCKT - 6);

		if (wristapp->len%(MAX_PCKT - 6))
			buf[3]++;

		buf[4] = 1;
		*buf = 7;

		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

		memcpy(buf, dspace, *dspace);

		if (wi->dl_device == DATALINK_70)
			buf[1] = DATA_70;

		pnum = 1;
		buf[2] = 2;
		buf[3] = pnum++;

		if (!_write_data(ofd, buf, wristapp->data, wristapp->len, &pnum, 2, wi))
			return((*dl_error_proc)("Can't write to tmp file."));

		if (*buf != 4) {
			dl_docrc(buf);

			if (write(ofd, buf, *buf) != *buf)
				return((*dl_error_proc)("Can't write to tmp file."));

		}

		memcpy(buf, dend, *dend);
		buf[2] = 2;
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

	}

	if (dl_download_data.num_melody) {
		melody = dl_download_data.melody;
		memcpy(buf, dstart, *dstart);
		buf[2] = 3;
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

		memcpy(buf, dinfo, *dinfo);
		buf[2] = 3;
		buf[3] = melody->len/(MAX_PCKT - 6);

		if (melody->len%(MAX_PCKT - 6))
			buf[3]++;

		buf[4] = 0xff - melody->len;
		*buf = 7;

		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

		memcpy(buf, dspace, *dspace);

		if (wi->dl_device == DATALINK_70)
			buf[1] = DATA_70;

		pnum = 1;
		buf[2] = 3;
		buf[3] = pnum++;

		if (!_write_data(ofd, buf, melody->data, melody->len, &pnum, 3, wi))
			return((*dl_error_proc)("Can't write to tmp file."));

		if (*buf != 4) {
			dl_docrc(buf);

			if (write(ofd, buf, *buf) != *buf)
				return((*dl_error_proc)("Can't write to tmp file."));

		}

		memcpy(buf, dend, *dend);
		buf[2] = 3;
		dl_docrc(buf);

		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

	}

	if (dl_download_data.num_system) {
		sys = dl_download_data.system;
		memcpy(buf, sysinfo, *sysinfo);
		if( wi->dl_device == DATALINK_IRONMAN)
		{
			buf[0] = 5;
			buf[1] = 0x32;
			buf[2] = sys->chime;
			buf[2] |= sys->beep << 1;
		}
		else
		{
			buf[2] = sys->chime;
			buf[3] = sys->beep;
		}

		dl_docrc(buf);
		if (write(ofd, buf, *buf) != *buf)
			return((*dl_error_proc)("Can't write to tmp file."));

	}

	memcpy(buf, end1, *end1);
	dl_docrc(buf);

	if (write(ofd, buf, *buf) != *buf)
		return((*dl_error_proc)("Can't write to tmp file."));

	close(ofd);

	switch (type) {
	case BLINK_FILE:
		ret = 0;
		break;
	case SVGA_BLINK:

		switch ((pid = fork())) {
		case -1:
			return((*dl_error_proc)("Could not execute svga blink engine."));
			return(-1);
		case 0:	/* Child */
			sprintf(buf, "svgablink", BINDIR);

			if (wi->dl_device == DATALINK_IRONMAN)
			{
				execlp(buf, "svgablink", "-ironman", fname, NULL);
				sprintf(buf, "./svgablink", BINDIR);
				execl(buf, "svgablink", "-ironman", fname, NULL);
			}
			else
			if (wi->dl_device == DATALINK_150)
			{
				execlp(buf, "svgablink", "-150", fname, NULL);
				sprintf(buf, "./svgablink", BINDIR);
				execl(buf, "svgablink", "-150", fname, NULL);
			}
			else
			{
				execlp(buf, "svgablink", "-70", fname, NULL);
				sprintf(buf, "./svgablink", BINDIR);
				execl(buf, "svgablink", "-70", fname, NULL);
			}

			(*dl_error_proc)("Could not execute svga blink engine.");
			exit(-1);
		default:

			if (waitpid(pid, &status, 0) < 0)
				perror("waitpid");

			(void)unlink(fname);

			if (WIFEXITED(status))
				ret = WEXITSTATUS(status);
			else
				ret = -1;

			break;
		}

		break;
	case SER_BLINK:

		switch ((pid = fork())) {
		case -1:
			return((*dl_error_proc)("Could not fork child for serial blink engine."));
			return(-1);
		case 0:	/* Child */
			sprintf(buf, "serblink", BINDIR);
			execlp(buf, "serblink", fname, NULL);
			sprintf(buf, "./serblink", BINDIR);
			execl(buf, "serblink", fname, NULL);
			(*dl_error_proc)("Could not execute serial blink engine.");
			exit(-1);
		default:

			if (waitpid(pid, &status, 0) < 0)
				perror("waitpid");

			(void)unlink(fname);

			if (WIFEXITED(status))
				ret = WEXITSTATUS(status);
			else
				ret = -1;

			break;
		}

		break;
	}

	return(ret);
}
