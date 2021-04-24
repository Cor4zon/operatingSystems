#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init_task.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FEDOROV V.");

static int __init md_init(void){
    printk(":) md init");
    struct task_struct *task = &init_task;

    do {
	    printk(":) ---%s-%d, parent %s-%d", task->comm,
		task->pid, task->parent->comm, task->parent->pid);
    } while ((task = next_task(task)) != &init_task);

    printk(":) ---%s-%d, parent %s-%d", current->comm,
       current->pid, current->parent->comm, current->parent->pid);
    return 0;
}

static void __exit md_exit(void){
    printk(":) md unloaded");
}

module_init(md_init);
module_exit(md_exit);
