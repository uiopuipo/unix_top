#include "stdhdr.h"

int main(void) {
	int pid;
	struct top data[MAX_PID];
	char str[20];
	char optionChoice;

	DIR *dp;
    char dirName[] = "/proc";

	dp = opendir(dirName); // proc 디렉토리를 연다.
    if(dp == NULL) {
		printf("ERROR: Could not open directory(%s).\n", dirName);
		exit(1);
	}

	system("clear");

	while(1){
		printf("Top Program \n");
		PrintPsInfo(dp, data);
		optionChoice = getc(stdin); //option 입력 시 optionChoice에 값을 넣음

		//옵션처리
		switch(optionChoice) {
			case 'k': //프로세스 종료 옵션
				OptKill();
				break;
			case 's': //정렬 옵션
				break;
			case 'f': //검색 옵션
				printf("검색할 항목을 입력하세요(pid, command) : ");
				scanf("%s", str);
				SearchData(str, data);
				break;
			case 'q': //q를 누르면 프로그램 종료
				system("clear");
				 if(closedir(dp) < 0) {
					printf("ERROR: Could not close directory(%s).\n", dirName);
					exit(2);
				}
				exit(1);
				break;
			case 'r': //r키 누르면 새로고침
				PrintPsInfo(dp, data);
				break;
			default :
				break;
		}
	}

    if(closedir(dp) < 0) {
		printf("ERROR: Could not close directory(%s).\n", dirName);
		exit(2);
	}

	return 0;
}

void PrintData(int pid, topData *data)
{
	printf("PID:%d ", data[pid].pid);
	printf("LWP:%d COMMAND:%s ", data[pid].lwp, data[pid].command);
	printf("SIZE:%d RES:%d ", data[pid].size, data[pid].res);
	printf("time: %s\n", data[pid].time);
}

void PrintPsInfo(DIR *dp, topData *data)
{
	int pid;
	struct dirent *dent;

	processCount = 0;

	while((dent=readdir(dp))) //디렉토리 정보를 읽어 dent가 가리키게 한다.
	{
		pid = atoi(dent->d_name);
		data[pid] = OpenPsinfo(pid);
		PrintData(pid, data);
	}
	printf("프로세스의 개수 : %d\n", processCount);
	rewinddir(dp);
}

void SearchData(char *str, topData *data)
{
	int i = 0;
	int tf = -1;
	char cmp[2][20] = {"pid", "command"};
	int value;
	char strValue[20];

	if( !strcmp(str, cmp[0]) )
		tf = 0;
	if( !strcmp(str, cmp[1]) )
		tf = 1;

	switch(tf){
		case 0:
			printf("검색할 PID를 입력하세요 : ");
			scanf("%d", &value);
			break;
		case 1:
			printf("검색할 COMMAND를 입력하세요 : ");
			scanf("%s", strValue);
			break;
	}

	while(i < MAX_PID)
	{
		switch(tf){
			case 0:
				if(data[i].pid == value)
					PrintData(data[i].pid, data);
				break;
			case 1:
				if( !strcmp(data[i].command, strValue) )
					PrintData(data[i].pid, data);
				break;
		}
		i++;
	}
}

topData OpenPsinfo(int pid)
{
	int fd;
	char fileName[1024]; //proc/PID/각종 정보파일
	char buffer[512];
	psinfo_t data;
	topData t_data;

	int lwp=0; //데이터를 꺼내 저장할 변수들
	char *command;
	int size, res;
	char *dmodel;
	timestruc_t time;
	int hour, min;
	char t_result[8];

	processCount++;

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
	if(min < 10)
		sprintf(t_result, "%d:0%d", hour, min);
	else 
		sprintf(t_result, "%d:%d", hour, min);

	t_data.pid = pid;
	t_data.lwp = (int)lwp;
	t_data.size = (int)size;
	t_data.res = (int)res;
	strcpy(t_data.command, command);
	strcpy(t_data.time, t_result);
	close(fd);

	return t_data;
}
