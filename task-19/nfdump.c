/* TASK_19 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

static struct nf_hook_ops nho;

static unsigned int custom_nf_dump_hk(
	void *priv, struct sk_buff *skb,
	const struct nf_hook_state *state)
{

	pr_info("packet size: %d", 3); /* FIXME */
	return NF_ACCEPT;
}

static int nf_dump_init(void)
{
	nho.hook = custom_nf_dump_hk;
	nho.hooknum = 0;	/* ALL INCOMMING */
	nho.pf = PF_INET;
	nho.priority = NF_IP_PRI_FIRST;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
	return nf_register_net_hook(&init_net, &nho);
#else
	return nf_register_hook(&nho);
#endif
}

static __init int initialize(void)
{
	if (nf_dump_init()) {
		pr_info("nfdump: Error while registering nf hook.");
		return -EINVAL;
	}
	return 0;
}

static __exit void destroy(void)
{
	return
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
		nf_unregister_net_hook(&init_net, &nho);
#else
	        nf_unregister_hook(&nho);
#endif
}

module_init(initialize);
module_exit(destroy);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Netfilter Dump module");
MODULE_AUTHOR("IOMONAD");
