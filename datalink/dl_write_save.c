/* This software is Copyright 1996 by Karl R. Hakimian
 *
 * datalink: Timex Data Link Watch download Library
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

#include <stdio.h>

#include "datalink.h"
#include "datalink_private.h"

int
dl_write_save(datafile, wristappfile, melodyfile)
char *datafile;
char *wristappfile;
char *melodyfile;
{
	FILE *fp;
	ItemPtr ip;
	char bakfile[1024];
	char buf[1024];
	int last_warn = 0;
	TimePtr tip;
	AlarmPtr alp;
	AppointmentPtr ap;
	ToDoPtr tp;
	PhonePtr pp;
	AnniversaryPtr anp;
	SystemPtr sp;
	WristAppPtr wp;
	MelodyPtr mp;
	int hour, min;
	int i;

/* Create backup of old datafile. */

	sprintf(bakfile, "%s.bak", datafile);
	//(void)unlink(bakfile);

	if (link(datafile, bakfile) < 0)
		last_warn = (*dl_warn_proc)("Could not make backup.");

	//(void)unlink(datafile);

	if ((fp = fopen(datafile, "w")) == NULL)
		return((*dl_error_proc)("Could not write save file."));

	fprintf(fp, "# Data Link save file created by the datalink library.\n\n");

/* Save each type of data. */

	for (i = 0; i < dl_download_data.num_times; i++) {
		tip = &dl_download_data.times[i];
		fprintf(fp, "timezone = %d, \"%s\", %d, %d, %d\n",
			tip->tz_num, tip->label, tip->offset, tip->hour_fmt, tip->date_fmt);
	}

	if (dl_download_data.num_times)
		fprintf(fp, "\n");

	for (i = 0; i < dl_download_data.num_alarms; i++) {
		alp = &dl_download_data.alarms[i];
		fprintf(fp, "alarm = %d, %02d/%02d, %02d:%02d, \"%s\", %d\n",
			alp->alarm_num, alp->month, alp->day, alp->hours, alp->minutes,
			alp->label, alp->audible);
	}

	if (dl_download_data.num_alarms)
		fprintf(fp, "\n");

	for (i = 0; i < dl_download_data.num_apps; i++) {
		ap = &dl_download_data.apps[i];
		min = 0;

		for (i = 1; i < 0xFF; i <<= 1) {

			if ((i&ap->time))
				min += i*15;

		}

		hour = min/60;
		min = hour*60;

		fprintf(fp, "appointment = %02d/%02d, %02d:%02d, \"%s\"\n",
			ap->month, ap->day, hour, min, ap->label);
	}

	if (dl_download_data.num_apps)
		fprintf(fp, "\n");

	for (i = 0; i < dl_download_data.num_todos; i++) {
		tp = &dl_download_data.todos[i];
		fprintf(fp, "todo = %d, \"%s\"\n", tp->priority, tp->label);
	}

	if (dl_download_data.num_todos)
		fprintf(fp, "\n");

	for (i = 0; i < dl_download_data.num_phones; i++) {
		pp = &dl_download_data.phones[i];
		fprintf(fp, "phone = \"%s\", \"%s\"\n", pp->number, pp->label);
	}

	if (dl_download_data.num_phones)
		fprintf(fp, "\n");

	for (i = 0; i < dl_download_data.num_annivs; i++) {
		anp = &dl_download_data.annivs[i];
		fprintf(fp, "anniversary = %02d/%02d, \"%s\"\n",
			anp->month, anp->day, anp->label);
	}

	if (dl_download_data.num_annivs)
		fprintf(fp, "\n");

	for (i = 0; i < dl_download_data.num_system; i++) {
		sp = &dl_download_data.system[i];
		fprintf(fp, "system = %d, %d\n", sp->chime, sp->beep);
	}

	if (dl_download_data.num_system)
		fprintf(fp, "\n");

	if (wristappfile && *wristappfile)
		fprintf(fp, "wristapp = \"%s\"\n\n", wristappfile);

	if (melodyfile && *melodyfile)
		fprintf(fp, "melody = \"%s\"\n\n", melodyfile);

	return(last_warn);
}
