/**
 * Character driver thet performs subtract operation
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "../kern_util.h"

#define DEV_NAME "my_sub_dev"
#define DEV_MAJ 255
#define DEV_MIN 6

#define KBUF_LEN 30

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vibhor");
MODULE_DESCRIPTION("Character driver thet performs SUB operation");

/**function prototypes**/
int SUB_DEV_open(struct inode *inode, struct file *file);
int SUB_DEV_release(struct inode *inode, struct file *file);
ssize_t SUB_DEV_write(struct file *__file, const char *__buf, size_t __nbytes, loff_t *offp);
ssize_t SUB_DEV_read(struct file *__file, char *__buf, size_t __nbytes, loff_t *offp);

/* member variables*/
struct file_operations my_device_file_operations =
    {
        .owner = THIS_MODULE,
        .open = SUB_DEV_open,
        .release = SUB_DEV_release,
        .write = SUB_DEV_write,
        .read = SUB_DEV_read,
};

struct cdev *my_cdev;
static char kbuf[KBUF_LEN]; //kernel side's buffer
static int num_idx = 0;
static int operands[2] = {0};

/*Module functions definition*/

static int __init SUB_MOD_init(void)
{
    int result, MAJ, MIN;
    dev_t my_device;

    INFO("SUB_MOD_init() enter");

    strcpy(kbuf, "Test Data Already Inside this Char Device"); //initialize the char buffer

    //make a dev_t statically using MKDEV
    my_device = MKDEV(DEV_MAJ, DEV_MIN);
    MAJ = MAJOR(my_device);
    MIN = MINOR(my_device);
    INFO("MKDEV: /dev/"DEV_NAME" MAJ=%d, MIN=%d", MAJ, MIN);

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

    INFO("SUB_MOD_init() exit");

    return 0;
}

static void __exit SUB_MOD_exit(void)
{
    int MAJ, MIN;
    dev_t my_device = MKDEV(DEV_MAJ, DEV_MIN);
    INFO("SUB_MOD_exit() enter")

    MAJ = MAJOR(my_device);
    MIN = MINOR(my_device);
    INFO("MKDEV: my_device MAJ=%d, MIN=%d", MAJ, MIN);
    unregister_chrdev_region(my_device, 1);
    cdev_del(my_cdev);

    INFO("exited: Device Unregistered successfully, Sayonara");
    INFO("SUB_MOD_exit() exit");
}

module_init(SUB_MOD_init);
module_exit(SUB_MOD_exit);

/*Char device file operation functions definitions*/

/**
 * Char device open
 *
 */
int SUB_DEV_open(struct inode *inode, struct file *file)
{
    INFO("SUB_DEV_open(..)");
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
ssize_t SUB_DEV_write(struct file *__file, const char *__ubuf, size_t __nbytes, loff_t *offp)
{
    int result, bytes_to_write;
    ssize_t retval;

    INFO("SUB_DEV_write(fp=%p,ubuf=[%s],nbytes=%d)", __file, __ubuf, __nbytes);

    memset(kbuf, '\0', KBUF_LEN); //first clear the kernel buffer

    //check if the bytes to be write exceed the value present int he device
    bytes_to_write = __nbytes > KBUF_LEN ? KBUF_LEN : __nbytes;

    result = copy_from_user(kbuf, __ubuf, bytes_to_write);

    if (result == 0)
    {
        INFO("%d bytes of data successfully written", bytes_to_write);
        INFO("Message received from User = %s", kbuf);
        kstrtoint(kbuf, 10, &operands[num_idx]);
        num_idx = (num_idx + 1) % 2;
        retval = bytes_to_write;
    }
    else
    {
        ERR("Error writing data");
        retval = -EFAULT;
    }
    INFO("SUB_DEV_write() exit");
    return retval;
}

/**
 * my implementation of read function for this char device
 * 
 * @param __file  pointer to the file struct associated with this device
 * @param __ubuff user space buffer in whic hthe data is to be copied back
 * @param __nbytes the number of bytes to be read(this value is same as the value received from read system call from UserApp app)
 * @param offp the start offset to be applied  before reading from this device
 * 
 */
ssize_t SUB_DEV_read(struct file *__file, char *__ubuf, size_t __nbytes, loff_t *offp)
{
    int result, oper_result;
    ssize_t retval;
    char oper_result_string[30] = {'\0'};

    INFO("SUB_DEV_read(fp=%p,ubuf=[%s],nbytes=%d)", __file, __ubuf, __nbytes);

    oper_result = operands[0] - operands[1];

    result = sprintf(oper_result_string, "%d", oper_result);

    if (result == 0) //if could not convert the number to string
    {
        retval = -EFAULT;
        goto ret;
    }

    INFO("Operation Result = %s", oper_result_string);
    INFO("Operation Result = %d", oper_result);

    result = copy_to_user(__ubuf, oper_result_string, sizeof(oper_result_string));

    if (result == 0) //if copy to user successful
    {
        INFO("Kernel buffer read successfully %d", sizeof(oper_result_string));
        retval = sizeof(oper_result_string);
    }
    else
    {
        ERR("Error writing data to user");
        retval = -EFAULT;
    }

ret:
    INFO("SUB_DEV_read() exit");
    return retval;
}

int SUB_DEV_release(struct inode *inode, struct file *file)
{
    INFO("SUB_DEV_release(..)");
    return 0;
}