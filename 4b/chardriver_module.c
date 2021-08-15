/**
 * Character driver open close read write
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEV_NAME "mychardev2"
#define DEV_MAJ 255
#define DEV_MIN 1
#define KBUF_LEN 100

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
MODULE_DESCRIPTION("Character Driver Module for open read write operations");

/**function prototypes**/
int CHARDEV_open(struct inode *inode, struct file *file);
int CHARDEV_release(struct inode *inode, struct file *file);
ssize_t CHARDEV_write(struct file *__file, const char *__buf, size_t __nbytes, loff_t *offp);
ssize_t CHARDEV_read(struct file *__file, char *__buf, size_t __nbytes, loff_t *offp);

/* member variables*/
struct file_operations my_device_file_operations =
    {
        .owner = THIS_MODULE,
        .open = CHARDEV_open,
        .release = CHARDEV_release,
        .write = CHARDEV_write,
        .read = CHARDEV_read,
};

struct cdev *my_cdev;
char kbuf[KBUF_LEN]; //kernel side's buffer

/*Module functions definition*/

static int __init MODULE_init(void)
{
    int result, MAJ, MIN;
    dev_t my_device;

    INFO("MODULE_init() enter");

    strcpy(kbuf, "Test Data Already Inside this Char Device"); //initialize the char buffer

    //make a dev_t statically using MKDEV
    my_device = MKDEV(DEV_MAJ, DEV_MIN);
    MAJ = MAJOR(my_device);
    MIN = MINOR(my_device);
    INFO("MKDEV: my_device MAJ=%d, MIN=%d", MAJ, MIN);

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

/**
 * my implementation for write function for this char device
 * 
 * @param __file  pointer to the file struct for this device
 * @param __ubuf the user space buffer to be written (the bytes buffer received from UserApp)
 * @param __nbytes the number of bytes to be written (this value is the same as passed in the write() system call from UserApp)
 * @param offp the offset to be applied before writing the content
 * 
 */
ssize_t CHARDEV_write(struct file *__file, const char *__ubuf, size_t __nbytes, loff_t *offp)
{
    int result, bytes_to_write;
    ssize_t retval;

    INFO("CHARDEV_write(fp=%p,ubuf=[%s],nbytes=%d)", __file, __ubuf, __nbytes);

    memset(kbuf, '\0', KBUF_LEN); //first clear the kernel buffer

    //check if the bytes to be write exceed the value present int he device
    bytes_to_write = __nbytes > KBUF_LEN ? KBUF_LEN : __nbytes;

    result = copy_from_user(kbuf, __ubuf, bytes_to_write);

    if (result == 0)
    {
        INFO("%d bytes of data successfully written", bytes_to_write);
        INFO("Message received from User = %s", kbuf);
        retval = bytes_to_write;
    }
    else
    {
        ERR("Error writing data");
        retval = -EFAULT;
    }
    INFO("CHARDEV_write() exit");
    return retval;
}

/**
 * my implementation of read function for this char device
 * 
 * @param __file  pointer to the file struct associated with this device
 * @param __ubuff user space buffer in whic hthe data is to be copied back
 * @param __nbytes the number of bytes to be read(this value is same as the value received from read system call from UserApp app)
 * @param ppof the start offset to be applied  before reading from this device
 * 
 */
ssize_t CHARDEV_read(struct file *__file, char *__ubuf, size_t __nbytes, loff_t *offp)
{
    int result, bytes_to_read;
    ssize_t retval;

    INFO("CHARDEV_read(fp=%p,ubuf=[%s],nbytes=%d)", __file, __ubuf, __nbytes);

    //check if the bytes to be read exceed the value present int he device
    bytes_to_read = __nbytes > KBUF_LEN ? strlen(kbuf) : __nbytes;

    result = copy_to_user(__ubuf, kbuf, bytes_to_read);

    if (result == 0)
    {
        INFO("Kernel buffer read successfully %d", bytes_to_read);
        retval = bytes_to_read;
    }
    else
    {
        ERR("Error writing data to user");
        retval = -EFAULT;
    }
    INFO("CHARDEV_read() exit");
    return retval;
}

int CHARDEV_release(struct inode *inode, struct file *file)
{
    INFO("CHARDEV_release(..)");
    return 0;
}