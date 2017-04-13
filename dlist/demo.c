#include <linux/init.h>
#include <linux/module.h>

struct game_style{
	char *name;
	int old;
	struct list_head list;
}var = {
	.name = "xo9527",
	.old  = 500,
};

static int demo_init(void)
{
	struct list_head *ptr = &var.list;
	struct game_style *pga;

	pga = container_of(ptr, struct game_style, list);

	printk("%s: %d\n", pga->name, pga->old);

	return 0;
}

module_init(demo_init);

static void demo_exit(void)
{
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
