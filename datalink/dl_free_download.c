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

#include "datalink.h"
#include "datalink_private.h"


void
dl_free_download()
{
/* Free up old download structure if it exists. */
	if (dl_download_data.times)
		free(dl_download_data.times);

	if (dl_download_data.alarms)
		free(dl_download_data.alarms);

	if (dl_download_data.timers)
		free(dl_download_data.timers);

	if (dl_download_data.apps)
		free(dl_download_data.apps);

	if (dl_download_data.phones)
		free(dl_download_data.phones);

	if (dl_download_data.todos)
		free(dl_download_data.todos);

	if (dl_download_data.annivs)
		free(dl_download_data.annivs);

	if (dl_download_data.system)
		free(dl_download_data.system);

	if (dl_download_data.wristapp)
		free(dl_download_data.wristapp);

	if (dl_download_data.melody)
		free(dl_download_data.melody);

	memset((char *)&dl_download_data, 0, sizeof(Download));
}
