/**
 * Calculator functionality using char driver ioctl functions
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "module_ioctl.h"

#define DEV_NAME "my_ioctldev"
#define DEV_MAJ 255
#define DEV_MIN 3
#define KBUF_LEN 100

#define ERR(...)                                      \
    {                                                 \
        printk(KERN_ERR "module_ioctl: "__VA_ARGS__); \
    }
#define INFO(...)                                      \
    {                                                  \
        printk(KERN_INFO "module_ioctl: "__VA_ARGS__); \
    }

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vibhor");
MODULE_DESCRIPTION("Character Driver Module for calculator functionality using IOCTL");

/**function prototypes**/
int CHARDEV_open(struct inode *inode, struct file *file);
int CHARDEV_release(struct inode *inode, struct file *file);

long CHARDEV_ioctl(struct file *__file, unsigned int cmg, unsigned long arg);

/* member variables*/
struct file_operations my_device_file_operations =
    {
        .owner = THIS_MODULE,
        .open = CHARDEV_open,
        .release = CHARDEV_release,
        .unlocked_ioctl = CHARDEV_ioctl,
};

struct cdev *my_cdev;

static int num1, num2, res;
static char opr;

/*Module functions definition*/

static int __init MODULE_init(void)
{
    int result, MAJ, MIN;
    dev_t my_device;

    INFO("MODULE_init() enter");

    //make a dev_t statically using MKDEV
    my_device = MKDEV(DEV_MAJ, DEV_MIN);
    MAJ = MAJOR(my_device);
    MIN = MINOR(my_device);
    INFO("MKDEV: /dev/" DEV_NAME " MAJ=%d, MIN=%d", MAJ, MIN);

    //register the newly created device with its region in /dev/
    result = register_chrdev_region(my_device, 1, DEV_NAME);

    //if unsuccessful
    if (result < 0)
    {
        ERR("Device could not be created!\n");
        return -1;
    }

    INFO("Device Registered successfully");

    my_cdev = cdev_alloc(); //allocate a cdev structure
    my_cdev->ops = &my_device_file_operations;

    result = cdev_add(my_cdev, my_device, 1); //add my_device to cdev

    //if unsuccessful
    if (result < 0)
    {
        ERR("Device could not be added!\n");
        unregister_chrdev_region(my_device, 1);
        return -1;
    }
    INFO("Added to cdev successfully");

    INFO("MODULE_init() exit");

    return 0;
}

static void __exit MODULE_exit(void)
{
    int MAJ, MIN;
    dev_t my_device = MKDEV(DEV_MAJ, DEV_MIN);
    INFO("MODULE_exit() enter")

    MAJ = MAJOR(my_device);
    MIN = MINOR(my_device);
    INFO("MKDEV: my_device MAJ=%d, MIN=%d", MAJ, MIN);
    unregister_chrdev_region(my_device, 1);
    cdev_del(my_cdev);

    INFO("exited: Device Unregistered successfully, Sayonara");
    INFO("MODULE_exit() exit");
}

module_init(MODULE_init);
module_exit(MODULE_exit);

/*Char device file operation functions definitions*/

/**
 * Char device open
 *
 */
int CHARDEV_open(struct inode *inode, struct file *file)
{
    INFO("CHARDEV_open(..)");
    return 0;
}

long CHARDEV_ioctl(struct file *__file, unsigned int cmd, unsigned long arg)
{
    long retval = -1;
    INFO("CHARDEV_ioctl(..) in");

    switch (cmd)
    {
    case IOCT_NUM1:
        retval = copy_from_user(&num1, (int *)arg, sizeof(num1));
        INFO("num1 = %d", num1);
        break;
    case IOCT_NUM2:
        retval = copy_from_user(&num2, (int *)arg, sizeof(num2));
        INFO("num2 = %d", num2);
        break;
    case IOCT_OPR:
        retval = copy_from_user(&opr, (char *)arg, sizeof(opr));
        INFO("opr = %c", opr);
        break;
    case IOCT_RES:
        switch (opr)
        {
        default:
            ERR("Arithemetic Operation %c Not Supported Error", opr);
            retval = -1;
            goto ret;
        case '+':
            res = num1 + num2;
            break;
        case '-':
            res = num1 - num2;
            break;
        case '*':
            res = num1 * num2;
            break;
        case '/':
            if (num1 == 0)
                res = 0;
            else
                res = num1 / num2;
            break;
        }
        INFO("res = %d", res);
        retval = copy_to_user((int *)arg, &res, sizeof(res));
        break;
    default:
        ERR("IOCTL Command Not Supported");
        retval = -1;
    }
ret:
    INFO("CHARDEV_ioctl(..) out");
    return retval;
}

int CHARDEV_release(struct inode *inode, struct file *file)
{
    INFO("CHARDEV_release(..)");
    return 0;
}
