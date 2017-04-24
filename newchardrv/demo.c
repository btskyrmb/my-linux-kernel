#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define LEN 512
#define CNT 256

#define DEVNAME "kkbuf"

static int major = 0;
static int minor = 0;
static dev_t devnum;

/*抽象驱动要操作的设备及实例化此指针对象*/
struct kbuf_t {
	char *kbuf;
	size_t len;
	struct cdev cdev;
}*kbufp = NULL;

//app: open --> kernel: open
static int foo_open (struct inode *inodp, struct file *filp)
{

	filp->private_data = container_of(inodp->i_cdev, struct kbuf_t, cdev);

	printk(DEVNAME " open ok!\n");

	return 0;
}

static ssize_t 
foo_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	struct kbuf_t *pdev = filp->private_data;

	if (cnt == 0) {
		return cnt;
	}	

	if (pdev->len == 0) {
		return -EIO;
	}

	cnt = min(pdev->len, cnt);

	if (copy_to_user(buf, pdev->kbuf, cnt)) {
		return -EINVAL;
	}

	return (ssize_t)cnt;
}

static ssize_t 
foo_write (struct file *filp, const char __user *buf, size_t cnt, loff_t *fpos)
{
	struct kbuf_t *pdev = filp->private_data;
	
	if (cnt == 0) {
		return cnt;
	}	

	cnt = min((size_t)LEN, cnt);

	if (copy_from_user(pdev->kbuf, buf, cnt)) {
		return -EINVAL;
	}

	pdev->len = cnt;

	return (ssize_t)cnt;
}

//app: close --> kernel: release
static int foo_release (struct inode *inodp, struct file *filp)
{
	return 0;
}

static struct file_operations fops = {
	.owner	=  	THIS_MODULE,
	.open	=  	foo_open,
	.read	=	foo_read,
	.write	=	foo_write,
	.release=  	foo_release,
};

static int demo_init(void)
{
	int ret;
	int i;

	/*1.申请注册设备号*/
	if (major != 0) {
		devnum = MKDEV(major, minor);
		ret = register_chrdev_region(devnum, CNT, DEVNAME);
	} else {
		ret = alloc_chrdev_region(&devnum, minor, CNT, DEVNAME);	
		major = MAJOR(devnum);
	}

	if (ret < 0) {
		return ret;
	}	
	
	kbufp = kzalloc(CNT*sizeof(*kbufp), GFP_KERNEL);
	if (NULL == kbufp) {
		ret = -ENOMEM;
		goto error0;
	}

	/*2.分配及初始化、注册字符设备驱动对象*/
	for (i = 0; i < CNT; i++) {
		kbufp[i].kbuf = kzalloc(LEN, GFP_KERNEL);
		if (NULL == kbufp[i].kbuf) {
			goto error1;
		}
		cdev_init(&kbufp[i].cdev, &fops);
		ret = cdev_add(&kbufp[i].cdev, devnum+i, 1);
		if (ret < 0) {
			goto error2;
		}
	}

	printk(DEVNAME " is ok!\n");

	return 0;
error2:
	kfree(kbufp[i].kbuf);
error1:
	while (i--) {
		kfree(kbufp[i].kbuf);
		cdev_del(&kbufp[i].cdev);
	}
	kfree(kbufp);
error0:
	unregister_chrdev_region(devnum, CNT);	
	return ret;
}

module_init(demo_init);

static void demo_exit(void)
{
	int i = CNT;

	while (i--) {
		kfree(kbufp[i].kbuf);
		cdev_del(&kbufp[i].cdev);
	}
	kfree(kbufp);
	unregister_chrdev_region(devnum, CNT);	

	printk(DEVNAME " is removed!\n");
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("millet9527");
MODULE_VERSION("millet plus 12");
MODULE_DESCRIPTION("example for driver module");
