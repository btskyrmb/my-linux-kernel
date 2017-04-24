#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timer.h>

#define SECS 5

static struct timer_list timer;

static void do_timer_hehe(unsigned long arg)
{
	printk("<0>%s...\n", (char *)arg);
	mod_timer(&timer, jiffies + HZ*SECS);
}

static int demo_init(void)
{
	setup_timer(&timer, do_timer_hehe, (unsigned long)"hehe");
	timer.expires = jiffies + HZ*SECS;
	add_timer(&timer);
	
	return 0;
}

module_init(demo_init);

static void demo_exit(void)
{
	del_timer_sync(&timer);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
