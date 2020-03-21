/* TASK_08 */

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My eudyptula sysfs implementation");
MODULE_AUTHOR("IOMONAD");
MODULE_VERSION("0.0.1");

#define SYSFS_FNAME "eudyptula"

static DEFINE_MUTEX(my_mutex);

static struct kobject *eudy_sysfs = NULL;
static char g_id[] = "iomonad";
static char *g_foo = NULL;

/* `id` implementation*/

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
		       char *buff)
{
	return sprintf(buff, "%s\n", g_id);
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buff, size_t count)
{
	if (strncmp(buff, g_id, 7) == 0)
		return strlen(g_id);
	return -EINVAL; /* write error: invalid argument */
}

static struct kobj_attribute id_file_attribute =
	__ATTR(id, 0644, id_show, id_store);

/* `jiffies` implementation */

static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buff)
{
	return sprintf(buff, "%lu\n", jiffies);
}

static ssize_t jiffies_store(struct kobject *kobj, struct kobj_attribute *attr,
			     const char *buff, size_t count)
{
	return -EINVAL; /* IMPLEMENTATION NEED */
}

static struct kobj_attribute jiffies_file_attribute =
	__ATTR(jiffies, 0444, jiffies_show, jiffies_store);

/* `foo` implementation */

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buff)
{
	int retval;

	mutex_lock(&my_mutex);
	retval = sprintf(buff, "%s\n", g_foo);
	mutex_unlock(&my_mutex);
	return (retval);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buff, size_t count)
{
	if (count > PAGE_SIZE)
		return -ENOMEM;
	if (buff) {
		mutex_lock(&my_mutex);
		strncpy(g_foo, buff, count);
		mutex_unlock(&my_mutex);
		return strlen(g_foo);
	}
	return 0;
}

static struct kobj_attribute foo_file_attribute =
	__ATTR(foo, 0644, foo_show, foo_store);

/* `kobject` attributes */

static struct attribute *attrs[] = { &id_file_attribute.attr,
				     &jiffies_file_attribute.attr,
				     &foo_file_attribute.attr, NULL };

static struct attribute_group attr_group = {
	.attrs = attrs,
};

/* Core Module */

static __init int initialize(void)
{
	int retval;

	if (!(g_foo = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL)))
		return -ENOMEM;

	/*
	 * Create a simple kobject named "eudyptula" in /sys/kernel.
	 *
	 * Since this is a simple directory, no 'uevent' will be sent
	 * to userspace. That why this function shouldn't be used of
	 * any type of dynamic kobjects, where the name & number are
	 * not know ahead of time.
	 */
	if (!(eudy_sysfs = kobject_create_and_add(SYSFS_FNAME, kernel_kobj))) {
		pr_info("MYSYSFS: `eudy_sysfs` kernel object returned NULL value");
		return -ENOMEM;
	}
	/* Create files associated the 'eudy_sysfs' kobject */
	if ((retval = sysfs_create_group(eudy_sysfs, &attr_group))) {
		pr_info("MYSYSFS: `sysfs_create_group()` returned non zero value");
		kobject_put(eudy_sysfs);
	}
	return retval;
}

static __exit void destroy(void)
{
	return kobject_put(eudy_sysfs);
}

module_init(initialize);
module_exit(destroy);
