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

struct top openPsinfo(int pid);

typedef struct top{
	int pid;
	int lwp;
	char command[64];
	int t_hour;
	int t_min;
	int size;
	int res;
}top_data;

int main(void) {
	int pid;
	struct top data[65535];
	DIR *dp;
	struct dirent *dent;
    char dirName[] = "/proc";

	dp = opendir(dirName);
    if(dp == NULL) {
		printf("ERROR: Could not open directory(%s).\n", dirName);
		exit(1);
	}

	while((dent=readdir(dp)))
	{
		pid = atoi(dent->d_name);
		data[pid] = openPsinfo(pid);
		printf("PID:%d ", data[pid].pid);
		printf("LWP:%d COMMAND:%s ", data[pid].lwp, data[pid].command);
		printf("SIZE:%d RES:%d ", data[pid].size, data[pid].res);
		printf("time: %d:%d\n", data[pid].t_hour, data[pid].t_min);
	}

    if(closedir(dp) < 0) {
		printf("ERROR: Could not close directory(%s).\n", dirName);
		exit(2);
	}

	return 0;
}

top_data openPsinfo(int pid)
{
	int fd;
	char fileName[1024]; //proc/PID/���� ��������
	char buffer[512];
	psinfo_t data;
	top_data t_data;

	int lwp=0; //�����͸� ���� ������ ������
	char *command;
	int size, res;
	char *dmodel;
	timestruc_t time;
	int hour, min;

	memset(&data, 0, sizeof(psinfo_t));
	sprintf(fileName, "/proc/%d/psinfo", pid);
	if((fd = open(fileName, O_RDONLY)) < 0) 
	{
		printf("ERROR: %s open failed\n", fileName);
	}
	read(fd, &data, sizeof(psinfo_t));

	lwp = data.pr_nlwp;
	command = data.pr_fname;
	size = data.pr_size;
	res = data.pr_rssize;

	time = data.pr_time;
	hour = time.tv_sec / 60;
	min = time.tv_sec % 60;

	t_data.pid = pid;
	t_data.lwp = (int)lwp;
	t_data.size = (int)size;
	t_data.res = (int)res;
	t_data.t_hour = (int)hour;
	t_data.t_min = (int)min;
	strcpy(t_data.command, command);
	close(fd);

	return t_data;
}
