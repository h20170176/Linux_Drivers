#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/types.h>
#include<linux/atomic.h>


__init int my_init(void)
{
   printk(KERN_INFO "Inside %s function of %s file\n", __FUNCTION__ ,__FILE__);

   atomic_t v = ATOMIC_INIT(5);
   printk(KERN_INFO "Value of V after ATOMIC_INIT is = %d\n",v.counter);

   atomic_set(&v,7);
   printk(KERN_INFO "Value of V after atomic_set is = %d\n",atomic_read(&v));

   atomic_add(3,&v);
   printk(KERN_INFO "Value of V after atomic_add is = %d\n",atomic_read(&v));

   return 0;
}

__exit void my_exit(void)
{
   printk(KERN_INFO "Inside %s function of %s file\n", __FUNCTION__ ,__FILE__);

}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("SASANK");
MODULE_DESCRIPTION("LINK LIST EXPERIMENTS");
MODULE_LICENSE("GPL");
