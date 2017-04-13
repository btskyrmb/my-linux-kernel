#include <linux/init.h>
#include <linux/module.h>

extern void fool(int);
extern int var;

static int demo2_init(void)
{
	fool(9527);

	/*操作硬件资源，申请内存， 注册中断， 映射*/
	printk("<0>"  "hello,world! var = %d\n", var);

	return 0;
}

module_init(demo2_init);

static void demo2_exit(void)
{
	printk("<0>"   "goodbye...\n");
}

module_exit(demo2_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
