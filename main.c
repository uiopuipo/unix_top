#include "stdhdr.h"


struct top OpenPsinfo(int pid);
static processCount=0;
void PrintPsInfo(DIR *dp, topData *data);
int getch(void);
int main(void) {
	int pid,men;
	struct top data[MAX_PID];
	DIR *dp;
    char dirName[] = "/proc";
	char str[20];
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
			//	OptKill();
				break;
			case 's': //���� �ɼ�
				break;
			case 'f': //�˻� �ɼ�
				printf("�˻��� �׸��� �Է��ϼ���(pid, command) : ");
				scanf("%s", str);
				SearchData(str, data);
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

void PrintData(int pid, topData *data)
{
	printf("%-4d | PID : %-7d| LWP %-4d| SIZE : %-7d| RES : %-7d| TIME : %-7s| MEM : %4d| COMMAND : %-10s\n ", processCount ,data[pid].pid, data[pid].lwp, data[pid].size, data[pid].res, data[pid].time, data[pid].mem, data[pid].command);
	printf("---------------------------------------------------------------------------------------------------------------\n");
}

void PrintPsInfo(DIR *dp, topData *data)
{
	int pid;
	struct dirent *dent;
	printf("ss\n");
	while((dent=readdir(dp))) //���丮 ������ �о� dent�� ����Ű�� �Ѵ�.
	{
			
		pid = atoi(dent->d_name);
		data[pid] = OpenPsinfo(pid);
		processCount++;
		PrintData(pid, data);
		if(processCount%30==0){
			printf("���� 30�� ���μ��� �ƹ�Ű�� �����ʽÿ�...\n");
			getch();
			system("clear");
		} continue;
}
}

int getch(void){
	int ch;
	struct termios buf, save;
	tcgetattr(0,&save);
	buf = save;
	buf.c_lflag &= ~(ICANON|ECHO);
	buf.c_cc[VMIN]=1;
	buf.c_cc[VTIME]=0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch=getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
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
			printf("�˻��� PID�� �Է��ϼ��� : ");
			scanf("%d", &value);
			break;
		case 1:
			printf("�˻��� COMMAND�� �Է��ϼ��� : ");
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
	char fileName[1024]; //proc/PID/���� ��������
	char buffer[512];
	psinfo_t data;
	topData t_data;

	int lwp=0; //�����͸� ���� ������ ������
	char *command;
	int size, res, mem;
	char *dmodel;
	timestruc_t time;
	int hour, min;
	char t_result[8];

	
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
	mem = data.pr_pctmem;
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
	t_data.mem = (int)mem;
	strcpy(t_data.command, command);
	strcpy(t_data.time, t_result);
	close(fd);

	return t_data;
}
