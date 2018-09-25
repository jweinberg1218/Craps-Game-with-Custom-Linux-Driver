/*
 * Jason Weinberg
 *
 * Driver for simulating 6-sided dice rolls
 *
 * Code adapted from example kernel module created by Valerie Henson <val@nmt.edu>
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>

#include <asm/uaccess.h>

/*
 * Helper method to get random byte with specified maximum.
 */

unsigned char get_random_byte(int max) {
         unsigned char c;
         get_random_bytes(&c, 1);
         return c%max;
}

/*
 * dice_read is the function called when a process calls read() on
 * /dev/dice.  It returns a random number between 0 and 5 to the buffer passed in the
 * read() call.
 */

static ssize_t dice_read(struct file * file, char * buf, 
			  size_t count, loff_t *ppos)
{
	int len = 1;

	/*
	 * Get a random number between 0 and 5.
	 *
	 */
	char dice_val = get_random_byte(6);

	/*
	 * Besides copying the number to the user provided buffer,
	 * this function also checks that the user has permission to
	 * write to the buffer, that it is mapped, etc.
	 */
	if (copy_to_user(buf, &dice_val, len))
		return -EINVAL;

	return len;
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations dice_fops = {
	.owner		= THIS_MODULE,
	.read		= dice_read,
};

static struct miscdevice dice_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/dice.
	 */
	"dice",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&dice_fops
};

static int __init
dice_init(void)
{
	int ret;

	/*
	 * Create the "dice" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/dice device using
	 * the default rules.
	 */
	ret = misc_register(&dice_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"Hello, world!\" misc device\n");

	return ret;
}

module_init(dice_init);

static void __exit
dice_exit(void)
{
	misc_deregister(&dice_dev);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jason Weinberg <jjw91@pitt.edu>");
MODULE_DESCRIPTION("Driver for simulating 6-sided dice rolls");
MODULE_VERSION("dev");
