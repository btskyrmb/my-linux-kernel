#include <linux/init.h>
#include <linux/module.h>
#include <linux/export.h>

static int __init demo_init(void)
{
	int i;
#ifdef CONFIG_UPLOOKING_EMB

	for (i = 0; i < CONFIG_MOTORNUM; i++) {
		printk("April motor ... %d\n", i);
	}

#endif
	return 0;
}

module_init(demo_init);

MODULE_LICENSE("GPL");
