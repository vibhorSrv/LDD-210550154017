#include <linux/init.h>
#include <linux/module.h>
#include "add.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vibhor");
MODULE_DESCRIPTION("Module Description");

static int a = 1;
static int b = 3;

static int __init myavginit(void)
{
    printk(KERN_ALERT "Avg module Init Sum = %d", (float)myadd(a, b)/2);
    return 0;
}

static void __exit myavgexit(void)
{
    printk(KERN_ALERT "Avg module sayonara");
}

module_init(myavginit);
module_exit(myavgexit);