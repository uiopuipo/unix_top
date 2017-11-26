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


//화면에 보여줄 top 구조체 요소.
typedef struct top{
	int pid;
	int lwp;
	char command[64];
	char time[8];
	int size;
	int res;
} topData;

//링크리스트 노드
typedef struct node {
	topData element;
	struct topNode *link;
} topNode;
