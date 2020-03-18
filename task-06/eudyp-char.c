/* TASK_06 */

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Eudyptula char device");
MODULE_AUTHOR("IOMONAD");
MODULE_SUPPORTED_DEVICE("eudyptula");

#define DEVICE_NAME "eudyptula"

static int major;
static int busy;
static int done;

static char buffer[] = "DEAD BEEF\n";

static int driver_open(struct inode *ip, struct file *fp)
{
    printk(KERN_ALERT "USER OPENED DRIVER");
    return 0;
}

static ssize_t driver_read(struct file *fp, char *user,
			   size_t size, loff_t *loff)
{
    ssize_t len;

    busy = 1;
    len = sizeof(buffer) / sizeof(buffer[0]);
    printk(KERN_ALERT "USER READ DRIVER");
    if (done)
	return 0;
    if (copy_to_user(user, buffer, len))
	printk(KERN_ALERT "COPY ON USER");
    done = 1;
    return len; /* RETURN BUFFER LEN */
}

static ssize_t driver_write(struct file *fp, const char __user *user,
			    size_t size, loff_t *loff)
{
    char input[128];

    if ((strcmp(input, "IOMONAD\n")) == 0) {
	return strlen(user);
    } else {
	return -EINVAL;
    }
}

static int driver_release(struct inode *ip, struct file *fp)
{
    done = busy = 0x0;		/* RESET STATE */
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .read = driver_read,
    .write = driver_write,
    .release = driver_release
};

static __init int initialize(void)
{
    done = busy = 0x0;
    if ((major = register_chrdev(0, DEVICE_NAME, &fops)) < 0) {
	printk(KERN_ALERT "Register Chrdev returned %d", major);
    }
    return 0;
}

static __exit void destroy(void)
{
    unregister_chrdev(0, DEVICE_NAME);
    return;
}

module_init(initialize);
module_exit(destroy);
