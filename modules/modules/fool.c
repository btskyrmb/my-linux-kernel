#include <linux/init.h>
#include <linux/module.h>


void fool(int n)
{
	printk("@O@ %d\n", n);
}

