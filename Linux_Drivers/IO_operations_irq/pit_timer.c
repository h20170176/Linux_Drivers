#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/types.h>
#include<asm/io.h>
#include<linux/ioport.h>
#include<linux/ktime.h>
#include<asm/barrier.h>
#include<linux/interrupt.h>
#include<linux/sched.h>
#include<linux/irqreturn.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/delay.h>

#define MAJOR_NO 252
#define MINOR_NO 3

static dev_t devno;
static struct class *class_cr;
static struct cdev c_dev;

void do_tasklet (unsigned long);

DECLARE_TASKLET(my_tasklet,do_tasklet,0);

static irqreturn_t irq_caught(unsigned int irq,void *dev_id)
{
	printk(KERN_INFO "Inside function %s\n",__FUNCTION__);
	tasklet_schedule(&my_tasklet);
	return IRQ_HANDLED;
}

void do_tasklet(unsigned long unused)
{
	printk(KERN_INFO "Inside function %s\n",__FUNCTION__);
	int i;
	for(i=0;i<10;i++)
	{
		printk(KERN_INFO "Value of i = %d\n",i);
		mdelay(200);
	}
}

ssize_t my_read (struct file *pfile, char __user *ioctl_param, size_t length, loff_t *poffset)
{
      printk(KERN_INFO "File opened for Read \n");
      asm("int $0x3B");
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


static struct file_operations fops = {
   .owner          = THIS_MODULE,
   .open           = my_open,
   .release        = my_release,
   .read           = my_read,
   .write          = my_write
};


__init int my_module_init(void)
{
   u32 ret;
   printk(KERN_INFO "Inside %s \n",__FUNCTION__);

   devno=MKDEV(MAJOR_NO,MINOR_NO);

   printk(KERN_INFO "Major and Minor device numbers has been generated successfully.\n");
   printk(KERN_INFO "Major Device no. = %d, Minor Device no. = %d \n",MAJOR(devno),MINOR(devno));

   if((class_cr=class_create(THIS_MODULE,"char2_driver"))==NULL)
   {
        unregister_chrdev_region(devno,1);
        return -1;
   }

   if(device_create(class_cr,NULL,devno,NULL,"timerxxx")==NULL)
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


	
   ret = request_irq(11,(irq_handler_t)irq_caught,IRQF_SHARED,"timerxxx",(void *)(irq_caught));
   if(ret)
   {
	pr_err("request irq failed\n");
        return -1;
   }
		
   return 0;
}

__exit void my_module_exit(void)
{
	free_irq(11,(void *)(irq_caught));
	cdev_del(&c_dev);
	   device_destroy(class_cr,devno);
	      class_destroy(class_cr);

	printk(KERN_INFO "Inside function %s of file %s \n",__FUNCTION__,__FILE__);
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_AUTHOR("SASANK");
MODULE_DESCRIPTION("IO OPERATION USING 8254 PIT USING IRQ");
MODULE_LICENSE("GPL");
