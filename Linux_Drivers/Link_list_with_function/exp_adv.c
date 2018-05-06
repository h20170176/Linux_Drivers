#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/list.h>
#include<linux/slab.h>

typedef struct link_list{
   struct list_head list;
   int val;
}llist;

int add_to_list(struct list_head *head,int val_ip)
{
   llist *dummy_entry;
   dummy_entry=kmalloc(sizeof(struct link_list *),GFP_KERNEL);
   dummy_entry->val=val_ip;

   list_add(&dummy_entry->list,head); // page - 297 of Jonathon Corbet book
//   kfree(dummy_entry);
   return 0;
}

__init int my_init(void)
{
   printk(KERN_INFO "Inside %s function of %s file\n", __FUNCTION__ ,__FILE__);

   llist *container;

   struct list_head head;
   struct list_head *list_ptr;

   INIT_LIST_HEAD(&head);

   add_to_list(&head,77);
   add_to_list(&head,87);

   list_for_each(list_ptr,&head) // list_for_each_prev : does the same work but in reverse order
   {
      container=container_of(list_ptr,struct link_list,list);
      printk(KERN_INFO "Value of link list data = %d\n",container->val);
//      kfree(container);
   }

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
