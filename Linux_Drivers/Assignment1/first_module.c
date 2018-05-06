#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/version.h>

int first_module_init(void)
{
   printk(KERN_ALERT "Hello world ! Whats up ? \n\n");
   return 0;
}
void first_module_exit(void)
{
   printk(KERN_ALERT "Leaving this world \n\n");
}
module_init(first_module_init);
module_exit(first_module_exit);

MODULE_AUTHOR("SASANK");
MODULE_DESCRIPTION("ASSIGNMENT ONE");
MODULE_LICENSE("GPL");
