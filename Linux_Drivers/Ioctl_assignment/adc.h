#ifndef ADC_H
#define ADC_H

#include <linux/ioctl.h>

typedef struct
{
   int val1,val2;
}args;

#define ADC_CHANNEL _IOR('p',1,args *)


#endif
