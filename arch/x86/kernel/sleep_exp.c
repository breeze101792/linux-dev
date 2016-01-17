#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/sched.h>
static wait_queue_head_t wq;
static int queue_flag = 1;
	

asmlinkage long sys_go_to_sleep_now() {
	printk("go_to_sleep_now");
	int sleep_flag = 0;
	if(queue_flag)
	{
		queue_flag = 0;
		init_waitqueue_head(&wq);
		printk("wq define");
	}
	wait_event_interruptible(wq, sleep_flag);
	printk("finished!\n");
        return 0;
}
asmlinkage long sys_wake_up_my_process() {
	printk("wake_up_my_process");
        return 0;
}
