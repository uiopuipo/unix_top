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
//OptSort()�Լ���flag 
#define SORT_SIZE 1 //size���� ������ ��
#define SORT_PID 2 //pid���� ������ ��
#define SORT_RES 3 //res���� ������ ��

//PrintPsInfo()�Լ���flag 
#define FRONT_PAGE 1 //���� �������� �Ѿ �� 
#define BACK_PAGE 2 //���� �������� �ѱ� ��
#define INIT_PAGE 3 //������Ʈ �� �� �ʱ�ȭ
#define CURRENT_PAGE 4 //���� �������� ����ϰ� ���� ��
#define MAX_SHOW_PROCESS 15 //�� �������� ������ ���μ����� ����

//data�迭�� �޾ƿ� ���μ����� �ִ� ����
#define MAX_DATA_SIZE 400

//ȭ�鿡 ������ top ����ü ���.
typedef struct top {
	int pid; //���μ��� id
	int lwp; //������ ����
	char command[64]; //����
	char time[8]; //�ð�
	int size; //���μ����� �Ҵ�� �� �޸��� ��
	int res; //���μ����� ���� ���� ���� �޸��� ��
	int nice;
	double percpu;
	double permem;
	char state;
	char username[64]; //����� �̸�


	
} topData;

//funciton
void SearchData(char *str, topData *data); //Ư�� ���μ��� ������ ã������ �Լ�.
void PrintPsInfo(topData *data, int flag); //���μ��� ������ ����Ѵ�.
void PrintMainInfo(); //top ���α׷��� �⺻ ���� �� �ֿ䳻�� ���
void GetPsInfo(topData *data); //data�� ���μ��� ���� �޾ƿ���.
struct top OpenPsinfo(int pid); //process�� ������ ����.
void ClearReadBuffer(); //���۸� ���� ���� �Լ�.
void InitData(topData *data); //Data�� �迭�� �ʱ�ȭ �ϱ����� �Լ�
int GetDataSize(topData *data); //���μ��� ���� ���ϴ� �Լ�
void PrintHelpInfo(); //Top ������ �ɼ� ����
void OptKill(); //Ư�� ���μ��� �����Ű�� ���� �Լ�
int OptSort(topData *data,int size, int flag);//�����ϱ����� �ɼ� �Լ�
#endif
