#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage long sys_linux_survey_TT() {
        printk(KERN_EMERG "memory analyze!");

        return 1;
}
