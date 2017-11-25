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
static processCount=0;

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
		printf("프로세스 PID : %d\n", atoi(dent->d_name));
		openPsinfo(atoi(dent->d_name));
	}
	printf("프로세스의 개수 : %d\n", processCount);

    if(closedir(dp) < 0) {
		printf("ERROR: Could not close directory(%s).\n", dirName);
		exit(10);
	}

	exit(0);
}

void openPsinfo(int pid)
{
	int fd;
	char fileName[1024]; //proc/PID/각종 정보파일
	char buffer[512];
	psinfo_t data;
	int lwp=0;

	processCount++;

	memset(&data, 0, sizeof(psinfo_t));
	sprintf(fileName, "/proc/%d/psinfo", pid);
	
	if((fd = open(fileName, O_RDONLY)) < 0) 
	{
		printf("ERROR: %s open failed\n", fileName);
	}
	read(fd, &data, sizeof(psinfo_t));

	lwp = data.pr_nlwp;
	printf("프로세스의 활성 LWP 수 : %d\n", lwp);
	close(fd);
}
