#include<stdio.h>
#include<stdlib.h>
#include<sys/io.h>
#include<sys/time.h>
#include<time.h>

#define PIT_GATE_PORT 0x61

int main()
{
	if(iopl(3)==-1)
	{
		printf("Could not get priviledge for IO port operation\n");
		return -1;
	}
/*	outb(0x00,RTC_INDEX);
	printf("SECOND= %d\n",inb(RTC_DATA));*/




	int i1,j,dum_val,counter,m;
	//int i=2000;
	unsigned long k;
	//printk(KERN_INFO "Inside function %s of file %s \n",__FUNCTION__,__FILE__);
	dum_val=inb(PIT_GATE_PORT);
	counter=1192;

	//outw(0x04A9,0x42);
	outb((0x01 | dum_val) ,PIT_GATE_PORT);
	clock_t start = clock();
	for(m=0;m<1000;m++)
	{
	outb(0xB0,0x43);
	outb((counter & 0xff),0x42);
	outb((counter >> 8),0x42);
	//outb(0x20,0x43);
	while((inb(0x61) & 0x20)==0)
	{
	//	i1=inb(0x42);
	//	i1=(i1 | (inb(0x42)<<8));
	//	printf("count %d\n",i1);
	//	printf("output = %d    ",(inb(0x61) & 0x20));
	//	i--;
	}
	}
	clock_t end = clock();
	double time_d = (end-start)/(double)CLOCKS_PER_SEC;
	printf("time duration = %f\n",time_d);



	if(iopl(0)==-1)
	{
		printf("Could not change previledge\n");
		return -1;
	}
	

	return 0;
}
