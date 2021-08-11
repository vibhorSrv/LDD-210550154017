#include <linux/module.h>
#include <linux/init.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vibhor");
MODULE_DESCRIPTION("module param demo");

static int roll = 10;
static char *name = "name";

module_param(roll, int, S_IRUGO);
module_param(name, charp, S_IRUGO);

static int __init my_module_param_init(void)
{
    printk("Hello from Module Param module param roll=%d, name = %s", roll, name);
    return 0;
}

static void __exit my_module_param_exit(void)
{
    printk("Bye bye from module!");
}

module_init(my_module_param_init);
module_exit(my_module_param_exit);