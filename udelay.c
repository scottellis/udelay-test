/*
  Checking the effective resolution of a Linux udelay on an OMAP3 kernel
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>

/* this is pin 19 on the TOBI/SUMMIT Gumstix expansion boards */
#define TOGGLE_PIN 170

struct udelay_dev {
	dev_t devt;
	struct cdev cdev;
	struct semaphore sem;
	struct class *class;
};

static struct udelay_dev test;

static ssize_t udelay_write(struct file *filp, const char __user *buff,
		size_t count, loff_t *f_pos)
{
	char str[16];
	ssize_t status;
	unsigned long usecs;

	if (count == 0)
		return 0;

	if (down_interruptible(&test.sem))
		return -ERESTARTSYS;

	if (count > 8)
		count = 8; 

	str[count] = 0;

	if (copy_from_user(str, buff, count)) {
		printk(KERN_ALERT "Error copy_from_user\n");
		status = -EFAULT;
		goto udelay_write_done;
	}

	usecs = simple_strtoul(str, NULL, 10);

	if (usecs < 1 || usecs > 500) {
		printk(KERN_ALERT "invalid delay %lu\n", usecs);
		status = -EINVAL;
		goto udelay_write_done;
	}
	
	gpio_set_value(TOGGLE_PIN, 1);
	udelay(usecs);
	gpio_set_value(TOGGLE_PIN, 0);
	udelay(usecs);
	gpio_set_value(TOGGLE_PIN, 1);
	udelay(usecs);
	gpio_set_value(TOGGLE_PIN, 0);

	status = count;
	*f_pos += count;

udelay_write_done:

	up(&test.sem);

	return status;
}

static const struct file_operations udelay_fops = {
	.owner = THIS_MODULE,
	.write = udelay_write,
};

static int __init udelay_init_cdev(void)
{
	int error;

	test.devt = MKDEV(0, 0);

	error = alloc_chrdev_region(&test.devt, 0, 1, "udelay");
	if (error < 0) {
		printk(KERN_ALERT 
			"alloc_chrdev_region() failed: error = %d \n", 
			error);
		
		return -1;
	}

	cdev_init(&test.cdev, &udelay_fops);
	test.cdev.owner = THIS_MODULE;

	error = cdev_add(&test.cdev, test.devt, 1);
	if (error) {
		printk(KERN_ALERT "cdev_add() failed: error = %d\n", error);
		unregister_chrdev_region(test.devt, 1);
		return -1;
	}	

	return 0;
}

static int __init udelay_init_class(void)
{
	test.class = class_create(THIS_MODULE, "udelay");

	if (!test.class) {
		printk(KERN_ALERT "class_create(udelay) failed\n");
		return -1;
	}

	if (!device_create(test.class, NULL, test.devt, NULL, "udelay")) {
		class_destroy(test.class);
		return -1;
	}

	return 0;
}

static int __init udelay_init_pins(void)
{
	if (gpio_request(TOGGLE_PIN, "togglepin")) {
		printk(KERN_ALERT "gpio_request failed\n");
		return -1;
	}

	if (gpio_direction_output(TOGGLE_PIN, 0)) {
		printk(KERN_ALERT "gpio_direction_output failed\n");
		gpio_free(TOGGLE_PIN);
		return -1;
	}

	return 0;
}

static int __init udelay_init(void)
{
	sema_init(&test.sem, 1);
	
	if (udelay_init_cdev())
		goto init_fail_1;

	if (udelay_init_class())
		goto init_fail_2;

	if (udelay_init_pins() < 0)
		goto init_fail_3;

	return 0;

init_fail_3:
	device_destroy(test.class, test.devt);
  	class_destroy(test.class);

init_fail_2:
	cdev_del(&test.cdev);
	unregister_chrdev_region(test.devt, 1);

init_fail_1:

	return -1;
}
module_init(udelay_init);

static void __exit udelay_exit(void)
{
	gpio_free(TOGGLE_PIN);

	device_destroy(test.class, test.devt);
  	class_destroy(test.class);

	cdev_del(&test.cdev);
	unregister_chrdev_region(test.devt, 1);
}
module_exit(udelay_exit);


MODULE_AUTHOR("Scott Ellis");
MODULE_DESCRIPTION("Test the udelay resolution on an OMAP3");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

