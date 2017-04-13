#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/dma-mapping.h>

static char *viradd;
static dma_addr_t phyadd;

static int demo_init(void)
{
	viradd = dma_alloc_coherent(NULL, SZ_256, &phyadd, GFP_KERNEL);
	if (NULL == viradd) {
		return -ENOMEM;
	}

	strcpy(viradd, "hello, boys...");
	printk("%s\n", viradd);

	printk("viradd = %p, phyadd = %#x\n",
			viradd, phyadd);
	
	return 0;
}

module_init(demo_init);

static void demo_exit(void)
{
	dma_free_coherent(NULL, SZ_256, viradd, phyadd);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
