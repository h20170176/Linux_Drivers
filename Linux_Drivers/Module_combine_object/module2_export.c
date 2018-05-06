#include<linux/init.h>
#include<linux/module.h>

__exit void my_export_module_exit(void)
{
   pr_info("Inside function %s\n",__FUNCTION__);
}

module_exit(my_export_module_exit);

MODULE_AUTHOR("SASANK");
MODULE_LICENSE("GPL");
