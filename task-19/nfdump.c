/* TASK_19 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/netfilter.h>
#include <linux/textsearch.h>
#include <linux/netfilter_ipv4.h>

#define PATTERN "iomonad_computorz_lord"

static struct nf_hook_ops nho;
static struct ts_config *tsc = NULL;

static unsigned int custom_nf_dump_hk(void *priv, struct sk_buff *skb,
				      const struct nf_hook_state *state)
{
	if (skb_find_text(skb, 0, skb->len, tsc) != UINT_MAX) {
		pr_info("Found matching pattern in packet (size=%d): %s !!",
			skb->len, PATTERN);
	}
	return NF_ACCEPT;
}

static int nf_dump_init(void)
{
	nho.hook = custom_nf_dump_hk;
	nho.hooknum = NF_INET_PRE_ROUTING;
	nho.pf = PF_INET;
	nho.priority = NF_IP_PRI_FIRST;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 13, 0)
	return nf_register_net_hook(&init_net, &nho);
#else
	return nf_register_hook(&nho);
#endif
}

static __init int initialize(void)
{
	tsc = textsearch_prepare("kmp", PATTERN, strlen(PATTERN), GFP_KERNEL,
				 TS_AUTOLOAD);
	if (IS_ERR(tsc))
		return PTR_ERR(tsc);
	if (nf_dump_init()) {
		pr_info("nfdump: Error while registering nf hook.");
		return -EINVAL;
	}
	return 0;
}

static __exit void destroy(void)
{
	textsearch_destroy(tsc);
	return
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 13, 0)
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
