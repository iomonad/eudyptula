/* TASK_12 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/err.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TASK12");
MODULE_AUTHOR("IOMONAD");

#define NAME_SIZE 20

/* struct indentity { */
/* 	char name[NAME_SIZE]; */
/* 	int id; */
/* 	bool busy; */
/* 	struct list_head curr; */
/* }; */

static LIST_HEAD(identity_list);

/* FIND */
struct identity *identity_find(int id)
{
	struct identity *cursor = NULL;

	list_for_each_entry (cursor, &identity_list, curr) {
		if (cursor->id == id) {
			return cursor;
		}
	}
	return NULL;
}

/* CREATE */
int identity_create(char *name, int id)
{
	struct identity *new = NULL;

	if (identity_find(id)) {
		pr_info("identity: id '%d' already exists.", id);
		return 1;
	}
	if (name == NULL)
		return 1;
	if (!(new = (struct identity *)kmalloc(sizeof(struct identity),
					       GFP_KERNEL))) {
		return -ENOMEM;
	}
	strncpy(new->name, name, NAME_SIZE);
	new->id = id;
	new->busy = false;
	list_add(&(new->list), &identity_list);
	return 0;
}

void identity_destroy(int id)
{
	struct indentity *destroy = NULL;

	if (!(destroy = identity_find(id))) {
		pr_debug("identity: id '%d' not found while destroy", id);
		return;
	}
	list_del(&(destroy->curr));
	if (destroy) {
		kfree(destroy);
	}
	return;
}

static __init int initialize(void)
{
	struct identity *temp;

	if (identity_create("Alice", 1)) {
		pr_debug("error creating Alice\n");
	}
	if (identity_create("Bob", 2)) {
		pr_debug("error creating Bob\n");
	}
	if (identity_create("Dave", 3)) {
		pr_debug("error creating Dave\n");
	}
	if (identity_create("Gena", 10)) {
		pr_debug("error creating Gena\n");
	}

	temp = identity_find(3);
	pr_debug("id 3 = %s\n", temp->name);

	if (!(temp = identity_find(42))) {
		pr_debug("id 42 not found\n");
	}

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);
	return 0;
}

static __exit void destroy(void)
{
	return;
}

module_init(initialize);
module_exit(destroy);
