#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/kthread.h>

struct procinfo
{
	pid_t pid;	/*process's id*/
	pid_t ppid; /*parent id*/
	struct timespec start_time; /*process start time*/
	int num_sib; /*number of siblings*/
};

void printTime(int seconds)
{
	printk(KERN_CRIT "\tStart Time: %d:%d:%d\n", seconds/3600, seconds/60, seconds);
}

void printProcinfo(struct procinfo *info)
{
	printk(KERN_CRIT "Process Information:\n");
	printk(KERN_CRIT "\tPID: %d\n", info->pid);
	printk(KERN_CRIT "\tPPID: %d\n", info->ppid);
	printk(KERN_CRIT "\tNumber of Siblings: %d\n", info->num_sib);
	printTime(info->start_time.tv_sec);
}

/* Counts number of nodes in list*/
int countNodes(struct list_head head)
{
	int n = 0;
/*
	struct list_head *current;
	list_for_each(current, &head)
		n++;
*/
	return n;
}

/*Populates a procinfo with information from a task_struct*/
void populate_proc_info(struct task_struct *ts, struct procinfo *info)
{
	info->pid = ts->pid;
	info->ppid = ts->real_parent->pid;
	info->start_time = ts->start_time;
	printTime(info->start_time.tv_sec);
	info->num_sib = countNodes(ts->sibling);
}

/*Retrieves information about a PID and stores it in a procinfo*/
asmlinkage long sys_get_procinfo(pid_t pid, struct procinfo *info)
{
	struct task_struct *ts;
	pid_t orig_pid = pid;
	if(pid <= 0)  
		pid = task_tgid_vnr(current);

	rcu_read_lock();
	ts = find_task_by_vpid(pid);
	rcu_read_unlock();

	if(ts == NULL)
		return -1;
	else if(orig_pid < 0)
	{
		rcu_read_lock();
		ts = ts->real_parent;
		rcu_read_unlock();
	}
	printk(KERN_CRIT "Found PID. Populating struct...");

	populate_proc_info(ts, info);
	printProcinfo(info);
	return 154;
}

