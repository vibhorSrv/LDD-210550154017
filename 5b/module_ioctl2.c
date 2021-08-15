/**
 * Calculator functionality using char driver ioctl functions
 * using a struct
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include "module_ioctl2.h"

#define DEV_NAME "my_ioctldev2"

#define DEV_MAJ 255

#define DEV_MIN 4

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

static struct cdev *my_cdev;

static struct oper_container *k_oper_container;

/*Module functions definition*/

static int __init MODULE_init(void)
{
    int add_device_result, MAJ, MIN;

    dev_t my_device;

    k_oper_container = (struct oper_container *)__kmalloc(sizeof(struct oper_container), GFP_KERNEL);

    INFO("MODULE_init() enter");

    //make a dev_t statically using MKDEV
    my_device = MKDEV(DEV_MAJ, DEV_MIN);

    MAJ = MAJOR(my_device);

    MIN = MINOR(my_device);

    INFO("MKDEV: /dev/" DEV_NAME " MAJ=%d, MIN=%d", MAJ, MIN);

    //register the newly created device with its region in /dev/
    add_device_result = register_chrdev_region(my_device, 1, DEV_NAME);

    //if unsuccessful
    if (add_device_result < 0)
    {
        ERR("Device could not be created!\n");

        return -1;
    }

    INFO("Device Registered successfully");

    my_cdev = cdev_alloc(); //allocate a cdev structure

    my_cdev->ops = &my_device_file_operations;

    add_device_result = cdev_add(my_cdev, my_device, 1); //add my_device to cdev

    //if unsuccessful
    if (add_device_result < 0)
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
    dev_t my_device;

    INFO("MODULE_exit() enter");

    my_device = MKDEV(DEV_MAJ, DEV_MIN);

    INFO("MKDEV: my_device MAJ=%d, MIN=%d", MAJOR(my_device), MINOR(my_device));

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
    int res;
    struct oper_container* c;

    INFO("CHARDEV_ioctl(..) in");

    switch (cmd)
    {
    case IOCT_CALC_PARCEL:
        c = (struct oper_container *)arg;
        retval = copy_from_user(k_oper_container, (struct oper_container *)arg, sizeof(struct oper_container));

        INFO("%d  %c  %d", k_oper_container->operand1, k_oper_container->operator, k_oper_container->operand2);

        break;
    case IOCT_RES:
        switch (k_oper_container->operator)
        {
        default:
            ERR("Arithemetic Operation %c Not Supported Error", k_oper_container->operator);
            retval = -1;
            goto ret;
        case '+':
            res = k_oper_container->operand1 + k_oper_container->operand2;
            break;
        case '-':
            res = k_oper_container->operand1 - k_oper_container->operand2;
            break;
        case '*':
            res = k_oper_container->operand1 * k_oper_container->operand2;
            break;
        case '/':
            if (k_oper_container->operand2 == 0)
                res = 0;
            else
                res = k_oper_container->operand1 / k_oper_container->operand2;
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
