#include<linux/init.h>
#include<linux/module.h>

void print_fn(void)
{
   pr_alert("Indide PRINT function\n");
}

EXPORT_SYMBOL(print_fn);

__init int module_export_init(void)
{
   pr_alert("Inside INIT function\n");
   return 0;
}

__exit void module_export_exit(void)
{
   pr_warning("Inside EXIT function\n");
}

module_init(module_export_init);
module_exit(module_export_exit);

MODULE_LICENSE("GPL");
