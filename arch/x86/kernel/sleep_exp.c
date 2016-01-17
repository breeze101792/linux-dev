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
asmlinkage long sys_wake_up_my_process(int pid) {
	struct task_struct *tk = pid_task(find_get_pid(pid), PIDTYPE_PID);
	printk("wake_up_my_process");
	if (!queue_flag)
	{
		printk("wake up");
		set_tsk_thread_flag(tk,TIF_SIGPENDING);
		wake_up_interruptible(&wq);
	}
	else
	{
		printk("no wait queue define");
	}
        return 0;
}
