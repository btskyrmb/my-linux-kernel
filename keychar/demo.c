#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/sched.h>

#define DEVNAME "keys"

/*此结构体类型代表按键类型*/
typedef struct {
	int keynum;
	char *name;
	int irqnumber;
	int irqcnt;	
}tiny4412_key_t;

/*实例化一个包含4个按键的对象数组*/
static tiny4412_key_t keydev[] = {
	{0, "gebilaowang_key1", IRQ_EINT(26), 0},
	{1, "gebilaowang_key2", IRQ_EINT(27), 0},
	{2, "gebilaowang_key3", IRQ_EINT(28), 0},
	{3, "gebilaowang_key4", IRQ_EINT(29), 0},
};


/*为了实现进程的阻塞定义如下对象*/
static wait_queue_head_t wait;

/*为了按键的防抖动通过定时器实现*/
static struct timer_list timer;

/*给阻塞的函数使用，用于表示是否有数据可读*/
static int flag_dnup = 0;

/*用于保存4个按键的状态*/
static char kbuf[4] = {0};

static void do_timer_debounce(unsigned long dev)
{
	int index;

	tiny4412_key_t *pdev = (void *)dev;
	pdev->irqcnt++;

	index = pdev->keynum;

	if((pdev->irqcnt%2) != kbuf[index]) {
		kbuf[index] = pdev->irqcnt%2;

		flag_dnup = 1;
		wake_up_interruptible(&wait);
	}				
}

/*中断的下半部*/
static void do_key_bottom(unsigned long dev)
{
	timer.data = dev;
	mod_timer(&timer, jiffies + msecs_to_jiffies(30));
}

/*实例化一个用于中断下半部任务的对象*/
DECLARE_TASKLET(keybh, do_key_bottom, 0);


/*中断的上半部处理函数*/
static irqreturn_t do_key_func1(int irqnumber, void *dev)
{
	keybh.data = (unsigned long)dev;

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

static ssize_t 
key_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	if (cnt != 4) {
		return -EINVAL;
	}	

	wait_event_interruptible(wait, flag_dnup);	

	if (copy_to_user(buf, kbuf, 4)) {
		return -EIO;
	}

	flag_dnup = 0;

	return cnt;
}

static unsigned int 
key_poll (struct file *filp, struct poll_table_struct *wt)
{
	unsigned int mask = 0;

	poll_wait(filp, &wait, wt);	

	if (flag_dnup) {
		mask |= POLLIN | POLLRDNORM;
	}

	return mask;
}

static struct file_operations fops = {
	.owner	= THIS_MODULE,
	.read	= key_read,
	.poll	= key_poll,
};

static struct miscdevice misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &fops,
	.name	= DEVNAME,
};


/*驱动模块的入口*/
static int demo_init(void)
{
	int ret;
	
	ret = misc_register(&misc);
	if (ret < 0) {
		return ret;
	}

	if ((ret = keydev_register()) < 0) {
		goto error0;
	}

	init_waitqueue_head(&wait);

	setup_timer(&timer, do_timer_debounce, 0);

	return 0;
error0:
	misc_deregister(&misc);

	return ret;	
}

module_init(demo_init);

static void demo_exit(void)
{
	tasklet_kill(&keybh);
	keydev_unregister();
	del_timer_sync(&timer);
	misc_deregister(&misc);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
