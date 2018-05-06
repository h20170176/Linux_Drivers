#include<linux/init.h>
#include<linux/module.h>

__init int my_export_module_init(void)
{
   pr_info("Inside function %s\n",__FUNCTION__);
   return 0;
}

module_init(my_export_module_init);

MODULE_AUTHOR("SASANK");
MODULE_LICENSE("GPL");
