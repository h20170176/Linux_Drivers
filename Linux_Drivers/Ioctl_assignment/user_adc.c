#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include"adc.h"

int main()
{
   args q;
   int file,value;
   char *device="/dev/adc_channel";
   char *buffer=(char *)calloc(100,sizeof(char));
   file=open(device,0);
   if(file<0)
   {
      printf("File did not open from user space\n");
      return -1;
   }
   printf("Device file opened successfully\n");

   printf("Enter ADC channel ID\n");
   scanf("%d",&value);
   q.val1=value;
   if(ioctl(file,ADC_CHANNEL,&q)<0)
   {
      printf("IOCTL failed in kernel space \n");
      return -1;
   }

   printf("ADC value from Device%d is = %d\n",value,q.val2);

   if(close(file)<0)
      printf("Unable to close device file\n");
   else
      printf("Device file closed successfully \n");
   return 0;
}
