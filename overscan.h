/*
 *  arch/arm/mach-bcm2708/include/mach/vcio.h
 *
 *  Copyright (C) 2010 Broadcom
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _OVERSCAN_H
#define _OVERSCAN_H

/* Mailbox property tags */
enum {
	VCMSG_GET_OVERSCAN               = 0x0004000a,
	VCMSG_TST_OVERSCAN               = 0x0004400a,
	VCMSG_SET_OVERSCAN               = 0x0004800a,
};

#include <linux/ioctl.h>

/* 
 * The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. 
 */
#define MAJOR_NUM 100

/* 
 * Set the message of the device driver 
 */
#define IOCTL_MBOX_PROPERTY _IOWR(MAJOR_NUM, 0, char *)
/*
 * _IOWR means that we're creating an ioctl command 
 * number for passing information from a user process
 * to the kernel module and from the kernel module to user process 
 *
 * The first arguments, MAJOR_NUM, is the major device 
 * number we're using.
 *
 * The second argument is the number of the command 
 * (there could be several with different meanings).
 *
 * The third argument is the type we want to get from 
 * the process to the kernel.
 */

/* 
 * The name of the device file 
 */
#define DEVICE_FILE_NAME "/dev/mailbox"

#endif
