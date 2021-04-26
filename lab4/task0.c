#include <linux/module.h>
#include <moduleparam.h>
#include <init.h>
#include <kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("V. Fedorov");

static struct proc_dir_entry *ent;

static ssize_t mywrite(){
    printk(KERN_DEBUG "write handler\n");
    return -1;
}

static ssize_t myread(){
    printk(KERN_DEBUG "read handler\n");
    return 0;
}

static struct file_operations myops = {
	.owner = THIS_MODULE,
	.read = myread,
	.write = mywrite,
};

static int simple_init(void){
	ent = proc_create("schizzo", 0660, NULL, &myops);
	return 0;
}

static void simple_cleanup(void){
	proc_remove(ent);
}

module_init(simple_init);
module_exit(simple_cleanup);