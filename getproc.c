#include<unistd.h>
#include<sys/syscall.h>
#include<linux/sched.h>
#include<include/asm/current.h>
#include<stdio.h>
#include<stdlib.h>

struct procinfo
{
	pid_t pid;	/*process's id*/
	pid_t ppid; /*parent id*/
	struct timespec start_time; /*process start time*/
	int num_sib; /*number of siblings*/
};

/* Counts number of nodes in list*/
int countNodes(list_head *head)
{
	int n = 0;
	while(head != NULL)
	{
		head = head->next;
		n++;
	}

	return n;
}

/*Populates a procinfo with information from a task_struct*/
void populate_proc_info(struct task_struct *ts, struct procinfo *info)
{
	info->pid = ts->pid;
	info->ppid = ts->real_parent->pid;	
	info->start_time = ts->start_time;
	info->num_sib = countNodes(ts->sibling);
}

/*Retrieves information about a PID and stores it in a procinfo*/
asmlinkage int get_procinfo(pid_t pid, struct procinfo *info)
{
	struct task_struct *ts;
	ts = pid <= 0 ?  get_current() : find_task_by_vpid(pid);

	if(ts == null)
		return EINVAL;
	else if(pid < 0)
		ts = ts->real_parent;

	populate_proc_info(ts, info);
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Useage:\n\tgetproc <pid>\n");
		return 1;
	}

	struct procinfo info;
	get_procinfo(atoi(argv[1]), &info);

	printf("PROCESS INFO:\n\tpid: %d\n", info.pid);
	return 0;
}
