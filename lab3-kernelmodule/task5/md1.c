#include <linux/init.h>
#include <linux/module.h>
#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor Fedorov");

int number_for_export = 100;
char* string_for_export = "I like operating systems";



extern char* get_string(int number){
    if (number == 1){
	return "you chose first action";
    }
    else {
        if (number == 2) {
	    return "you chose second action";
	}
	else {
	    return "you chose strange action";
	}
    }
}

extern int factorial(int number){
    int res = 1;
    int i = 0;
    while (i < number){
        res *= (i + 1);
	i++;
    }
    return res;
} 

extern int get_zero(void){
    return 0;
}

EXPORT_SYMBOL(number_for_export);
EXPORT_SYMBOL(string_for_export);
EXPORT_SYMBOL(get_string);
EXPORT_SYMBOL(factorial);
EXPORT_SYMBOL(get_zero);

static int __init md_init(void){
    printk(":) md1 start");
    return 0;
}

static void __exit md_exit(void){
    printk(":) md1 unloaded");
}

module_init(md_init);
module_exit(md_exit);
