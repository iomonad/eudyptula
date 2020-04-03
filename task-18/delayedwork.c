/* TASK_17 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Delayed work");
MODULE_AUTHOR("IOMONAD");

#define NAME_LEN 20

#define MISCDEV_NAME "eudyptula"
#define MAINTHREAD_N MISCDEV_NAME

static struct task_struct *mythread = NULL;

static DEFINE_MUTEX(tmutex);
DECLARE_WAIT_QUEUE_HEAD(wee_wait);

/* IDENTITY STUFF */

struct identity {
	char name[NAME_LEN];
	int id;
	bool busy;
	struct list_head lst;
};

static LIST_HEAD(identity_list);

/* SEARCH */
struct identity *identity_find(int id)
{
	struct identity *it = NULL;

	mutex_lock_interruptible(&tmutex);
	list_for_each_entry (it, &identity_list, lst) {
		if (it->id == id) {
			return it;
		}
	}
	mutex_unlock(&tmutex);
	return NULL;
}

/* CREATE */
int identity_create(char *name, int id)
{
	struct identity *new = NULL;

	if (identity_find(id) != NULL) {
		/* ID EXISTS */
		return 1;
	}
	if (!(new = kmalloc(sizeof(*new), GFP_KERNEL))) {
		/* ALLOC FAILED */
		return 2;
	}
	mutex_lock_interruptible(&tmutex);
	memset(new->name, 0x00, NAME_LEN);
	strncpy(new->name, name, NAME_LEN - 1);
	new->id = id;
	new->busy = false;
	list_add(&(new->lst), &identity_list);
	mutex_unlock(&tmutex);
	pr_info("identity %d: with %s name was created", id, name);
	return 0;
}

/* DELETE */
void identity_destroy(int id)
{
	struct identity *temp = NULL;

	if (!(temp = identity_find(id))) {
		return;
	}
	mutex_lock_interruptible(&tmutex);
	list_del(&(temp->lst));
	kfree(temp);
	mutex_unlock(&tmutex);
	return;
}

struct identity *identity_get(void)
{
	struct identity *it = NULL;

	if (list_empty(&identity_list))
		return NULL;
	mutex_lock_interruptible(&tmutex);
	it = list_entry(identity_list.next, struct identity, lst);
	list_del(&it->lst);
	mutex_unlock(&tmutex);
	return it;
}

/* WAITLIST STUFF */

static int thread_runner(void *data)
{
	struct identity *it = NULL;

	for (;;) {
		if (wait_event_interruptible(wee_wait, 1))
			return -ERESTARTSYS;
		if (kthread_should_stop())
			break;
		if ((it = identity_get()) != NULL) {
			/* Make service poll relaxed */
			msleep_interruptible(5000);
			pr_info("identity %d: %s polled in kthread", it->id,
				it->name);
			kfree(it);
		}
	}
	return 0;
}

static ssize_t driver_write(struct file *fp, const char __user *buff,
			    size_t size, loff_t *lf)
{
	char input[20];
	static int local_id = 1;

	memset(input, 0x0, 20);
	if (copy_from_user(input, buff, 19)) {
		return -EINVAL;
	}

	printk(KERN_INFO "Got buffer: %s", input);
	if (identity_create(input, local_id)) {
		return -ENOMEM;
	}
	local_id++;
	wake_up(&wee_wait);
	return strlen(input);
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
	struct identity *it, *nx = NULL;

	kthread_stop(mythread);
	list_for_each_entry_safe (it, nx, &identity_list, lst) {
		list_del(&it->lst);
		kfree(it);
	}
	return misc_deregister(&miscdev);
}

module_init(initialize);
module_exit(destroy);
