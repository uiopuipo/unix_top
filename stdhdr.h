#include <dirent.h> 
#include <errno.h> 
#include <fcntl.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <signal.h>
#include <procfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <signal.h>

//OptSort()함수의 signal
#define SORT_SIZE 1
#define SORT_PID 2
#define SORT_RES 3

#define MAX_PID 65535
//화면에 보여줄 top 구조체 요소.
typedef struct top{
	int pid;
	int lwp;
	char command[64];
	char time[8];
	int size;
	int res;
} topData;

///////define


///////funciton
struct top OpenPsinfo(int);
void PrintData(int pid, topData *data);
void SearchData(char *str, topData *data);
void PrintPsInfo(DIR *dp, topData *data);
//option fucntion
void OptKill();

int OptSort(topData *data, int flag);

static processCount=0;

