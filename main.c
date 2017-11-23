#include <dirent.h> 
#include <errno.h> 
#include <fcntl.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <procfs.h>
#include <sys/stat.h>
#include <sys/types.h>

void openPsinfo(int pid);

int main(void) {
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
		printf("���μ��� PID : %d\n", atoi(dent->d_name));
		openPsinfo(atoi(dent->d_name));
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

	memset(&data, 0, sizeof(psinfo_t));
	sprintf(fileName, "/proc/%d/psinfo", pid);
	if((fd = open(fileName, O_RDONLY)) < 0) 
	{
		printf("ERROR: %s open failed\n", fileName);
	}
	read(fd, &data, sizeof(psinfo_t));

	lwp = data.pr_nlwp;
	printf("���μ����� Ȱ�� LWP �� : %d\n", lwp);
	close(fd);
}
