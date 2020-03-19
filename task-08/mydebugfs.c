/* TASK_08 */

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My eudyptula debugfs implementation");
MODULE_AUTHOR("IOMONAD");

#define DEBUGFS_FOLDER_NAME "eudyptula"

static struct dentry *mentry = NULL;

/* `id` file implementation */

static int id_open(struct inode *ip, struct file *fp)
{
	return 0;
}

static ssize_t id_read(struct file *fp, char __user *user, size_t size,
		       loff_t *loff)
{
	return 0;
}

static ssize_t id_write(struct file *fp, const char __user *buffer, size_t size,
			loff_t *loff)
{
	return 0;
}

static struct file_operations id_fops = { .open = id_open,
					  .read = id_read,
					  .write = id_write };

/* `jiffries` file implementation */

static int jiffries_open(struct inode *ip, struct file *fp)
{
	return 0;
}

static ssize_t jiffries_read(struct file *fp, char __user *user, size_t size,
			     loff_t *loff)
{
	return 0;
}

static ssize_t jiffries_write(struct file *fp, const char __user *buffer,
			      size_t size, loff_t *loff)
{
	return 0;
}

static struct file_operations jiffries_fops = { .open = jiffries_open,
						.read = jiffries_read,
						.write = jiffries_write };

/* `foo` file implementation */

static int foo_open(struct inode *ip, struct file *fp)
{
	return 0;
}

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

static struct file_operations foo_fops = { .open = foo_open,
					   .read = foo_read,
					   .write = foo_write };

/** MODULE CORE */
static __init int initialize(void)
{
	struct dentry *id, *jiffries, *foo;

	if ((mentry = debugfs_create_dir(DEBUGFS_FOLDER_NAME, NULL)) == NULL) {
		printk(KERN_ALERT "Error while creating debugfs root folder");
		return 1;
	}
	if ((id = debugfs_create_file("id", 777, mentry, NULL, &id_fops)) ==
	    NULL)
		return 1;
	if ((jiffries = debugfs_create_file("jiffries", 777, mentry, NULL,
					    &jiffries_fops)) == NULL)
		return 1;
	if ((foo = debugfs_create_file("foo", 777, mentry, NULL, &foo_fops)) ==
	    NULL)
		return 1;
	return 0;
}

static __exit void destroy(void)
{
	return debugfs_remove_recursive(mentry);
}

module_init(initialize);
module_exit(destroy);
