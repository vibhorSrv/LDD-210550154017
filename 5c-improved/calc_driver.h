#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEV_NAME_ADD "my_add_dev"
#define DEV_NAME_SUB "my_sub_dev"
#define DEV_NAME_MUL "my_mul_dev"
#define DEV_NAME_DIV "my_div_dev"

#define DEV_MAJ 255
#define DEV_MIN_ADD 5
#define DEV_MIN_SUB 6
#define DEV_MIN_MUL 7
#define DEV_MIN_DIV 8

#define KBUF_LEN 30

#define ERR(...) {printk(KERN_ERR "chardriver_module: "__VA_ARGS__);}
#define INFO(...) {printk(KERN_INFO "chardriver_module: "__VA_ARGS__);}

/**add device function prototypes**/
int ADD_DEV_open(struct inode *inode, struct file *file);
int ADD_DEV_release(struct inode *inode, struct file *file);
ssize_t ADD_DEV_write(struct file *__file, const char *__buf, size_t __nbytes, loff_t *offp);
ssize_t ADD_DEV_read(struct file *__file, char *__buf, size_t __nbytes, loff_t *offp);

/**sub device function prototypes**/
int SUB_DEV_open(struct inode *inode, struct file *file);
int SUB_DEV_release(struct inode *inode, struct file *file);
ssize_t SUB_DEV_write(struct file *__file, const char *__buf, size_t __nbytes, loff_t *offp);
ssize_t SUB_DEV_read(struct file *__file, char *__buf, size_t __nbytes, loff_t *offp);

/**mul device function prototypes**/
int MUL_DEV_open(struct inode *inode, struct file *file);
int MUL_DEV_release(struct inode *inode, struct file *file);
ssize_t MUL_DEV_write(struct file *__file, const char *__buf, size_t __nbytes, loff_t *offp);
ssize_t MUL_DEV_read(struct file *__file, char *__buf, size_t __nbytes, loff_t *offp);

/**div device function prototypes**/
int DIV_DEV_open(struct inode *inode, struct file *file);
int DIV_DEV_release(struct inode *inode, struct file *file);
ssize_t DIV_DEV_write(struct file *__file, const char *__buf, size_t __nbytes, loff_t *offp);
ssize_t DIV_DEV_read(struct file *__file, char *__buf, size_t __nbytes, loff_t *offp);

int Register_New_Device(dev_t *_devt, char *_dev_name, int _maj, int _min);
int Register_Cdev(dev_t *_devt, struct cdev **_cdev, struct file_operations *_fops);
void Unregister_Device(int _maj, int _min, struct cdev *_cdev);