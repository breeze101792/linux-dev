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
#include <linux/seq_file.h>
//#define PAGE_SHIFT      12
//#define PAGE_SIZE       (_AC(1,UL) << PAGE_SHIFT)
//#define PAGE_MASK       (~(PAGE_SIZE-1))
void vma_read(struct vm_area_struct *vma, struct task_struct *tk)
{
	int is_pid = 1;
	struct mm_struct *mm = vma->vm_mm;
	struct file *file = vma->vm_file;
	//struct proc_maps_private *priv = m->private;
	struct task_struct *task = tk;
	vm_flags_t flags = vma->vm_flags;
	unsigned long ino = 0;
	unsigned long long pgoff = 0;
	unsigned long start, end;
	dev_t dev = 0;
	int len;
	const char *name = NULL;

	if (file) {
		struct inode *inode = file_inode(vma->vm_file);
		dev = inode->i_sb->s_dev;
		ino = inode->i_ino;
		pgoff = ((loff_t)vma->vm_pgoff) << PAGE_SHIFT;
	}

	/* We don't show the stack guard page in /proc/maps */
	start = vma->vm_start;
	if (stack_guard_page_start(vma, start))
		start += PAGE_SIZE;
	end = vma->vm_end;
	if (stack_guard_page_end(vma, end))
		end -= PAGE_SIZE;

	printk("%08lx-%08lx %c%c%c%c %08llx %02x:%02x %lu %n",
			start,
			end,
			flags & VM_READ ? 'r' : '-',
			flags & VM_WRITE ? 'w' : '-',
			flags & VM_EXEC ? 'x' : '-',
			flags & VM_MAYSHARE ? 's' : 'p',
			pgoff,
			MAJOR(dev), MINOR(dev), ino, &len);

	/*
	 * Print the dentry name for named mappings, and a
	 * special [heap] marker for the heap:
	 */

	if (file) {
		//pad_len_spaces(m, len);
		//seq_path(m, &file->f_path, "\n");
		//size_t size = 20;
		//char *buf = (char *)kmalloc(sizeof(char) * size, GFP_KERNEL);
		//get_path_name(&file->f_path, buf, size);
		
		printk("%s", vma->vm_file->f_dentry->d_name.name );
		//kfree(buf);
		goto done;
	}


	name = arch_vma_name(vma);
	if (!name) {
		pid_t tid;

		if (!mm) {
			name = "[vdso]";
			goto done;
		}

		if (vma->vm_start <= mm->brk &&
		    vma->vm_end >= mm->start_brk) {
			name = "[heap]";
			goto done;
		}

		tid = vm_is_stack(task, vma, is_pid);

		if (tid != 0) {
			/*
			 * Thread stack in /proc/PID/task/TID/maps or
			 * the main process stack.
			 */
			if (is_pid || (vma->vm_start <= mm->start_stack &&
			    vma->vm_end >= mm->start_stack)) {
				name = "[stack]";
			} else {
				/* Thread stack in /proc/PID/maps */
				//pad_len_spaces(m, len);
				printk("[stack:%d]", tid);
			}
		}
	}

done:
	if (name) {
		//pad_len_spaces(m, len);
		//seq_puts(m, name);
		printk(name);
	}
	//seq_putc(m, '\n');
	printk("\n");
}
asmlinkage long sys_linux_survey_TT(int pid, char* mem_data) {
        printk(KERN_EMERG "memory analyze!");

	//find task_struck by
	//struct task_struct *tk = pid_task(find_vpid(pid), PIDTYPE_PID);
	struct task_struct *tk = pid_task(find_get_pid(pid), PIDTYPE_PID);
	printk(KERN_EMERG "find task_struct");
	if (!tk) {
            printk ("Error: pid does not exists!!\n");
            return 1;
        }
	if (!tk->mm) {
            printk ("Error: mm is Null point!!\n");
            return 2;
	}
	struct mm_struct *mms = tk->mm;
	if (!mms) {
            printk ("Error: mm_struct *mm does not exist!!\n");
            return 4;
        }
	struct vm_area_struct *vma;
	//find task by virtual pid!
	//struct mm_struct *mm = find_task_by_vpid(pid)->mm;
	//struct pgd_t pgd;
	struct page* page;
	long pgframe_addr, vm_address;
	for (vma = mms->mmap;vma;vma = vma->vm_next)
	{
		vma_read(vma, tk);
		for (vm_address = vma->vm_start;vm_address < vma->vm_end;vm_address += 0x1000)
		{
			/*
			pgd = pgd_offset(mm, address);
			pud = pud_offset(pgd, address);
			pmd = pmd_offset(pud, address);
			*/
			page = follow_page(vma, vm_address, 0);
			if (page == NULL) continue;
			pgframe_addr = page_to_phys(page);
			printk("0x%x ", pgframe_addr);
		}
	}
        return 0;
}
