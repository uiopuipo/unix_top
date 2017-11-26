#include "stdhdr.h"



struct top OpenPsinfo(int pid);
static processCount=0;
void PrintPsInfo(DIR *dp, topData *data);
int main(void) {
	int pid;
	struct top data[MAX_PID];
	DIR *dp;
    char dirName[] = "/proc";

	char optionChoice;
	dp = opendir(dirName); // /proc ���丮�� ���� 
    if(dp == NULL) {
		printf("ERROR: Could not open directory(%s).\n", dirName);
		exit(1);
	}

	system("clear");
	while(1){

		printf("Top Program \n");
		
		PrintPsInfo(dp, data);
		printf("���μ����� ���� : %d\n", processCount);

		optionChoice = getc(stdin); //option �Է� �� optionChoice�� ���� ����


		//�ɼ�ó��
		switch(optionChoice) {
			case 'k': //���μ��� ���� �ɼ�
				OptKill();
				break;
			case 's': //���� �ɼ�
				break;
			case 'q': //q�� ������ ���α׷� ����
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
<<<<<<< Updated upstream

void PrintPsInfo(DIR *dp, topData *data){
=======
void PrintPsInfo(DIR **dp){
>>>>>>> Stashed changes
	int pid;
	struct dirent *dent;
	printf("ss");
	while((dent=readdir(dp))) //���丮 ������ �о� dent�� ����Ű�� �Ѵ�.
	{
		pid = atoi(dent->d_name);
		data[pid] = OpenPsinfo(pid);
		printf("PID:%d ", data[pid].pid);
		printf("LWP:%d COMMAND:%s ", data[pid].lwp, data[pid].command);
		printf("SIZE:%d RES:%d ", data[pid].size, data[pid].res);
		printf("time: %s\n", data[pid].time);
	}
}

topData OpenPsinfo(int pid)
{
	int fd;
	char fileName[1024]; //proc/PID/���� ��������
	char buffer[512];
	psinfo_t data;
	topData t_data;

	int lwp=0; //�����͸� ���� ������ ������
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
