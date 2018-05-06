#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include"loc_ioport.h"
#include<time.h>


args q;
int file,value;
char *device="/dev/timer0";

/******************************************************************************************************
*Function Name - get_counter
*
*Author - Sasanka Sekhar Dash
*
*Description - This function is used to fetch the counter value at any instant 
*
*******************************************************************************************************/

int get_counter(void)
{
   file=open(device,0);
   if(file<0)
   {
      printf("File did not open from user space\n");
      return -1;
   }
   printf("Device file opened successfully\n");

   if(ioctl(file,GET_TIMER_COUNTER,&q)<0)
   {
      printf("IOCTL failed in kernel space \n");
      return -1;
   }

   printf("Counter value from PIT = %d\n",q.val);

   if(close(file)<0)
      printf("Unable to close device file\n");
   else
      printf("Device file closed successfully \n");

   return 0;
}
/******************************************************************************************************
*Function Name - pit_delay
*
*Author - Sasanka Sekhar Dash
*
*Description - This function is used to introduce delay in milliseconds through kernel sleep function 
*
*******************************************************************************************************/

int pit_delay(int delay_ms)
{
   q.val=delay_ms;
   file=open(device,0);
   if(file<0)
   {
      printf("File did not open from user space\n");
      return -1;
   }
   printf("Device file opened successfully\n");

   if(ioctl(file,SLEEP_MS,&q)<0)
   {
      printf("IOCTL failed in kernel space \n");
      return -1;
   }

   if(close(file)<0)
      printf("Unable to close device file\n");
   else
      printf("Device file closed successfully \n");

}

int main()
{
   printf("Inside Main function\n");
   get_counter();

   //The below commented code can be used to introduce delay from kernel space and can
   //also be used to see the accuracy of delay function designed in kernel space.

   /*printf("\n");
   clock_t start = clock();
   pit_delay(15000);
   clock_t end = clock();
   double time_d = (end-start)/(double)CLOCKS_PER_SEC;
   printf("time duration = %f\n",time_d);*/

   return 0;
}
