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
#include <signal.h>

///////define
//OptSort()함수의 signal
#define SORT_SIZE 1
#define SORT_PID 2
#define SORT_RES 3
//받아올 프로세스 한정치
#define MAX_DATA_SIZE 400


//화면에 보여줄 top 구조체 요소.
typedef struct top{
	int pid;
	int lwp;
	char command[64];
	char time[8];
	int size;
	int res;
} topData;

//data에 가져온 프로세스 개수.
static processCount=0;

//funciton
struct top OpenPsinfo(int);
void PrintPsInfo(DIR *dp, topData *data);
void ChangePos(topData *a, topData *b); 
void ClearReadBuffer();
void InitData(topData *data);
void GetDataSize(topData *data);

//option fucntion
void OptKill();
int OptSort(topData *data,int size, int flag);


