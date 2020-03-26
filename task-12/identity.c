/* LEVEL12 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/err.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked List Kernel Impl");
MODULE_AUTHOR("IOMONAD");

#define NAME_LEN 20

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

	list_for_each_entry (it, &identity_list, lst) {
		if (it->id == id) {
			return it;
		}
	}
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
	memset(new->name, 0x00, NAME_LEN);
	strncpy(new->name, name, NAME_LEN - 1);
	new->id = id;
	new->busy = false;
	list_add(&(new->lst), &identity_list);
	pr_info("identity %d: with '%s' name was created\n", id, name);
	return 0;
}

/* DELETE */
void identity_destroy(int id)
{
	struct identity *temp = NULL;

	if (!(temp = identity_find(id))) {
		return;
	}
	list_del(&(temp->lst));
	kfree(temp);
	return;
}

static __init int initialize(void)
{
	struct identity *temp = NULL;

	if (identity_create("Alice", 1))
		pr_debug("error creating Alice\n");
	if (identity_create("Bob", 2))
		pr_debug("error creating Bob\n");
	if (identity_create("Dave", 3))
		pr_debug("error creating Dave\n");
	if (identity_create("Gena", 10))
		pr_debug("error creating Gena\n");

	temp = identity_find(3);
	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
}

static __exit void destroy(void)
{
}

module_init(initialize);
module_exit(destroy);
