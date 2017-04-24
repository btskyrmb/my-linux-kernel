#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

typedef struct {
	char *name;
	int irqnumber;
	int irqcnt;	
}tiny4412_key_t;

static tiny4412_key_t keydev[] = {
	{"gebilaowang_key1", IRQ_EINT(26), 0},
	{"gebilaowang_key2", IRQ_EINT(27), 0},
	{"gebilaowang_key3", IRQ_EINT(28), 0},
	{"gebilaowang_key4", IRQ_EINT(29), 0},
};

static irqreturn_t do_key_func1(int irqnumber, void *dev)
{
	tiny4412_key_t *pdev = dev;
	pdev->irqcnt++;

	printk("%s is %s!\n", pdev->name,
			pdev->irqcnt%2?"down":"up");
	return IRQ_HANDLED;
}

static irqreturn_t do_key_func2(int irqnumber, void *dev)
{
	printk("%s %d!\n", "++++++++++++++++", (int)dev);

	return IRQ_HANDLED;
}
static int keydev_register(void)
{
	int i;
	int ret;

	for (i = 0; i < ARRAY_SIZE(keydev); ++i) {
		ret = request_irq(keydev[i].irqnumber, do_key_func1, 
			IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING | IRQF_SHARED,
					keydev[i].name, (keydev+i));
		if (ret < 0) {
			goto error0;
		}
		ret = request_irq(keydev[i].irqnumber, do_key_func2, 
			IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING | IRQF_SHARED,
					keydev[i].name, (void *)(1+i));
		if (ret < 0) {
			free_irq(keydev[i].irqnumber, (keydev+i));
			goto error0;
		}
	}

	return 0;
error0:
	while (i--) {
		free_irq(keydev[i].irqnumber, (keydev+i));
		free_irq(keydev[i].irqnumber, (void *)(i+1));
	}

	return ret;
}

static void keydev_unregister(void)
{
	int i = ARRAY_SIZE(keydev);

	while (i--) {
		free_irq(keydev[i].irqnumber, (keydev+i));
		free_irq(keydev[i].irqnumber, (void *)(i+1));
	}
}

static int demo_init(void)
{

	return keydev_register();
}

module_init(demo_init);

static void demo_exit(void)
{
	keydev_unregister();
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
