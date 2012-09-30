#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYS_yfkm2_monitor __NR_yfkm2_monitor
#define SYS_yfkm2_notifyme __NR_yfkm2_notifyme
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
