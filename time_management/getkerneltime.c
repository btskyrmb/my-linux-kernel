#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/delay.h>

//
//	do_gettimeofday();
//
//	current_kernel_time();
//
// 

static int demo_init(void)
{
	struct timespec ts0, ts1;
	u32 ns;

	ts0 = current_kernel_time();
	mdelay(1);	
	ts1 = current_kernel_time();

#define NSPERSEC 1000000000UL
	if (ts1.tv_nsec >= ts0.tv_nsec) {
		ns = (ts1.tv_nsec - ts0.tv_nsec) + (ts1.tv_sec - ts0.tv_sec)*NSPERSEC;
	} else {
		ns = (ts1.tv_nsec + NSPERSEC - ts0.tv_nsec) + (ts1.tv_sec - ts0.tv_sec -1)*NSPERSEC;
	}

	printk("ts1 - ts0 = %dns\n", ns);

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
