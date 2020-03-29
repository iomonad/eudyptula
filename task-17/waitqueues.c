/* TASK_17 */

#include <linux/fs.h>
#include <linux/err.h>
#include <linux/wait.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Wait queues & Threads");
MODULE_AUTHOR("IOMONAD");

#define MISCDEV_NAME "eudyptula"
#define MAINTHREAD_N MISCDEV_NAME

DECLARE_WAIT_QUEUE_HEAD(wee_wait);

static struct task_struct *mythread = NULL;

static int thread_runner(void *data)
{
	for (;;) {
		if (wait_event_interruptible(wee_wait, kthread_should_stop()))
			return -ERESTART;
		if (kthread_should_stop())
			break;
	}
	return 0;
}

static ssize_t driver_write(struct file *fp, const char __user *buff,
			    size_t size, loff_t *lf)
{
	char input[0x8];

	copy_from_user(input, buff, 0x8);
	printk(KERN_INFO "Got buffer: %s", input);
	if (size == 8 && (strncmp(input, "iomonad", 7)) == 0) {
		return strlen(buff);
	} else {
		return -EINVAL; /* write error: invalid argument */
	}
}

static struct file_operations mfops = {
	.owner = THIS_MODULE,
	.write = driver_write,
};

static struct miscdevice miscdev = { .minor = MISC_DYNAMIC_MINOR,
				     .name = MISCDEV_NAME,
				     .fops = &mfops,
				     .mode = 0222 };

static __init int initialize(void)
{
	int status;

	if ((status = misc_register(&miscdev)))
		return status;
	mythread = kthread_run(thread_runner, NULL, MAINTHREAD_N);
	if (mythread == ERR_PTR(-ENOMEM))
		return -ENOMEM;
	return 0;
}

static __exit void destroy(void)
{
	kthread_stop(mythread);
	return misc_deregister(&miscdev);
}

module_init(initialize);
module_exit(destroy);
