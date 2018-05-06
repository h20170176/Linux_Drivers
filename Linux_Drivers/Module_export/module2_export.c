#include<linux/init.h>
#include<linux/module.h>

void print_fn(void);

__init int module_export_init(void)
{
   pr_alert("Inside INIT2 function\n");
   print_fn();
   return 0;
}

__exit void module_export_exit(void)
{
   pr_warning("Inside EXIT2 function\n");
}

module_init(module_export_init);
module_exit(module_export_exit);

MODULE_LICENSE("GPL");
