/**
 * Copyright (c) 2012 Broadcom. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Tiny mod by Russell "ukscone" Davis to make it usable for the 
 * set_overscan.sh script. 2013-01-05
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "vcio.h"

#define PAGE_SIZE (4*1024)

static void *mapmem(unsigned base, unsigned size)
{
   int mem_fd;
   unsigned offset = base % PAGE_SIZE; 
   base = base - offset;
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit (-1);
   }
   void *mem = mmap(
      0,
      size,
      PROT_READ|PROT_WRITE,
      MAP_SHARED/*|MAP_FIXED*/,
      mem_fd, 
      base);
   if (mem == MAP_FAILED) {
      printf("mmap error %d\n", (int)mem);
      exit (-1);
   }
   close(mem_fd);
   return (char *)mem + offset;
}

static void *unmapmem(void *addr, unsigned size)
{
   int s = munmap(addr, size);
   if (s != 0) {
      printf("munmap error %d\n", s);
      exit (-1);
   }
}

/* 
 * use ioctl to send mbox property message
 */

static int mbox_property(int file_desc, void *buf)
{
   int ret_val = ioctl(file_desc, IOCTL_MBOX_PROPERTY, buf);

   if (ret_val < 0) {
      printf("ioctl_set_msg failed:%d\n", ret_val);
   }

   return ret_val;
}

static unsigned get_overscan(int file_desc, unsigned coord[4])
{
   int i=0;
   unsigned p[32];
   p[i++] = 0; // size
   p[i++] = 0x00000000; // process request

   p[i++] = 0x0004000a; // get overscan
   p[i++] = 0x00000010; // buffer size
   p[i++] = 0x00000000; // request size
   p[i++] = 0x00000000; // value buffer
   p[i++] = 0x00000000; // value buffer
   p[i++] = 0x00000000; // value buffer
   p[i++] = 0x00000000; // value buffer
   p[i++] = 0x00000000; // end tag
   p[0] = i*sizeof *p; // actual size

   mbox_property(file_desc, p);
   coord[0] = p[5];
   coord[1] = p[6];
   coord[2] = p[7];
   coord[3] = p[8];
   return 0;
}


static unsigned set_overscan(int file_desc, unsigned coord[4])
{
   int i=0;
   unsigned p[32];
   p[i++] = 0; // size
   p[i++] = 0x00000000; // process request

   p[i++] = 0x0004800a; // set overscan
   p[i++] = 0x00000010; // buffer size
   p[i++] = 0x00000010; // request size
   p[i++] = coord[0]; // value buffer
   p[i++] = coord[1]; // value buffer
   p[i++] = coord[2]; // value buffer
   p[i++] = coord[3]; // value buffer
   p[i++] = 0x00000000; // end tag
   p[0] = i*sizeof *p; // actual size

   mbox_property(file_desc, p);
   coord[0] = p[5];
   coord[1] = p[6];
   coord[2] = p[7];
   coord[3] = p[8];
   return 0;
}



/* 
 * Main - Call the ioctl functions 
 */
int main(int argc, char *argv[])
{
   int file_desc, i;
   unsigned coord[4];

   // open a char device file used for communicating with kernel mbox driver
   file_desc = open(DEVICE_FILE_NAME, 0);

   if (argc == 5) {
      for (i=0; i<4; i++)
         if (argc > 1+i)
           coord[i] = strtoul(argv[1+i], 0, 0);
      set_overscan(file_desc, coord);
   } else {
     get_overscan(file_desc, coord);
     printf("%d %d %d %d\n", coord[0], coord[1], coord[2], coord[3]);
   }

   close(file_desc);
   return 0;
}
