#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/random.h>
#include<linux/uaccess.h>
#include"adc.h"

#define MAJOR_NO 252
#define MINOR_NO 3

static dev_t devno;
static struct class *class_cr;
static struct cdev c_dev;

int fetch_adc_val(void)
{
   int adc_val2;
   adc_val2=get_random_int()%1024;
   return adc_val2;
}
ssize_t my_read (struct file *pfile, char __user *ioctl_param, size_t length, loff_t *poffset)
{
   int adc_val;
   args q;
   pr_info("File opened for Read \n");
   if(copy_from_user(&q,(args *)ioctl_param,sizeof(args)))
      return -1;
   switch(q.val1)
   {
      case 1:
         adc_val=fetch_adc_val();
         pr_info("Digital value from ADC1 = %d\n",adc_val);
         break;
      case 2:
         adc_val=fetch_adc_val();
         pr_info("Digital value from ADC2 = %d\n",adc_val);
         break;
      case 3:
         adc_val=fetch_adc_val();
         pr_info("Digital value from ADC3 = %d\n",adc_val);
         break;
      case 4:
         adc_val=fetch_adc_val();
         pr_info("Digital value from ADC4 = %d\n",adc_val);
         break;
      case 5:
         adc_val=fetch_adc_val();
         pr_info("Digital value from ADC5 = %d\n",adc_val);
         break;
      case 6:
         adc_val=fetch_adc_val();
         pr_info("Digital value from ADC6 = %d\n",adc_val);
         break;
      case 7:
         adc_val=fetch_adc_val();
         pr_info("Digital value from ADC7 = %d\n",adc_val);
         break;
      case 8:
         adc_val=fetch_adc_val();
         pr_info("Digital value from ADC8 = %d\n",adc_val);
         break;
      default:
         pr_err("Invalid channel ID\n");
         return -1;
   }
   
   q.val2=adc_val;
   if(copy_to_user((args *)ioctl_param,&q,sizeof(args)))
   return -1;

   return 0;
}
ssize_t my_write (struct file *pfile, const char __user *puser, size_t length, loff_t *poffset)
{
   pr_info("File Write() \n");
   return length;
}
int my_open (struct inode *pinode, struct file *pfile)
{
   pr_info("File Opened successfully !!! \n");
   return 0;
}
int my_release (struct inode *pinode, struct file *pfile)
{
   pr_info("File Closed Successfully !!! \n");
   return 0;
}

long my_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
   int dummy_read;
   pr_info("Inside IOCTL Function\n");
   switch (ioctl_num)
   {
      case ADC_CHANNEL:
         dummy_read =  my_read (file, (char *)ioctl_param, 100, 0);
         break;

      default:
         pr_err("Invalid command entered\n");
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

int char_driver_init(void)
{
   pr_info("Inside char_driver_init function \n");

   struct task_struct *task_cre;
   task_cre = current;

   printk(KERN_INFO "Current Process Name = %s\n",task_cre->comm);
   printk(KERN_INFO "Current State = %ld\n",task_cre->state);
   printk(KERN_INFO "Current PID = %d\n",task_cre->pid);

   devno=MKDEV(MAJOR_NO,MINOR_NO);
   
   pr_info("Major and Minor device numbers has been generated successfully.\n");
   pr_info("Major Device no. = %d, Minor Device no. = %d \n",MAJOR(devno),MINOR(devno));

   if((class_cr=class_create(THIS_MODULE,"char2_driver"))==NULL)
   {
      unregister_chrdev_region(devno,1);
      return -1;  
   }

   if(device_create(class_cr,NULL,devno,NULL,"adc_channel")==NULL)
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

   return 0;
}

void char_driver_exit(void)
{
   cdev_del(&c_dev);
   device_destroy(class_cr,devno);
   class_destroy(class_cr);
   unregister_chrdev_region(devno,1);
   pr_info("Char Driver unregistered successfully \n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_AUTHOR("SASANKA");
MODULE_DESCRIPTION("CHAR DRIVER");
MODULE_LICENSE("GPL");
