/* LEVEL12 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Tasks");
MODULE_AUTHOR("IOMONAD");

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
