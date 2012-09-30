/*
	Name: 		Chad Wyszynski
	Assignment: Implementing a system call
*/
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

/*
void printTime(int seconds)
{
	int hours = seconds / 3600;
	seconds -= 3600*hours;
	int minutes = seconds/60;
	seconds -= 60*minutes;
	printk(KERN_CRIT "\tStart Time: %d:%d:%d\n", hours, minutes, seconds);
}

void printProcinfo(struct procinfo *info)
{
	printk(KERN_CRIT "Process Information:\n");
	printk(KERN_CRIT "\tPID: %d\n", info->pid);
	printk(KERN_CRIT "\tPPID: %d\n", info->ppid);
	printk(KERN_CRIT "\tNumber of Siblings: %d\n", info->num_sib);
	printTime(info->start_time.tv_sec);
}
*/

/* Counts number of nodes in list*/
int countNodes(struct list_head *head)
{
	int n = 0;
	struct list_head *pos;
	list_for_each(pos, head)
		n++;
	return n;
}

/*Populates a procinfo with information from a task_struct*/
void populate_proc_info(struct task_struct *ts, struct procinfo *info)
{
	info->pid = ts->pid;
	info->ppid = ts->real_parent->pid;
	info->start_time = ts->start_time;
	info->num_sib = countNodes(&ts->sibling);
}

/*
Retrieves information about a PID and stores it in a procinfo struct
Returns 0 on success; -1 if pid not found
*/
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

	populate_proc_info(ts, info);
	/*printProcinfo(info);*/
	return 0;
}

