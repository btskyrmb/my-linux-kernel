
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/gpio.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>

#include "ioctl.h"

#define DEVNAME "leds"

static int tiny4412led[] = {
	EXYNOS4X12_GPM4(0),
	EXYNOS4X12_GPM4(1),
	EXYNOS4X12_GPM4(2),
	EXYNOS4X12_GPM4(3),
};

enum {OFF,ON};

static void led_ctl_num(int cmd, int arg)
{
	gpio_set_value(tiny4412led[arg-1], !cmd);
}

static void led_ctl_all(int cmd)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(tiny4412led); ++i) {
		gpio_set_value(tiny4412led[i], !cmd);
	}
}

static int led_state(char *buf)
{
	int i;
	char kbuf[4];

	for (i = 0; i < ARRAY_SIZE(tiny4412led); ++i) {
		kbuf[i] = !gpio_get_value(tiny4412led[i]);
	}

	if (copy_to_user(buf, kbuf, sizeof(kbuf))) {
		return -EIO;
	}

	return 0;
}


static long 
led_unlocked_ioctl (struct file *filp, unsigned int request, unsigned long arg)
{
	long ret;

	switch (request) {
	case LEDON:
		if (arg < 1 || arg > 4) {
			return -EINVAL;
		}	
		led_ctl_num(ON, arg);
		break;
	case LEDOFF:	
		if (arg < 1 || arg > 4) {
			return -EINVAL;
		}	
		led_ctl_num(OFF, arg);
		break;
	case LEDSTAT:
		ret = led_state((char *)arg);
		if (ret < 0) {
			return ret;
		}
		break;
	case LEDONALL:
		led_ctl_all(ON);
		break;
	case LEDOFFALL:
		led_ctl_all(OFF);
		break;
	default:
		led_ctl_all(OFF);
		break;	
	}

	return 0;
}

static struct file_operations fops = {
	.owner		=  	THIS_MODULE,
	.unlocked_ioctl	=	led_unlocked_ioctl,
};

static struct miscdevice misc = {
	.minor	=  MISC_DYNAMIC_MINOR,
	.name	=  DEVNAME,
	.fops	=  &fops,	
};

static int demo_init(void)
{
	int i;
	int ret;

	for (i = 0; i < ARRAY_SIZE(tiny4412led); ++i) {
		ret = gpio_request(tiny4412led[i], "led");
		if (ret < 0) {
			goto error0;
		}
		gpio_direction_output(tiny4412led[i], 1);
	}

	ret = misc_register(&misc);
	if (ret < 0) {
		goto error0;
	}

	return 0;
error0:
	while (i--) {
		gpio_free(tiny4412led[i]);
	}

	return ret;
}

module_init(demo_init);

static void demo_exit(void)
{
	misc_deregister(&misc);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
