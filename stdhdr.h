#ifndef _TOP_
#define _TOP_
#include <dirent.h> 
#include <errno.h> 
#include <fcntl.h>
#include <procfs.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <sys/resource.h>

//define
//OptSort()함수의flag 
#define SORT_SIZE 1 //size별로 정렬할 때
#define SORT_PID 2 //pid별로 정렬할 때
#define SORT_RES 3 //res별로 정렬할 때

//PrintPsInfo()함수의flag 
#define FRONT_PAGE 1 //이후 페이지로 넘어갈 때 
#define BACK_PAGE 2 //이전 페이지로 넘길 때
#define INIT_PAGE 3 //업데이트 할 시 초기화
#define CURRENT_PAGE 4 //현재 페이지를 출력하고 싶을 때
#define MAX_SHOW_PROCESS 15 //한 페이지에 보여줄 프로세스의 개수

//data배열에 받아올 프로세스의 최대 개수
#define MAX_DATA_SIZE 400

//화면에 보여줄 top 구조체 요소.
typedef struct top {
	int pid; //프로세스 id
	int lwp; //쓰레드 갯수
	char command[64]; //명렁어
	char time[8]; //시간
	int size; //프로세스에 할당된 총 메모리의 양
	int res; //프로세스에 의해 사용된 물리 메모리의 양
	int nice;
	double percpu;
	double permem;
	char state;
	char username[64]; //사용자 이름


	
} topData;

//funciton
void SearchData(char *str, topData *data); //특정 프로세스 정보를 찾기위한 함수.
void PrintPsInfo(topData *data, int flag); //프로세스 정보를 출력한다.
void PrintMainInfo(); //top 프로그램의 기본 정보 및 주요내용 출력
void GetPsInfo(topData *data); //data에 프로세스 정보 받아오기.
struct top OpenPsinfo(int pid); //process의 정보를 열기.
void ClearReadBuffer(); //버퍼를 비우기 위한 함수.
void InitData(topData *data); //Data의 배열을 초기화 하기위한 함수
int GetDataSize(topData *data); //프로세스 개수 구하는 함수
void PrintHelpInfo(); //Top 사용법과 옵션 설명
void OptKill(); //특정 프로세를 종료시키기 위한 함수
int OptSort(topData *data,int size, int flag);//정렬하기위한 옵션 함수
#endif
