#include <dirent.h> 
#include <unistd.h>
#include <errno.h> 
#include <fcntl.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <signal.h>
#include <procfs.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <signal.h>

//define
//OptSort()함수의 signal
#define SORT_SIZE 1 //size별로 정렬할 때
#define SORT_PID 2 //pid별로 정렬할 때
#define SORT_RES 3 //res별로 정렬할 때

//PrintPsInfo()함수의 signal
#define FRONT_PAGE 1 //이후 페이지로 넘어갈 때 
#define BACK_PAGE 2 //이전 페이지로 넘길 때
#define INIT_PAGE 3 //업데이트 할 시 초기화
#define CURRENT_PAGE 4 //현재 페이지를 출력하고 싶을 때

//받아올 프로세스 한정치
#define MAX_DATA_SIZE 400

//화면에 보여줄 top 구조체 요소.
typedef struct top{
	int pid; //프로세스 id
	int lwp; //
	char command[64]; //명렁어
	char time[8]; //시간
	int size; //프로세스 size
	int res;
	int mem;
} topData;

static int processCount=0; //data에 가져온 프로세스 개수

//funciton
void PrintData(int pid, topData *data); //process 값을 출력해줌.
void SearchData(char *str, topData *data); //특정 프로세스 정보를 찾기위한 함수.
void PrintPsInfo(topData *data, int signal); //프로세스 정보를 출력한다.
void PrintMainInfo(); //top 프로그램의 기본 정보 및 주요내용 출력
void GetPsInfo(topData *data); //data에 프로세스 정보 받아오기.
struct top OpenPsinfo(int pid); //process의 정보를 열기.
void ChangePos(topData *a, topData *b); //구조체 바꾸기
void ClearReadBuffer(); //버퍼를 비우기 위한 함수.
void InitData(topData *data); //Data의 배열을 초기화 하기위한 함수
void GetDataSize(topData *data); //지울수도 있음
int Getch(void); //지울수도 있음 
void PrintHelpInfo(); //Top 사용법과 옵션 설명

//option fucntion
void OptKill(); //특정 프로세를 종료시키기 위한 함수
int OptSort(topData *data,int size, int flag);


