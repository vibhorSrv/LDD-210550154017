#include "calc_driver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vibhor");
MODULE_DESCRIPTION("Character driver thet performs ADD operation");

static char kbuf[KBUF_LEN]; //kernel side's buffer
static int num_idx = 0;
static int operands[2] = {0};

/* member variables*/

struct cdev *add_cdev;
struct cdev *sub_cdev;
struct cdev *mul_cdev;
struct cdev *div_cdev;

struct file_operations add_device_file_operations =
    {
        .owner = THIS_MODULE,
        .open = ADD_DEV_open,
        .release = ADD_DEV_release,
        .write = ADD_DEV_write,
        .read = ADD_DEV_read,
};

struct file_operations sub_device_file_operations =
    {
        .owner = THIS_MODULE,
        .open = SUB_DEV_open,
        .release = SUB_DEV_release,
        .write = SUB_DEV_write,
        .read = SUB_DEV_read,
};

struct file_operations mul_device_file_operations =
    {
        .owner = THIS_MODULE,
        .open = MUL_DEV_open,
        .release = MUL_DEV_release,
        .write = MUL_DEV_write,
        .read = MUL_DEV_read,
};

struct file_operations div_device_file_operations =
    {
        .owner = THIS_MODULE,
        .open = DIV_DEV_open,
        .release = DIV_DEV_release,
        .write = DIV_DEV_write,
        .read = DIV_DEV_read,
};

/*Module functions definition*/

static int __init CALC_MOD_init(void)
{
    int result;
    dev_t my_dev_t;

    INFO("CALC_MOD_init() enter");

    strcpy(kbuf, "Test Data Already Inside this Char Device"); //initialize the char buffer

    if (!(Register_New_Device(&my_dev_t, DEV_NAME_ADD, DEV_MAJ, DEV_MIN_ADD) == 0 &&
          Register_Cdev(&my_dev_t, &add_cdev, &add_device_file_operations) == 0))
    {
        ERR("Device " DEV_NAME_ADD " not added");
    }

    if (!(Register_New_Device(&my_dev_t, DEV_NAME_SUB, DEV_MAJ, DEV_MIN_SUB) == 0 &&
          Register_Cdev(&my_dev_t, &sub_cdev, &sub_device_file_operations) == 0))
    {
        ERR("Device " DEV_NAME_SUB " not added");
    }

    if (!(Register_New_Device(&my_dev_t, DEV_NAME_MUL, DEV_MAJ, DEV_MIN_MUL) == 0 &&
          Register_Cdev(&my_dev_t, &mul_cdev, &mul_device_file_operations) == 0))
    {
        ERR("Device " DEV_NAME_MUL " not added");
    }

    if (!(Register_New_Device(&my_dev_t, DEV_NAME_DIV, DEV_MAJ, DEV_MIN_DIV) == 0 &&
          Register_Cdev(&my_dev_t, &div_cdev, &div_device_file_operations) == 0))
    {
        ERR("Device " DEV_NAME_DIV " not added");
    }

    INFO("CALC_MOD_init() exit");

    return 0;
}

static void __exit CALC_MOD_exit(void)
{
    Unregister_Device(DEV_MAJ, DEV_MIN_ADD, add_cdev);
    Unregister_Device(DEV_MAJ, DEV_MIN_SUB, sub_cdev);
    Unregister_Device(DEV_MAJ, DEV_MIN_MUL, mul_cdev);
    Unregister_Device(DEV_MAJ, DEV_MIN_DIV, div_cdev);
    INFO("exited: Devices Unregistered successfully, Sayonara");
    INFO("CALC_MOD_exit() exit");
}

module_init(CALC_MOD_init);
module_exit(CALC_MOD_exit);

/*****************************************************************************/

/***************Custom functions for device registration *********************/
int Register_New_Device(dev_t *_devt, char *_dev_name, int _maj, int _min)
{
    int MAJ, MIN, result;
    *_devt = MKDEV(_maj, _min);
    MAJ = MAJOR(*_devt);
    MIN = MINOR(*_devt);
    INFO("MKDEV: /dev/%s MAJ=%d, MIN=%d", _dev_name, MAJ, MIN);
    result = register_chrdev_region(*_devt, 1, _dev_name);

    //if unsuccessful
    if (result < 0)
    {
        ERR("Device %s could not be created!\n", _dev_name);
        return -1;
    }
    else
    {
        INFO("Device %s Registered Successfully", _dev_name);
        return 0;
    }
}

int Register_Cdev(dev_t *_devt, struct cdev **_cdev, struct file_operations *_fops)
{
    int result;
    *_cdev = cdev_alloc(); //allocate a cdev structure
    (*_cdev)->ops = _fops;

    result = cdev_add(*_cdev, *_devt, 1); //add my_device to cdev

    //if unsuccessful
    if (result < 0)
    {
        ERR("CDEV could not be added!\n");
        unregister_chrdev_region(*_devt, 1);
        return -1;
    }
    else
    {
        INFO("Added to cdev successfully");
        return 0;
    }
}

void Unregister_Device(int _maj, int _min, struct cdev *_cdev)
{
    int MAJ, MIN;
    dev_t my_device = MKDEV(_maj, _min);
    MAJ = MAJOR(my_device);
    MIN = MINOR(my_device);
    INFO("Unregistering device with  MAJ=%d, MIN=%d", MAJ, MIN);
    unregister_chrdev_region(my_device, 1);
    cdev_del(_cdev);
    return;
}
/*****************************************************************************/

/******************************ADD DEV****************************************/
int ADD_DEV_open(struct inode *inode, struct file *file)
{
    INFO("ADD_DEV opened");
    return 0;
}
int ADD_DEV_release(struct inode *inode, struct file *file)
{
    INFO("ADD_DEV released");
    return 0;
}

ssize_t ADD_DEV_write(struct file *__file, const char *__buf, size_t __nbytes, loff_t *offp)
{
    int result, bytes_to_write;
    ssize_t retval;

    INFO("ADD_DEV_write(fp=%p,ubuf=[%s],nbytes=%d)", __file, __buf, __nbytes);

    memset(kbuf, '\0', KBUF_LEN); //first clear the kernel buffer

    //check if the bytes to be write exceed the value present int he device
    bytes_to_write = __nbytes > KBUF_LEN ? KBUF_LEN : __nbytes;

    result = copy_from_user(kbuf, __buf, bytes_to_write);

    if (result == 0)
    {
        INFO("%d bytes of data successfully written", bytes_to_write);
        INFO("Message received from User = %s", kbuf);
        kstrtoint(kbuf, 10, &operands[num_idx]);
        INFO("operands[%d] = %d", num_idx, operands[num_idx]);
        num_idx = (num_idx + 1) % 2;
        retval = bytes_to_write;
    }
    else
    {
        ERR("Error writing data");
        retval = -EFAULT;
    }
    INFO("ADD_DEV_write() exit");
    return retval;
}

ssize_t ADD_DEV_read(struct file *__file, char *__buf, size_t __nbytes, loff_t *offp)
{
    int result, oper_result;
    ssize_t retval;
    char oper_result_string[30] = {'\0'};

    INFO("ADD_DEV_read(fp=%p,ubuf=[%s],nbytes=%d)", __file, __buf, __nbytes);

    oper_result = operands[0] + operands[1];

    result = sprintf(oper_result_string, "%d", oper_result);

    if (result == 0) //if could not convert the number to string
    {
        retval = -EFAULT;
        goto ret;
    }

    INFO("Operation Result = %s", oper_result_string);

    result = copy_to_user(__buf, oper_result_string, sizeof(oper_result_string));

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
    INFO("ADD_DEV_read() exit");
    return retval;
}

/*****************************************************************************/

/******************************DIV DEV****************************************/


int DIV_DEV_open(struct inode *inode, struct file *file)
{
    INFO("DIV_DEV_open(..)");
    return 0;
}


ssize_t DIV_DEV_write(struct file *__file, const char *__ubuf, size_t __nbytes, loff_t *offp)
{
    int result, bytes_to_write;
    ssize_t retval;

    INFO("DIV_DEV_write(fp=%p,ubuf=[%s],nbytes=%d)", __file, __ubuf, __nbytes);

    memset(kbuf, '\0', KBUF_LEN); //first clear the kernel buffer

    //check if the bytes to be write exceed the value present int he device
    bytes_to_write = __nbytes > KBUF_LEN ? KBUF_LEN : __nbytes;

    result = copy_from_user(kbuf, __ubuf, bytes_to_write);

    if (result == 0)
    {
        INFO("%d bytes of data successfully written", bytes_to_write);
        INFO("Message received from User = %s", kbuf);
        kstrtoint(kbuf, 10, &operands[num_idx]);
        INFO("operands[%d] = %d", num_idx, operands[num_idx]);
        num_idx = (num_idx + 1) % 2;
        retval = bytes_to_write;
    }
    else
    {
        ERR("Error writing data");
        retval = -EFAULT;
    }
    INFO("DIV_DEV_write() exit");
    return retval;
}


ssize_t DIV_DEV_read(struct file *__file, char *__ubuf, size_t __nbytes, loff_t *offp)
{
    int result, oper_result;
    ssize_t retval;
    char oper_result_string[30] = {'\0'};

    INFO("DIV_DEV_read(fp=%p,ubuf=[%s],nbytes=%d)", __file, __ubuf, __nbytes);

    oper_result = operands[0] / operands[1];

    result = sprintf(oper_result_string, "%d", oper_result);

    if (result == 0) //if could not convert the number to string
    {
        retval = -EFAULT;
        goto ret;
    }

    INFO("Operation Result = %s", oper_result_string);

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
    INFO("DIV_DEV_read() exit");
    return retval;
}

int DIV_DEV_release(struct inode *inode, struct file *file)
{
    INFO("DIV_DEV_release(..)");
    return 0;
}
/*****************************************************************************/

/******************************MUL DEV****************************************/

int MUL_DEV_open(struct inode *inode, struct file *file)
{
    INFO("MUL_DEV_open(..)");
    return 0;
}


ssize_t MUL_DEV_write(struct file *__file, const char *__ubuf, size_t __nbytes, loff_t *offp)
{
    int result, bytes_to_write;
    ssize_t retval;

    INFO("MUL_DEV_write(fp=%p,ubuf=[%s],nbytes=%d)", __file, __ubuf, __nbytes);

    memset(kbuf, '\0', KBUF_LEN); //first clear the kernel buffer

    //check if the bytes to be write exceed the value present int he device
    bytes_to_write = __nbytes > KBUF_LEN ? KBUF_LEN : __nbytes;

    result = copy_from_user(kbuf, __ubuf, bytes_to_write);

    if (result == 0)
    {
        INFO("%d bytes of data successfully written", bytes_to_write);
        INFO("Message received from User = %s", kbuf);
        kstrtoint(kbuf, 10, &operands[num_idx]);
        INFO("operands[%d] = %d", num_idx, operands[num_idx]);
        num_idx = (num_idx + 1) % 2;
        retval = bytes_to_write;
    }
    else
    {
        ERR("Error writing data");
        retval = -EFAULT;
    }
    INFO("MUL_DEV_write() exit");
    return retval;
}


ssize_t MUL_DEV_read(struct file *__file, char *__ubuf, size_t __nbytes, loff_t *offp)
{
    int result, oper_result;
    ssize_t retval;
    char oper_result_string[30] = {'\0'};

    INFO("MUL_DEV_read(fp=%p,ubuf=[%s],nbytes=%d)", __file, __ubuf, __nbytes);

    oper_result = operands[0] * operands[1];

    result = sprintf(oper_result_string, "%d", oper_result);

    if (result == 0) //if could not convert the number to string
    {
        retval = -EFAULT;
        goto ret;
    }

    INFO("Operation Result = %s", oper_result_string);

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
    INFO("MUL_DEV_read() exit");
    return retval;
}

int MUL_DEV_release(struct inode *inode, struct file *file)
{
    INFO("MUL_DEV_release(..)");
    return 0;
}

/*****************************************************************************/

/******************************SUB DEV****************************************/

int SUB_DEV_open(struct inode *inode, struct file *file)
{
    INFO("SUB_DEV_open(..)");
    return 0;
}


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
        INFO("operands[%d] = %d", num_idx, operands[num_idx]);
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