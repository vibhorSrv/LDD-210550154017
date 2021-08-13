/**
 * Character driver open close
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vibhor");
MODULE_DESCRIPTION("Character Driver Module");

/**prototypes**/
int my_device_open(struct inode *inode, struct file *file);
int my_device_release(struct inode *inode, struct file *file);

/*definitons*/
struct file_operations my_device_file_operations =
    {
        .owner = THIS_MODULE,
        .open = my_device_open,
        .release = my_device_release,
};

struct cdev *my_cdev;

static int __init my_module_init(void)
{
    int result;
    int MAJ, MIN;

    //make a dev_t statically using MKDEV
    dev_t my_device = MKDEV(255, 0);
    MAJ = MAJOR(my_device);
    MIN = MINOR(my_device);
    printk(KERN_ALERT "my_module_init(void): MKDEV: my_device MAJ=%d, MIN=%d", MAJ, MIN);

    //register the newly created device with its region in /dev/
    result = register_chrdev_region(my_device, 1, "mychardev1");

    //if unsuccessful
    if (result < 0)
    {
        printk(KERN_ALERT "Device could not be created!\n");
        return -1;
    }
    my_cdev = cdev_alloc(); //allocate a cdev structure
    my_cdev->ops = &my_device_file_operations;

    result = cdev_add(my_cdev, my_device, 1); //add my_device to cdev
    //if unsuccessful
    if (result < 0)
    {
        printk(KERN_ALERT "Device could not be added!\n");
        unregister_chrdev_region(my_device, 1);
        return -1;
    }

    return 0;
}

static void __exit my_module_exit(void)
{
    int MAJ, MIN;
    dev_t my_device = MKDEV(255, 0);
    MAJ = MAJOR(my_device);
    MIN = MINOR(my_device);
    printk("\nMKDEV: my_device MAJ=%d, MIN=%d", MAJ, MIN);
    unregister_chrdev_region(my_device, 1);
    cdev_del(my_cdev);
    printk(KERN_ALERT "my_module_exit(void): Device Unregistered successfully, Sayonara\n");
}

int my_device_open(struct inode *inode, struct file *file)
{
    printk("inside my_device_open(..)");
    return 0;
}

int my_device_release(struct inode *inode, struct file *file)
{
    printk("inside my_device_release(..)");
    return 0;
}

module_init(my_module_init);
module_exit(my_module_exit);