#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor Fedorov");  // only for information target
MODULE_VERSION("1.0");

static int __init md_init( void )
{
    printk("Hello world\n");
    return 0;
}

static void __exit md_exit( void )
{
    printk("Good by");
}

module_init(md_init);
module_exit(md_exit);

