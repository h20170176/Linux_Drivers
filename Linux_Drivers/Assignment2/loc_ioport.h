#ifndef IOPORT_H
#define IOPORT_H

#include <linux/ioctl.h>

typedef struct
{
   int val;
}args;

#define GET_TIMER_COUNTER _IOR('p',1,args *)
#define SLEEP_MS _IOR('p',2,args *)

#endif
