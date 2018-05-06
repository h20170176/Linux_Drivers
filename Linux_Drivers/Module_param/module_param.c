#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include<linux/random.h>

int count=10;
int count2=1;

module_param(count,int,0755);
module_param(count2,int,0);

int module_param_init(void)
{
   int index=0;
   int rand;
   pr_info("Inside function %s \n",__FUNCTION__);
   for(index=0;index<count;index++)
   {
      //pr_info("For Index %d \n",index);
      rand=get_random_int();
      if(rand<0)
         rand=rand * -1;
      pr_info("For Index %d , the random value = %d\n",index,rand%1024);
   }
   return 0;
}

void module_param_exit(void)
{
   int index=0;
   pr_info("Inside function %s \n",__FUNCTION__);
   for(index=0;index<count2;index++)
   {
      pr_info("Leaving world Index : %d\n",index);
   }
}

module_init(module_param_init);
module_exit(module_param_exit);

MODULE_AUTHOR("SASANK");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MY PARAMETER MODULE");
