#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include"loc_ioport.h"
#include<asm/io.h>
#include<linux/ioport.h>
#include<linux/ktime.h>
#include<asm/barrier.h>

#define PIT_CH2 0x42
#define PIT_MODE_PORT 0x43
#define PIT_MODE_IN 0xB0
#define PIT_OUT_PORT 0x61
#define PIT_GATE_PORT PIT_OUT_PORT
#define PIT_OUT_BIT 0x20
#define RD_CNTR_MD_CH2 0x20
#define PIT_GATE_IN_BIT 0x01
#define MS_RELOAD_VAL 1192

#define MAJOR_NO 252
#define MINOR_NO 3

static dev_t devno;
static struct class *class_cr;
static struct cdev c_dev;

/******************************************************************************************************
*Function Name - init_loc_timer
*
*Author - Sasanka Sekhar Dash
*
*Description - This function initializes the 8254 with appropriate channel(here ch2 has been used),
*mode (here mode 0 has been used) and access.
*In mode 0 , the counter takes a load value and decrements the value to 0 and then starts the counter
*from the max value (here 65535) and continue decrement.
*Once the counter value goes to zero from load value, the output pin (bit #5 of port 0x61) changes to
*one from zero and remain at one till the time when new load counter value is loaded.
*
*Here the load value is 1192(to achieve 1ms delay) , as in mode 0 , the counter decrements "n+1" times.
*
*******************************************************************************************************/

int init_loc_timer(void)
{
	u32 gate_port_val;
	printk(KERN_INFO "Inside function %s\n",__FUNCTION__);
	gate_port_val=inb(PIT_GATE_PORT);

        outb_p((PIT_GATE_IN_BIT | gate_port_val) ,PIT_GATE_PORT);
	outb_p(PIT_MODE_IN,PIT_MODE_PORT);
	outb_p((MS_RELOAD_VAL & 0xff),PIT_CH2);
	outb_p((MS_RELOAD_VAL >> 8),PIT_CH2);

	return 0;
}
/******************************************************************************************************
*Function Name - delay_ms
*
*Author - Sasanka Sekhar Dash
*
*Description - This function introduces 1millisecond delay in kernel space. 
*
*******************************************************************************************************/

int delay_ms(int del_ms)
{
        u32 counter_val,gate_port_val,dummy_del;
        s64 actual_time;
        ktime_t start,end;

        printk(KERN_INFO "Inside function %s\n",__FUNCTION__);
        gate_port_val=inb(PIT_GATE_PORT);

        outb_p((PIT_GATE_IN_BIT | gate_port_val) ,PIT_GATE_PORT);
        start = ktime_get();

	for(dummy_del=0;dummy_del<del_ms;dummy_del++)
	{
        	outb_p(PIT_MODE_IN,PIT_MODE_PORT);
        	outb_p((MS_RELOAD_VAL & 0xff),PIT_CH2);
        	outb_p((MS_RELOAD_VAL >> 8),PIT_CH2);
        	outb_p(RD_CNTR_MD_CH2,PIT_MODE_PORT);
        	while((inb(PIT_OUT_PORT) & PIT_OUT_BIT)==0)
        	{
                	counter_val=inb(PIT_CH2);
                	counter_val=(counter_val | (inb(PIT_CH2)<<8));
                	rmb();
                //	printk(KERN_INFO "count %d ,",counter_val);
                //	printk(KERN_INFO "output = %d\n",(inb(PIT_OUT_PORT) & PIT_OUT_BIT));
                //	rmb();
        	}
	}
        end=ktime_get();
        actual_time = ktime_to_ms(ktime_sub(end,start));
        printk(KERN_INFO "Time taken for execution in milli seconds = %u\n",(unsigned int)actual_time);
        return 0;
}
/******************************************************************************************************
*Function Name - sleep_ms_kernel_space
*
*Author - Sasanka Sekhar Dash
*
*Description - This function is a wrapper function of "delay_ms" and used to introduce sleep functionality in kernel space
*
*******************************************************************************************************/

void sleep_ms_kernel_space(int slp_ms)
{
	printk(KERN_INFO "Inside function %s\n",__FUNCTION__);
	delay_ms(slp_ms);
}
/******************************************************************************************************
*Function Name - sleep_ms_user_space
*
*Author - Sasanka Sekhar Dash
*
*Description - This function is a wrapper function of "sleep_ms_kernel_space" and used to introduce sleep functionality in userspace
*
*******************************************************************************************************/

void sleep_ms_user_space(char * ioctl_param)
{
   args q;
   copy_from_user(&q,(args *)ioctl_param,sizeof(args));
   sleep_ms_kernel_space(q.val);
}
/******************************************************************************************************
*Function Name - get_timer_counter
*
*Author - Sasanka Sekhar Dash
*
*Description - This function is used to fetch the counter value at any instant 
*
*******************************************************************************************************/

int get_timer_counter(void)
{
	u32 counter_val;

	counter_val=inb(PIT_CH2);
	counter_val=(counter_val | (inb(PIT_CH2)<<8));
	rmb();

	return counter_val;
}

ssize_t my_read (struct file *pfile, char __user *ioctl_param, size_t length, loff_t *poffset)
{
   args q;
   printk(KERN_INFO "File opened for Read \n");
   if(copy_from_user(&q,(args *)ioctl_param,sizeof(args)))
      return -1;
   
   q.val=get_timer_counter();
   
   if(copy_to_user((args *)ioctl_param,&q,sizeof(args)))
   return -1;

   return 0;
}
ssize_t my_write (struct file *pfile, const char __user *puser, size_t length, loff_t *poffset)
{
   printk(KERN_INFO "File Write() \n");
   return length;
}
int my_open (struct inode *pinode, struct file *pfile)
{
   printk(KERN_INFO "File Opened successfully !!! \n");
   return 0;
}
int my_release (struct inode *pinode, struct file *pfile)
{
   printk(KERN_INFO "File Closed Successfully !!! \n");
   return 0;
}

long my_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
   int dummy_read;
   printk(KERN_INFO "Inside IOCTL Function\n");
   switch (ioctl_num)
   {
      case GET_TIMER_COUNTER:
         dummy_read =  my_read (file, (char *)ioctl_param, 100, 0);
         break;

      case SLEEP_MS:
         sleep_ms_user_space ((char *)ioctl_param);
         break;

      default:
         printk(KERN_ERR "Invalid command entered\n");
         return -1;
   }
   return 0;
}

static struct file_operations fops = {
   .owner          = THIS_MODULE,
   .open           = my_open,
   .release        = my_release,
   .read           = my_read,
   .write          = my_write,
   .unlocked_ioctl = my_ioctl
};

__init int char_driver_init(void)
{
   printk(KERN_INFO "Inside char_driver_init function \n");

   devno=MKDEV(MAJOR_NO,MINOR_NO);
   
   printk(KERN_INFO "Major and Minor device numbers has been generated successfully.\n");
   printk(KERN_INFO "Major Device no. = %d, Minor Device no. = %d \n",MAJOR(devno),MINOR(devno));

   if((class_cr=class_create(THIS_MODULE,"char2_driver"))==NULL)
   {
      unregister_chrdev_region(devno,1);
      return -1;  
   }

   if(device_create(class_cr,NULL,devno,NULL,"timer0")==NULL)
   {
      class_destroy(class_cr);
      unregister_chrdev_region(devno,1);
      return -1;
   }

   cdev_init(&c_dev,&fops);
   if(cdev_add(&c_dev,devno,1) == -1)
   {
      device_destroy(class_cr,devno);
      class_destroy(class_cr);
      unregister_chrdev_region(devno,1);
      return -1;
   }
   //Intializing timer.
   init_loc_timer();

   //Below function can be used to introduce delay.
   //delay_ms_kernel_space(2000);

   return 0;
}

__exit void char_driver_exit(void)
{
   cdev_del(&c_dev);
   device_destroy(class_cr,devno);
   class_destroy(class_cr);
   unregister_chrdev_region(devno,1);
   printk(KERN_INFO "Char Driver unregistered successfully \n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_AUTHOR("SASANKA SEKHAR DASH");
MODULE_DESCRIPTION("CHAR DRIVER FOR TIMER APPLICATION");
MODULE_LICENSE("GPL");
