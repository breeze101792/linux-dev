#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <linux/pid.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/slab.h>

asmlinkage long sys_go_to_sleep_now() {
	printk("go_to_sleep_now");
        return 0;
}
asmlinkage long sys_wake_up_my_process() {
	printk("wake_up_my_process");
        return 0;
}
