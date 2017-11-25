#include <dirent.h> 
#include <errno.h> 
#include <fcntl.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <procfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/syscall.h>
#include <sys/uio.h>

void openPsinfo(int pid);
void openStatus(int pid);

int main(void) {
	int pid;
	DIR *dp;
	struct dirent *dent;
    char dirName[] = "/proc";

	dp = opendir(dirName);
    if(dp == NULL) {
		printf("ERROR: Could not open directory(%s).\n", dirName);
		exit(10);
	}

	while((dent=readdir(dp)))
	{
		pid = atoi(dent->d_name);
		printf("PID : %d ", pid);
		openPsinfo(pid);
	}

    if(closedir(dp) < 0) {
		printf("ERROR: Could not close directory(%s).\n", dirName);
		exit(10);
	}

	exit(0);
}

void openPsinfo(int pid)
{
	int fd;
	char fileName[1024]; //proc/PID/���� ��������
	char buffer[512];
	psinfo_t data;
	int lwp=0;
	char *command;
	int size, res;

	memset(&data, 0, sizeof(psinfo_t));
	sprintf(fileName, "/proc/%d/psinfo", pid);
	if((fd = open(fileName, O_RDONLY)) < 0) 
	{
		printf("ERROR: %s open failed\n", fileName);
	}
	read(fd, &data, sizeof(psinfo_t));

	lwp = data.pr_nlwp;
	command=data.pr_fname;
	size = data.pr_size;
	res = data.pr_rssize;

	printf("LWP : %d COMMAND : %s ", lwp, command);
	printf("SIZE : %d RES : %d\n", size, res);
	close(fd);
}
