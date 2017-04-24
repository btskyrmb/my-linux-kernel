#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

#define NUM 186

struct game_style{
	char name[64];
	int old;
	struct list_head list;
};

struct game_head {
	int aver;
	struct list_head list;
}head = {
	.aver = 0,
};

static int demo_init(void)
{
	struct list_head *pos, *n;
	struct game_style *pga;
	int i;

	INIT_LIST_HEAD(&head.list);

	for (i = 0; i < NUM; i++) {
		pga = kzalloc(sizeof(*pga), GFP_KERNEL);
		if (NULL == pga) {
			goto error0;
		}	

		sprintf(pga->name, "QuanHuang%d", i+1);		
		pga->old = 60 + i%41;
		head.aver += pga->old;
		list_add_tail(&pga->list, &head.list);
	}

	head.aver /= NUM;

	printk("%d's old aver: %d\n", NUM, head.aver);

	list_for_each(pos, &head.list) {
		pga = container_of(pos, struct game_style, list);
		printk("+++++++++++++++++++++++++++\n");	
		printk("style name: %s\n", pga->name);	
		printk("style old : %d\n", pga->old);		
	}

	return 0;

error0:
	list_for_each_safe(pos, n, &head.list) {
		pga = container_of(pos, struct game_style, list);
		printk("%s is free...\n", pga->name);
		kfree(pga);
	}	

	return -ENOMEM;	
}

module_init(demo_init);

static void demo_exit(void)
{
	struct list_head *pos, *n;
	struct game_style *gamep;

	list_for_each_safe(pos, n, &head.list) {
		gamep = container_of(pos, struct game_style, list);
		printk("%s is free...\n", gamep->name);
		kfree(gamep);
	}	
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
