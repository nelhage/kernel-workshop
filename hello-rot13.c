/**
 * hello-rot13 -- defines a rot13 device
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define  DEVICE_NAME "rot13"
#define  CLASS_NAME  "rot13"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nelson Elhage");
MODULE_DESCRIPTION("Kernel-accelerated rot13 driver");
MODULE_VERSION("0.1");

static int    major_number = -1;
static struct class*  rot13_class  = NULL;
static struct device* rot13_device = NULL;

static int     rot13_open(struct inode *, struct file *);
static int     rot13_release(struct inode *, struct file *);
static ssize_t rot13_read(struct file *, char *, size_t, loff_t *);
static ssize_t rot13_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations rot13_fops =
{
   .open = rot13_open,
   .read = rot13_read,
   .write = rot13_write,
   .release = rot13_release,
};

static int rot13_open(struct inode *inodep, struct file *filep) {
	return 0;
}

static ssize_t rot13_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
	return -EINVAL;
}

static ssize_t rot13_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
	return -EINVAL;
}

static int rot13_release(struct inode *inodep, struct file *filep) {
	return 0;
}

static int __init hello_rot13_init(void) {
	int error = 0;

	major_number = register_chrdev(0, DEVICE_NAME, &rot13_fops);
	if (major_number < 0) {
		printk(KERN_ALERT "Failed to register device major number\n");
		return major_number;
	}

	printk(KERN_INFO "Registered rot13 device with major number %d\n", major_number);

	rot13_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(rot13_class)) {
		printk(KERN_ALERT "Failed to register device class\n");
		error = PTR_ERR(rot13_class);
		goto out_cleanup;
	}

	rot13_device = device_create(rot13_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
	if (IS_ERR(rot13_device)) {
		printk(KERN_ALERT "Failed to create the device\n");
		error = PTR_ERR(rot13_device);
		goto out_cleanup;
	}

	return 0;

 out_cleanup:
	if (rot13_class != NULL && !IS_ERR(rot13_class)) {
		class_destroy(rot13_class);
	}

	if (major_number > 0) {
		unregister_chrdev(major_number, DEVICE_NAME);
	}
	return error;
}

static void __exit hello_rot13_exit(void){
	device_destroy(rot13_class, MKDEV(major_number,  0));
	class_unregister(rot13_class);
	class_destroy(rot13_class);
	unregister_chrdev(major_number, DEVICE_NAME);
}

module_init(hello_rot13_init);
module_exit(hello_rot13_exit);
