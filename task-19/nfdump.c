/* TASK_19 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static __init int initialize(void)
{
	return 0;
}

static __exit void destroy(void)
{
	return ;
}

module_init(initialize);
module_exit(destroy);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Netfilter Dump module");
MODULE_AUTHOR("IOMONAD");
