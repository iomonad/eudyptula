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

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My eudyptula sysfs implementation");
MODULE_AUTHOR("IOMONAD");

#define SYSFS_FNAME "eudyptula"

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
	if (strncmp(buff, g_id, 7) == 0) {
		return strlen(g_id);
	}
	return -EINVAL; /* write error: invalid argument */
}

static struct kobj_attribute id_file_attribute =
	__ATTR(foo, 0444, id_show, id_store);

/* `jiffies` implementation */

static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buff)
{
	return sprintf(buff, "%lu\n", jiffies);
}

static struct kobj_attribute jiffies_file_attribute =
	__ATTR(foo, 0444, jiffies_show, NULL);

/* `foo` implementation */

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buff)
{
	return sprintf(buff, "%s\n", g_id);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buff, size_t count)
{
	if (buff) {
		strcpy(g_foo, buff);
		return strlen(g_foo);
	}
	return 0;
}

static struct kobj_attribute foo_file_attribute =
	__ATTR(foo, 0444, foo_show, foo_store);

/* `kobject` attributes */

static struct attribute *attrs[] = { &id_file_attribute.attr,
				     &jiffies_file_attribute.attr,
				     &foo_file_attribute.attr, NULL };
static struct attribute_group attr_group = { .attrs = attrs };

/* Core Module */

static __init int initialize(void)
{
	int retval;

	if ((g_foo = (char *)kmalloc(1024, GFP_KERNEL)) == NULL) {
		return 0;
	}

	/*
	 * Create a simple kobject named "eudyptula" in /sys/kernel.
	 *
	 * Since this is a simple directory, no 'uevent' will be sent
	 * to userspace. That why this function shouldn't be used of
	 * any type of dynamic kobjects, where the name & number are
	 * not know ahead of time.
	 */
	if (!(eudy_sysfs = kobject_create_and_add(SYSFS_FNAME, kernel_kobj)))
		return -ENOMEM;

	/* Create files associated the 'eudy_sysfs' kobject */
	if ((retval = sysfs_create_group(eudy_sysfs, &attr_group)) != 0)
		kobject_put(eudy_sysfs);
	return retval;
}

static __exit void destroy(void)
{
	if (g_foo != NULL) {
		kfree(g_foo);
	}
	return kobject_put(eudy_sysfs);
}

module_init(initialize);
module_exit(destroy);
