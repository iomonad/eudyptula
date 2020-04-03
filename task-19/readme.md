# Kernel Netfilter Module

## Using netfilter hook mechanism

Netfilter offers something called netfilter hooks,
which is a way to use callbacks in order to filter packets inside the kernel.

There are 5 different kinds of netfilter hooks:

```
A Packet Traversing the Netfilter System:

   --->[1]--->[ROUTE]--->[3]--->[4]--->
                 |            ^
                 |            |
                 |         [ROUTE]
                 v            |
                [2]          [5]
                 |            ^
                 |            |
                 v            |
```
- NF_IP_PER_ROUNTING — This hook is called when a packet arrives into the machine.
- NF_IP_LOCAL_IN — This hook is called when a packet is destined to the machine itself.
- NF_IP_FORWARD — This hook is called when a packet is destined to another interface.
- NF_IP_POST_ROUTING — Is called when a packet is on its way back to the wire and outside the machine.
- NF_IP_LOCAL_OUT — When a packet is created locally, and is destined out, this hook is called.

## Creating our hook

Populate the nf's structure:

`struct nf_hook_ops`

Then create your call back and add the fn_ptr to the structure.
(As always, init the hook with `nf_register_net_hook` & `nf_unregister_net_hook` )

Once your callback created you will be able to process each incoming packets, which
are represented by the `struct sk_buff` ptr, a structure that contains the complete
architecture, based on ptr offset. It's recommended to use provided helpers in the 
header to avoid memory manipulation mistake.

## Grepping in data section

To grep into our data packet, linux provide a nice tool nammed textsearch in `lib/textsearch.c`

Init:

```c
tsc = textsearch_prepare("kmp", PATTERN, strlen(PATTERN), GFP_KERNEL, TS_AUTOLOAD);
```

Finding:
```c
    /*                     SEARCH RANGE                   */
	/*                     <--------->                    */
 	if (skb_find_text(skb, 0, skb->len, tsc) != UINT_MAX) {
```

Deinit:
```c
 	textsearch_destroy(tsc);
```


## Testing

Run test.sh file wich craft a packet with netcat

Then check kernel logs:

```
[ 4281.741738] Found matching pattern in packet (size=143): iomonad_computorz_lord !!
```

[1] https://netfilter.org/documentation/HOWTO/fr/netfilter-hacking-HOWTO-3.html
[2] http://amsekharkernel.blogspot.com/2014/08/what-is-skb-in-linux-kernel-what-are.html
[3] https://www.kernel.org/doc/Documentation/core-api/kernel-api.rst
