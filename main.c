#include "stdhdr.h"



struct top OpenPsinfo(int pid);
void PrintPsInfo(DIR *dp, topData *data);
int main(void) {
	int pid;
	topData data[MAX_DATA_SIZE];
	DIR *dp;
    char dirName[] = "/proc";
	char optionChoice;
	dp = opendir(dirName); // /proc 디렉토리를 열어 
    if(dp == NULL) {
		printf("ERROR: Could not open directory(%s).\n", dirName);
		exit(1);
	}

	system("clear");
	InitData(data);
	while(1){

		printf("Top Program \n");
		
		PrintPsInfo(dp, data);

		optionChoice = fgetc(stdin); //option 입력 시 optionChoice에 값을 넣음
		ClearReadBuffer();
		


		//옵션처리
		switch(optionChoice) {
			case 'k': //프로세스 종료 옵션
				OptKill();
				break;
			case 's': //정렬 옵션
				{
					char choice;
					int size = processCount; //나중에 static없앨 수도 있어서 size 변수 선언
					printf("size : :%d\n", size);
					printf("sorting (size:'s', pid:'p', res:'r') : ");
					choice = getc(stdin);
					ClearReadBuffer();
					switch(choice) {
						case 's':
							printf("you choose s\n");
							if(OptSort(data, size, SORT_SIZE) < 0)
								printf("sort part error\n");
							break;
						case 'p':
							printf("you choose p\n");
							if(OptSort(data, size, SORT_PID) < 0)
								printf("sort part error\n");
							break;
						case 'r':
							printf("you choose r\n");
							if(OptSort(data, size, SORT_RES) < 0)
								printf("sort part error\n");
							break;
						default:
							break;
					}

				}
				break;
			case 'q': //q를 누르면 프로그램 종료
				system("clear");
				 if(closedir(dp) < 0) {
					printf("ERROR: Could not close directory(%s).\n", dirName);
					exit(2);
				}
				exit(1);
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

void PrintPsInfo(DIR *dp,topData *data){
	int pid;
	int count=0;
	struct dirent *dent;
	printf("ss");
	while((dent=readdir(dp))) //디렉토리 정보를 읽어 dent가 가리키게 한다.
	{
		pid = atoi(dent->d_name);
		data[count] = OpenPsinfo(pid);
		PrintProcess(data[count]);
		count++;
	}
	processCount = count; //프로세스 개수 설정
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
