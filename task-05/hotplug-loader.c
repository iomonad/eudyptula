/* TASK_05 */

#include <linux/usb.h>
#include <linux/hid.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("USBmon toy driver");
MODULE_AUTHOR("IOMONAD <iomonad@riseup.net>");

#if !defined(MODULE)
  #define MY_NAME "usbmon-toy"
#else
  #define MY_NAME THIS_MODULE->name
#endif
#define err(format, arg...) printk(KERN_ERR "%s: " format, MY_NAME, ## arg)

static struct usb_device_id usbmon_toy_table[] = {
     {USB_INTERFACE_INFO
      (USB_INTERFACE_CLASS_HID,
       USB_INTERFACE_SUBCLASS_BOOT,
       USB_INTERFACE_PROTOCOL_KEYBOARD)},
     {}
};

MODULE_DEVICE_TABLE (usb, usbmon_toy_table); /* DEPMOD required */

static int usbmon_probe(struct usb_interface *intf,
			const struct usb_device_id *id)
{
    pr_alert("USB KEYBOARD PLUGGED IN");
    return 0;
}

static void usbmon_disconnect(struct usb_interface *intf)
{
    pr_alert("USB KEYBOARD DISCONNECTED");
}

static struct usb_driver usbmon_toy_driver = {
    .name = "usbmon_toy",
    .probe = usbmon_probe,
    .disconnect = usbmon_disconnect,
    .id_table = usbmon_toy_table
};

static __init int initialize(void)
{
    int result;

    printk(KERN_DEBUG "Loaded USBmon toy driver");
    if ((result = usb_register(&usbmon_toy_driver)) < 0) {
	err("`usb_register` failed for the "__FILE__" driver."
	    "Error number %d", result);
	return -1;
    }
    return 0;
}

static __exit void destroy(void)
{
    printk(KERN_DEBUG "Destroying USBmon toy driver");
    usb_deregister(&usbmon_toy_driver);
}

module_init(initialize);
module_exit(destroy);
