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

/*实例化一个用于中断下半部任务的对象*/
static struct keywork_t{
	tiny4412_key_t *keyp;
	struct work_struct work;
}workkey;


/*中断的下半部*/
static void do_key_work (struct work_struct *work)
{
	struct keywork_t *p = 
		container_of(work, struct keywork_t, work);

	tiny4412_key_t *pdev = p->keyp;
	pdev->irqcnt++;

	printk("bottom -> %s is %s!\n", pdev->name,
			pdev->irqcnt%2?"down":"up");

	if (in_interrupt()) {
		printk("%s in interrupt ...\n", __func__);
	} else {
		printk("%s in process ...\n", __func__);
	}
}
	
/*中断的上半部处理函数*/
static irqreturn_t do_key_func1(int irqnumber, void *dev)
{
	printk("interrupt top half...\n");

	workkey.keyp = dev;

	/*将下半部任务对象交给调度器调度*/
	schedule_work(&workkey.work);

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
	INIT_WORK(&workkey.work, do_key_work);

	return keydev_register();
}

module_init(demo_init);

static void demo_exit(void)
{
	flush_work(&workkey.work);

	keydev_unregister();
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
