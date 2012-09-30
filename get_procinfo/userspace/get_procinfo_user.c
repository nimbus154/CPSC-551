#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

#define SYS_getproc __NR_getproc
struct procinfo
{
	pid_t pid;	/*process's id*/
	pid_t ppid; /*parent id*/
	struct timespec start_time; /*process start time*/
	int num_sib; /*number of siblings*/
};

void printTime(int seconds)
{
	int hours = seconds / 3600;
	seconds -= 3600*hours;
	int minutes = seconds/60;
	seconds -= 60*minutes;
	printf("\tStarted %02d:%02d:%02d after system boot\n", hours, minutes, seconds);
}

void printProcinfo(struct procinfo *info)
{
	printf("Process Information:\n");
	printf("\tPID: %d\n", info->pid);
	printf("\tPPID: %d\n", info->ppid);
	printf("\tNumber of Siblings: %d\n", info->num_sib);
	printTime(info->start_time.tv_sec);
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Useage:\n\tgetproc <pid>\n");
		printf("\tEnter 0 for current process "
				"and a negative value for parent of current.\n");
		return 1;
	}

	struct procinfo *info = malloc(sizeof(struct procinfo));
	
	if(syscall(SYS_getproc, atoi(argv[1]), info) != -1)
		printProcinfo(info);

	free(info);
	info = NULL;
	return 0;
}
