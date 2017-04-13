#include <linux/init.h>
#include <linux/module.h>

#define NUM 16

static int array[NUM] = {[8] = 9527};
static int n = NUM;

module_param_array(array, int, &n, 0);

static int demo_init(void)
{
	int i = 0;

	for (; i < n; i++) {
		printk("array[%d] = %d\n", i, array[i]);
	}

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
