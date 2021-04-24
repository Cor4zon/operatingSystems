#include <linux/init.h>
#include <linux/module.h>
#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor Fedorov");

static int __init md_init(void){
    printk(":) md2 start");
    printk(":) export some data from md1:");

    printk(":) number_for_export: %d\n", number_for_export);
    printk(":) string_for_export: %s\n", string_for_export);
    printk(":) function get_zero: %d\n", get_zero());
    printk(":) function get_string(1): %s\n", get_string(1));
    printk(":) function get_string(2): %s\n", get_string(2));
    printk(":) function get_string(10): %s\n", get_string(10));
    printk(":) factorial(1): %d\n", factorial(1));
    printk(":) factorial(5): %d\n", factorial(5));
    printk(":) factorial(3): %d\n", factorial(3));
    return 0;
}

static void __exit md_exit(void){
    printk(":) md2 unloaded");
}

module_init(md_init);
module_exit(md_exit);
