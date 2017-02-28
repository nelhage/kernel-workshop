/**
 * hello_package -- a slightly more involved kernel module
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nelson Elhage");
MODULE_DESCRIPTION("Packet Logger");
MODULE_VERSION("0.1");

int log_packets;
module_param(log_packets, int, 0644);

unsigned int log_hook(void *p,
		      struct sk_buff *skb,
		      const struct nf_hook_state *state)  {
	struct sock *sk = skb->sk;
	printk(KERN_DEBUG "packet!");
	return NF_ACCEPT;
}

static struct nf_hook_ops nfho =
	{
	 .hook = log_hook,
	 .hooknum = 0,
	 .pf = PF_INET,
	 .priority = NF_IP_PRI_FIRST,
	};


static int __init hello_packet_init(void){
	nf_register_hook(&nfho);
	printk(KERN_INFO "Registered logging hook!\n");
	return 0;
}

static void __exit hello_packet_exit(void){
	nf_unregister_hook(&nfho);
}


module_init(hello_packet_init);
module_exit(hello_packet_exit);
