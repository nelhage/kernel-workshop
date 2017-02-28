/**
 * hello_rc -- a Hello, World kernel module for the Recurse Center
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nelson Elhage");
MODULE_DESCRIPTION("Say Hello from RC");
MODULE_VERSION("0.1");


static int __init hello_rc_init(void){
	printk(KERN_INFO "Hello, Recurse Center!\n");
	return 0;
}

static void __exit hello_rc_exit(void){
	printk(KERN_INFO "Never Graduate!\n");
}


module_init(hello_rc_init);
module_exit(hello_rc_exit);
