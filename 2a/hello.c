#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vibhor");
MODULE_DESCRIPTION("description");

static int hello_init(void)
{
	printk("Hello");
	return 0;
}

static void hello_exit(void)
{
	printk("Exit");
}

module_init(hello_init);
module_exit(hello_exit);

