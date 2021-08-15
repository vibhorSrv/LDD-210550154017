/**
 * Character driver open close : dynamically allocate device number
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEV_NAME "mychardev3"

#define ERR(...)                                           \
    {                                                      \
        printk(KERN_ERR "chardriver_module: "__VA_ARGS__); \
    }
#define INFO(...)                                           \
    {                                                       \
        printk(KERN_INFO "chardriver_module: "__VA_ARGS__); \
    }

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Vibhor");

MODULE_DESCRIPTION("Character Driver Module 3");

/**prototypes**/
int CHARDEV_open(struct inode *inode, struct file *file);

int CHARDEV_release(struct inode *inode, struct file *file);

struct file_operations my_device_file_operations =
    {
        .owner = THIS_MODULE,
        .open = CHARDEV_open,
        .release = CHARDEV_release,
};

struct cdev *my_cdev;
dev_t my_device;
/*definitons*/

static int __init MODULE_init(void)
{
    int add_device_result;

    int MAJ, MIN;

    //dynamically allocate a device for /dev/chardev3
    add_device_result = alloc_chrdev_region(&my_device, 0, 1, DEV_NAME);

    //if unsuccessful
    if (add_device_result < 0)
    {
        ERR("Device could not be created!\n");
        return -1;
    }
    else
    {
        MAJ = MAJOR(my_device);

        MIN = MINOR(my_device);

        INFO("Allocated MAJ/MIN by Kernel for /dev/" DEV_NAME " =  MAJ=%d, MIN=%d)", MAJ, MIN);
    }

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

    return 0;
}

static void __exit MODULE_exit(void)
{
    int MAJ, MIN;

    MAJ = MAJOR(my_device);

    MIN = MINOR(my_device);

    INFO("my_device MAJ=%d, MIN=%d", MAJ, MIN);

    unregister_chrdev_region(my_device, 1);

    cdev_del(my_cdev);

    INFO("MODULE_exit(void): Device Unregistered successfully, Sayonara\n");
}

int CHARDEV_open(struct inode *inode, struct file *file)
{
    INFO("CHARDEV_open(..)");

    return 0;
}

int CHARDEV_release(struct inode *inode, struct file *file)
{
    INFO("CHARDEV_release(..)");

    return 0;
}

module_init(MODULE_init);

module_exit(MODULE_exit);