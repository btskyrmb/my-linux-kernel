#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

/*此结构体类型代表按键类型*/
typedef struct {
	char *name;
	int irqnumber;
	int irqcnt;	
}tiny4412_key_t;

/*实例化一个包含4个按键的对象数组*/
static tiny4412_key_t keydev[] = {
	{"gebilaowang_key1", IRQ_EINT(26), 0},
	{"gebilaowang_key2", IRQ_EINT(27), 0},
	{"gebilaowang_key3", IRQ_EINT(28), 0},
	{"gebilaowang_key4", IRQ_EINT(29), 0},
};

static void do_key_bottom(unsigned long dev)
{
	tiny4412_key_t *pdev = (void *)dev;
	pdev->irqcnt++;

	printk("bottom -> %s is %s!\n", pdev->name,
			pdev->irqcnt%2?"down":"up");

	/*in_interrupt函数的返回值为真代表运行在中断上下文，否则为进程上下文*/
	if (in_interrupt()) {
		printk("%s in interrupt ...\n", __func__);
	} else {
		printk("%s in process ...\n", __func__);
	}
}

/*实例化一个用于中断下半部任务的对象*/
DECLARE_TASKLET(keybh, do_key_bottom, 0);


/*中断的上半部处理函数*/
static irqreturn_t do_key_func1(int irqnumber, void *dev)
{
	keybh.data = (unsigned long)dev;

	printk("interrupt top half...\n");

	tasklet_schedule(&keybh);

	return IRQ_HANDLED;
}

static int keydev_register(void)
{
	int i;
	int ret;

	for (i = 0; i < ARRAY_SIZE(keydev); ++i) {
		ret = request_irq(keydev[i].irqnumber, do_key_func1, 
			IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
					keydev[i].name, (keydev+i));
		if (ret < 0) {
			goto error0;
		}
	}

	return 0;
error0:
	while (i--) {
		free_irq(keydev[i].irqnumber, (keydev+i));
	}

	return ret;
}

static void keydev_unregister(void)
{
	int i = ARRAY_SIZE(keydev);

	while (i--) {
		free_irq(keydev[i].irqnumber, (keydev+i));
	}
}

static int demo_init(void)
{

	return keydev_register();
}

module_init(demo_init);

static void demo_exit(void)
{
	tasklet_kill(&keybh);

	keydev_unregister();
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
