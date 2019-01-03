/*
 * QEMU Object Model
 *
 * Authors:
 *   Kang Li, Rohit Mullangi
 *
 * This work is licensed under the terms of the GNU GPL, 
 * version 2 or later.
 *
 */

#define MAX_DEV_SNAPSHOTS  1

/* pointers to a device snapshot */
struct dev_snapshots {
	const char * devname;
	int  statesize;
	char * snapshot;
	char * swapspace;  /* tmp buffer */
	const VMStateDescription *vmsd;	 
};

/* a qemu global variable for all DISC device snapshots*/
struct dev_snapshots g_dss[MAX_DEV_SNAPSHOTS];

int g_num_dss = 0;

/* Defined in hw/core/qdev.c */
void device_nap_reset(const char *name, void *opaque);
