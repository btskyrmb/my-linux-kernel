#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/delay.h>

//
//	do_gettimeofday();
//

static int demo_init(void)
{
	struct timeval ts0, ts1;
	u32 us;
	
	do_gettimeofday(&ts0);
	mdelay(1);	
	do_gettimeofday(&ts1);

#define NSPERSEC 1000000UL
	if (ts1.tv_usec >= ts0.tv_usec) {
		us = (ts1.tv_usec - ts0.tv_usec) + (ts1.tv_sec - ts0.tv_sec)*NSPERSEC;
	} else {
		us = (ts1.tv_usec + NSPERSEC - ts0.tv_usec) + (ts1.tv_sec - ts0.tv_sec -1)*NSPERSEC;
	}

	printk("ts1 - ts0 = %dus\n", us);

	return 0;
}

module_init(demo_init);

static void demo_exit(void)
{
	printk("goodbye...\n");
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
