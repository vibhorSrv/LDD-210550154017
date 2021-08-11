#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vibhor");
MODULE_DESCRIPTION("Vibhor add module");

static int myadd(int a, int b)
{
    return a + b;
}
EXPORT_SYMBOL_GPL(myadd);

static int __init add_module_init(void)
{
    printk(KERN_ALERT "ADD module initialized");
    return 0;
}

static void __exit add_module_exit(void)
{
    printk(KERN_ALERT "ADD module sayonara");
}

module_init(add_module_init);
module_exit(add_module_exit);
