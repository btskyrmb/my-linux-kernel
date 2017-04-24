#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/delay.h>

// ndelay, udelay, mdelay
//
// msleep, msleep_interruptible, ssleep,

void ourdelay(int sec)
{
	unsigned long endjiffy = jiffies + sec*HZ;

	while (time_before(jiffies, endjiffy)) {
		schedule();
	}
}

void oursleep(int sec)
{
	wait_queue_head_t head;
	
	init_waitqueue_head(&head);

	wait_event_interruptible_timeout(head, 0, sec*HZ);
}

void out_jiffis_format(void)
{
	u32 sec = jiffies/HZ;

	u32 hr  = sec/3600;
	u32 min = sec%3600/60;
	    sec = sec%60;
	printk("%02dh-%02dm-%02ds\n", hr, min, sec);
}

static int demo_init(void)
{
	/*操作硬件资源，申请内存， 注册中断， 映射*/
//	printk("HZ = %d, jiffies = %lu!\n", HZ, jiffies);
	
	out_jiffis_format();
	oursleep(2);
	out_jiffis_format();

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
