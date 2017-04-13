#include <linux/init.h>
#include <linux/module.h>

static int demo_init(void)
{
	/*操作硬件资源，申请内存， 注册中断， 映射*/
	printk("hello,world!\n");

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