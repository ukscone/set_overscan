/****************************************************************
 * overscan.c version 1.0.1 -- get/set overscan values on the fly
 * to use this standalone you will need to create the mailbox
 * device first. sudo mknod /dev/mailbox c 100 0
 * if used with the set_overscan.sh script this is not necessary
 ****************************************************************
 * returns 0 if successful, positive integer if failure
 * 1 == unable to open device, 2 == ioctl error
 * 
 ****************************************************************
 * complete rewrite from scratch of version 0.7
 * and add some semblance of error checking.
 ***************************************************************/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define	GET_OVERSCAN		0x0004000a
#define	TST_OVERSCAN		0x0004400a
#define	SET_OVERSCAN		0x0004800a
#define END_TAG			0x00000000

/* 
 * send property message to mailbox using ioctl
 */
static int mailbox_property(int file_desc, void *buf)
{
   int return_value = ioctl(file_desc, _IOWR(100, 0, char *), buf);

   /* ioctl error of some kind */ 
   if (return_value < 0) {
      close(file_desc);
      exit(2);
   }

   return return_value;
}

/*
 * Get the current values for overscan
 */
static unsigned get_overscan(int file_desc, unsigned coord[4])
{
   int i=0;
   unsigned property[32];
   property[i++] = 0;
   property[i++] = 0x00000000;

   property[i++] = GET_OVERSCAN; 
   property[i++] = 0x00000010; 
   property[i++] = 0x00000000;
   property[i++] = 0x00000000; 
   property[i++] = 0x00000000;
   property[i++] = 0x00000000;
   property[i++] = 0x00000000;
   property[i++] = END_TAG;
   property[0] = i*sizeof *property;

   mailbox_property(file_desc, property);
   coord[0] = property[5]; /* top */
   coord[1] = property[6]; /* bottom */
   coord[2] = property[7]; /* left */
   coord[3] = property[8]; /* right */
   return 0;
}

/*
 * Set overscan values. No checking that the values are sane or
 * successful. If you want to check they've been set you could
 * do a get after the set.
 */
static unsigned set_overscan(int file_desc, unsigned coord[4])
{
   int i=0;
   unsigned property[32];
   property[i++] = 0;
   property[i++] = 0x00000000;

   property[i++] = SET_OVERSCAN;
   property[i++] = 0x00000010;
   property[i++] = 0x00000010;
   property[i++] = coord[0]; /* top */
   property[i++] = coord[1]; /* bottom */
   property[i++] = coord[2]; /* left */
   property[i++] = coord[3]; /* right */
   property[i++] = END_TAG; 
   property[0] = i*sizeof *property;

   mailbox_property(file_desc, property);
   coord[0] = property[5]; /* top */
   coord[1] = property[6]; /* bottom */
   coord[2] = property[7]; /* left */
   coord[3] = property[8]; /* right */
   return 0;
}

/*
 * Start of program
 */
int main(int argc, char *argv[])
{
   int file_desc;
   unsigned coord[4];

   file_desc = open("/dev/vcio", 0);
   if (file_desc == -1)
      exit(1);
   /* order of coords on the commasnd line/return order
    * top, bottom, left, right and they are all or nothing
    * you can't set just one value.
    */
   if (argc == 5) {
      for (int i=0; i<4; i++)
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
