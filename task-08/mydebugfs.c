/* TASK_08 */

#include <linux/fs.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My eudyptula debugfs implementation");
MODULE_AUTHOR("IOMONAD");

#define DEBUGFS_FOLDER_NAME "eudyptula"

static struct dentry *mentry = NULL;

/* `id` file implementation */

static ssize_t id_read(struct file *fp, char __user *user, size_t size,
		       loff_t *loff)
{
	ssize_t len;
	static char local_buffer[] = "iomonad";

	len = sizeof(local_buffer) / sizeof(local_buffer[0]);
	if (copy_to_user(user, local_buffer, len)) {
		printk(KERN_ALERT "COPY ON USER");
		return 0;
	}
	return (len);
}

static ssize_t id_write(struct file *fp, const char __user *buffer, size_t size,
			loff_t *loff)
{
	char input[128];

	copy_from_user(input, buffer, 128);
	printk(KERN_INFO "Got buffer: \"%s\"", input);
	if ((strncmp(input, "iomonad", 7)) == 0) {
		return strlen(buffer);
	} else {
		return -EINVAL; /* write error: invalid argument */
	}
}

static struct file_operations id_fops = { .read = id_read, .write = id_write };

/* `jiffies` file implementation */

static ssize_t jiffies_read(struct file *fp, char __user *user, size_t size,
			    loff_t *loff)
{
	return 0;
}

static struct file_operations jiffies_fops = { .read = jiffies_read };

/* `foo` file implementation */

static ssize_t foo_read(struct file *fp, char __user *user, size_t size,
			loff_t *loff)
{
	return 0;
}

static ssize_t foo_write(struct file *fp, const char __user *buffer,
			 size_t size, loff_t *loff)
{
	return 0;
}

static struct file_operations foo_fops = { .read = foo_read,
					   .write = foo_write };

/** MODULE CORE */

static __init int initialize(void)
{
	struct dentry *id, *jiffies, *foo;

	if ((mentry = debugfs_create_dir(DEBUGFS_FOLDER_NAME, NULL)) == NULL) {
		printk(KERN_ALERT "Error while creating debugfs root folder");
		return 1;
	}
	if ((id = debugfs_create_file("id", 666, mentry, NULL, &id_fops)) ==
	    NULL) {
		return 1;
	}
	if ((jiffies = debugfs_create_file("jiffies", 444, mentry, NULL,
					   &jiffies_fops)) == NULL) {
		return 1;
	}
	if ((foo = debugfs_create_file("foo", 777, mentry, NULL, &foo_fops)) ==
	    NULL) {
		return 1;
	}
	return 0;
}

static __exit void destroy(void)
{
	return debugfs_remove_recursive(mentry);
}

module_init(initialize);
module_exit(destroy);
