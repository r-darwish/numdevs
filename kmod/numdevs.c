#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/module.h>
#include "names.h"

MODULE_LICENSE("GPL");

static struct class * device_class;
static struct cdev c_devs[DEVICES];
static struct device * devices[DEVICES];
static dev_t major = -1;

static int numdev_open(struct inode * i, struct file * f)
{
    return 0;
}

static int numdev_close(struct inode * i, struct file * f)
{
    return 0;
}

static ssize_t numdev_read(struct file * f, char __user * buf, size_t len, loff_t * off)
{
    int i = 0;
    char to_write = f->f_inode->i_rdev - major + 1;
    for (i = 0; i < len; i++) {
        buf[i] = to_write;
    }
    return len;
}

static int numdev_dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0444);
    return 0;
}

static struct file_operations file_ops =
{
 .owner = THIS_MODULE,
 .open = numdev_open,
 .release = numdev_close,
 .read = numdev_read
};

static void cleanup(void)
{
    int i = 0;

    for (i = 0; i < DEVICES; i++) {
        if (c_devs[i].dev != 0) {
            cdev_del(&c_devs[i]);
        }
        if (devices[i] != NULL) {
            device_destroy(device_class, major + i);
            devices[i] = NULL;
        }
    }

    if (device_class != NULL) {
        class_destroy(device_class);
        device_class = NULL;
    }

    if (major != -1) {
        unregister_chrdev_region(major, DEVICES);
        major = -1;
    }
}

static int __init numdevs_init(void)
{
    int ret = 0;
    int i = 0;

    if (alloc_chrdev_region(&major, 0, DEVICES, "numdevs") < 0) {
        printk("Region allocation failed\n");
        ret = -1;
        goto out;
    }

    device_class = class_create(THIS_MODULE, "numdevs");
    if (device_class == NULL) {
        printk("Class creation failed\n");
        ret = -1;
        goto out;
    }
    device_class->dev_uevent = numdev_dev_uevent;

    for (i = 0; i < DEVICES; i++) {
        devices[i] = device_create(device_class, NULL, major + i, NULL, names[i]);
        if (devices[i] == NULL) {
            printk("Falied to create device #%d\n", i);
            ret = -1;
            goto out;
        }

        cdev_init(&c_devs[i], &file_ops);
        if (cdev_add(&c_devs[i], major + i, 1)) {
            printk("Falied to add cdev #%d\n", i);
            ret = -1;
            goto out;
        }
    }

    goto out;
out:
    if (ret != 0) {
        cleanup();
    }
    return ret;
}

static void __exit numdevs_exit(void)
{
    cleanup();
}

module_init(numdevs_init);
module_exit(numdevs_exit);
