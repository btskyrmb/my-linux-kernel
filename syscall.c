#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define GPM4BASE 0x11000000
#define GPM4CON	 0x2e0
#define GPM4DAT  0x2e4

static char *gpm4vir = NULL;
char kbuf[4] = {0};

//long sys_ledctl(int cmd, int num)
SYSCALL_DEFINE2(ledctl, int, cmd, int, num)
{
	u8 tmp;
	
	tmp = ioread8(gpm4vir+GPM4DAT);

	if ((cmd == 0 || cmd == 1) && (num >=1 && num <= 4)) {
		if (cmd == 1) {
			tmp &= ~(1 << (num - 1));
			kbuf[num-1] = 1;
			iowrite8(tmp, gpm4vir+GPM4DAT);
		} else if(!cmd) {
			tmp |= (1 << (num - 1));
			kbuf[num-1] = 0;
			iowrite8(tmp, gpm4vir+GPM4DAT);
		}
	} else if ((cmd == 2)) {
		if (copy_to_user((char *)num, kbuf, 4)) {
			printk("copy_to_user error...\n");
			return -EINVAL;
		}	
	} else {
		return -EINVAL;
	}

	return 0;
}

static int myled_init(void)
{
	u32 tmp;

	gpm4vir = ioremap(GPM4BASE, SZ_4K);	
	if (NULL == gpm4vir) {
		return -ENOMEM;		
	}

	/*set 4gpio as output*/
	tmp = ioread32(gpm4vir+GPM4CON);
	tmp &= ~0xffff;
	tmp |= 0x1111;
	iowrite32(tmp, gpm4vir+GPM4CON);

	/*set 4led off*/
	tmp = ioread8(gpm4vir+GPM4DAT);
	tmp |= 0xf;
	iowrite8(tmp, gpm4vir+GPM4DAT);

	return 0;
}

module_init(myled_init);
