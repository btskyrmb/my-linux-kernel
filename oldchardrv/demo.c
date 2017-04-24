#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#define DEVNAME "hi"

static int major = 38;

//app: open --> kernel: open
static int foo_open (struct inode *inodp, struct file *filp)
{
	printk("+++++++++++ open major:%d, minor:%d  ++++++++++\n", 
			imajor(inodp), iminor(inodp));

	return 0;
}

static ssize_t 
foo_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	char *kstr = "nothing to do for you...";

	if (copy_to_user(buf, kstr, strlen(kstr))) {
		return -EINVAL;
	}

	return strlen(kstr);
}

//app: close --> kernel: release
static int foo_release (struct inode *inodp, struct file *filp)
{
	printk("+++++++++++ close major:%d, minor:%d  ++++++++++\n", 
			imajor(inodp), iminor(inodp));


	return 0;
}

static struct file_operations fops = {
	.owner	=  	THIS_MODULE,
	.open	=  	foo_open,
	.read	=	foo_read,
	.release=  	foo_release,
};

static int demo_init(void)
{
	return register_chrdev(major, DEVNAME, &fops);
}

module_init(demo_init);

static void demo_exit(void)
{
	unregister_chrdev(major, DEVNAME);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
