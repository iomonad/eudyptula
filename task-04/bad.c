/* TASK_01 */

#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TASK01");
MODULE_AUTHOR("IOMONAD");

static __init
int initialize(void) {
              printk(KERN_DEBUG "HELLO WORLD");
	      return 0;
}

static __exit
void destroy(void) {
              printk(KERN_DEBUG "GOODBYE WORLD");
}

module_init(initialize);
module_exit(destroy);
