#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x41086e, "module_layout" },
	{ 0x4292364c, "schedule" },
	{ 0x9ccb2622, "finish_wait" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0x33d92f9a, "prepare_to_wait" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x7d0bea0b, "per_cpu__current_task" },
	{ 0xffc7c184, "__init_waitqueue_head" },
	{ 0x7d11c268, "jiffies" },
	{ 0xb72397d5, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EB8098CBD358E56038EB809");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 7,
	.rhel_release = 572,
};
