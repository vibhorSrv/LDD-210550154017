#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VIBHOR");
MODULE_DESCRIPTION("MODULE TO ADD SUBTRACT ETC");
static char *oper;
static int num1;
static int num2;
static int res;
module_param(oper, charp, S_IRUGO);
module_param(num1, int, S_IRUGO);
module_param(num2, int, S_IRUGO);

static int add(int num1, int num2)
{
    return num1 + num2;
}

static int sub(int num1, int num2)
{
    return num1 - num2;
}
static int mul(int num1, int num2)
{
    return num1 * num2;
}
static int div(int num1, int num2)
{
    if (num2 != 0)
        return num1 / num2;
    else
        return 0;
}

static int __init my_init(void)
{
    printk("Module init with oper =  %s", oper);
    printk("\nnum1 =  %d, num2 = %d", num1, num2);
    if (!strcmp("add", oper))
        res = add(num1, num2);
    else if (!strcmp("sub", oper))
        res = sub(num1, num2);
    else if (!strcmp("div", oper))
        res = div(num1, num2);
    else if (!strcmp("mul", oper))
        res = mul(num1, num2);
    else
    {
    }
    printk("Result = %d", res);

    return 0;
}

static void __exit my_exit(void)
{
    printk("My work here is done");
}

module_init(my_init);
module_exit(my_exit);