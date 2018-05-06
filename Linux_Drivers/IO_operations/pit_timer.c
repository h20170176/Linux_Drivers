#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/types.h>
#include<asm/io.h>
#include<linux/ioport.h>
#include<linux/ktime.h>
#include<asm/barrier.h>

#define PIT_CH2 0x42
#define PIT_MODE_PORT 0x43
#define PIT_MODE_IN 0xB0
#define PIT_OUT_PORT 0x61
#define PIT_GATE_PORT PIT_OUT_PORT
#define PIT_OUT_BIT 0x20
#define RD_CNTR_MD_CH2 0x20
#define PIT_GATE_IN_BIT 0x01
#define MS_RELOAD_VAL 1192

__init int my_module_init(void)
{
	u32 counter_val,gate_port_val;
	s64 actual_time;
	ktime_t start,end;

	printk(KERN_INFO "Inside function %s of file %s \n",__FUNCTION__,__FILE__);
	gate_port_val=inb(PIT_GATE_PORT);

	outb_p((PIT_GATE_IN_BIT | gate_port_val) ,PIT_GATE_PORT);
	start = ktime_get();
	outb_p(PIT_MODE_IN,PIT_MODE_PORT);
	outb_p((MS_RELOAD_VAL & 0xff),PIT_CH2);
	outb_p((MS_RELOAD_VAL >> 8),PIT_CH2);
	outb_p(RD_CNTR_MD_CH2,PIT_MODE_PORT);
	while((inb(PIT_OUT_PORT) & PIT_OUT_BIT)==0)
	{
		counter_val=inb(PIT_CH2);
		counter_val=(counter_val | (inb(PIT_CH2)<<8));
		rmb();
		printk(KERN_INFO "count %d ,",counter_val);
		printk(KERN_INFO "output = %d\n",(inb(PIT_OUT_PORT) & PIT_OUT_BIT));
		rmb();
	}
	end=ktime_get();
	actual_time = ktime_to_ns(ktime_sub(end,start));
	printk(KERN_INFO "Time taken for execution = %lld\n",(long long)actual_time);
	return 0;
}

__exit void my_module_exit(void)
{
	printk(KERN_INFO "Inside function %s of file %s \n",__FUNCTION__,__FILE__);
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_AUTHOR("SASANK");
MODULE_DESCRIPTION("IO OPERATION USING 8254 PIT");
MODULE_LICENSE("GPL");
