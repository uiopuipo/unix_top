#include "stdhdr.h"
///////////////
//Option Part//
///////////////

void OptKill(){
	int targetPID; //�ñ׳��� ���� pid
	short flag; //���� ó���� ���� ����

	printf("PID to signal/kill [default pid = 1234] ");
	scanf("%d",&targetPID); //�ñ׳��� ���� pid�Է�
	ClearReadBuffer(); //���۸� ����

	if(flag = kill(targetPID, SIGKILL) < 0 ){//�Է��� pid�� ���μ����� ���δ�.
		printf("%d PID is not exist or you have to get permission \n",targetPID);
	} else { 
		printf("%d PID is deleted \n",targetPID);
	}
}

//���� �ϰ���� �迭, �迭�� ����, flag(���� ����)�� ���ڷ� �޴´�.
int OptSort(topData *data, int size, int flag){
	int i, j; //�ݺ��� ���� ����
	topData key; 

	//����ó�� �κ�
	if(size <= 0) { //size ������
		printf("OptSort: size error\n");
		return -1;
	} else if(data == NULL) { //����ü�� ���� ���
		printf("OptSort: data error\n");
		return -1;
	}

	switch(flag) {//flag���� ���� ������ ���� �����Ѵ�.
		case SORT_SIZE: //size�� ���� 
			for(i=1; i< size; i++){
				key = data[i];
				for(j=i-1; j>=0 && key.size > data[j].size ; j--){
					data[j+1] = data[j];
				}
				data[j+1] = key;
			}
			break;
		case SORT_PID://pid�� ����
			for(i=1; i< size; i++){
				key = data[i];
				for(j=i-1; j>=0 && key.pid > data[j].pid; j--){
					data[j+1] = data[j];
				}
				data[j+1] = key;
			}
			break;
		case SORT_RES://res�� ����
			for(i=1; i< size; i++){
				key = data[i];
				for(j=i-1; j>=0 && key.res > data[j].res ; j--){
					data[j+1] = data[j];
				}
				data[j+1] = key;
			}
			break;
		default : //flag���� �߸� �־��� �� �������
			printf("OptSort Flag error!\n");
			return -1;
			break;
	}

	return 0;
}

/////////////////
//Function Part//
/////////////////

//���۸� ���� ���� �Լ�.
void ClearReadBuffer(){
	if(stdin->_cnt)
		while(getchar() != '\n');
}

//data �迭 �ʱ�ȭ.
void InitData(topData *data) {
	int i;

	for(i=0; i<MAX_DATA_SIZE; i++){
		data[i].pid = -1;//pid�� -1�� ����
	}
}

//���μ��� ������ ���Ѵ�.
int GetDataSize(topData *data){
	int i;
	int count=0;

	for(i=0; i<MAX_DATA_SIZE; i++){
		if(data[i].pid == -1)//pid�� -1�� �� ������ �ݺ�
			break;
		count++;
	}
	return count;
}

// /proc ���丮�� ��ȸ�ϸ鼭 �� pid���� ������ ���� ������ ���� �غ� �Ѵ�.
void GetPsInfo(topData *data){
	DIR *dp; //���丮 ������
	int pid; 
	int count = 0; //�迭�� ������ �������� ����
	struct dirent *dent; //���丮 �����Ͱ� ����Ű�� ������ ������ ��´�.
    char dirName[] = "/proc";

	dp = opendir(dirName); // /proc ���丮�� ����.
    if(dp == NULL) {
		printf("ERROR: Could not open directory(%s).\n", dirName);
		exit(1);
	}

	while((dent=readdir(dp))) { //���丮 ������ �о� dent�� ����Ű�� �Ѵ�.
		pid = atoi(dent->d_name); //proc�� ���� ���丮 �̸��� pid�̴�.
		data[count] = OpenPsinfo(pid); //data �迭�� ���μ��� ������ ��´�.
		count++;
	}
	data[count].pid = -1; //�޺κп��� pid���� -1�� ����. ���ΰ�ħ �� �� ������ �޾ƿ� �����͸� �ɷ����� ���ؼ�.


    if(closedir(dp) < 0) {
		printf("ERROR: Could not close directory(%s).\n", dirName);
		exit(2);
	}
}

//���μ��� ������ �־�� topData ����ü�� �޾� ������ ����Ѵ�.
void PrintProcess(topData data){
	printf(" %-4d %-8s %-4d %-4d %-6d %-6d %-5s %-6c %-4.2lf %-4.2lf %-10s\n " ,data.pid, data.username, data.lwp, data.nice, data.size, data.res, data.time, data.state, data.percpu, data.permem, data.command);
}


//top���α׷� ȭ�� ���� �����ִ� �κ� ���
void PrintMainInfo(){
	printf("  ---------------------------------------\n");
	printf("                 Top program             \n");
	printf("                Help press 'h'           \n");
	printf("  ---------------------------------------\n");
	
	printf("  PID  USERNAME NLWP NICE SIZE   RES    TIME  STATE  CPU  MEM  COMMAND\n ");
}

//���μ��� ���� ���
//flag�� �ʱ�ȭ, ����/����/���� ������ ����� �����Ѵ�
void PrintPsInfo(topData *data, int flag) {
	static int currentIndex=0;
	static int page = 1;
	int i;
	int processCount = GetDataSize(data);
	
	// ���ΰ�ħ �� �� �ʱ�ȭ
	if(flag == INIT_PAGE){ //�ʱ�ȭ�϶�
		currentIndex =0;//���� index 0
		page = 1;//page 1�� ����
		flag = CURRENT_PAGE; //���� �������� ����ϱ����� �ñ׳��� �ٲ�.
	}

	//���� ������
	if(flag  == CURRENT_PAGE){
		for(i=currentIndex; i < page * MAX_SHOW_PROCESS; i++){//���� �������� ���
			if(i < processCount){//����ϴ� ������ ���μ��� ������ �ȳ�����
				PrintProcess(data[i]);//���
			}else {//������
				printf("\n");//�������� ���
			}
		}
	}

	//���� ������
	if(flag == FRONT_PAGE){
		if(currentIndex + MAX_SHOW_PROCESS >= processCount) { //�� �޺κ��̸�
			for(i=currentIndex;i< page * MAX_SHOW_PROCESS;i++){ //�� ��ġ����
				if(i < processCount){//����ϴ� ������ ���μ��� ������ �ȳ�����
					PrintProcess(data[i]);//���μ��� ������ �κ� ���
				}else {//������
					printf("\n");//�������� ���
				}
			}
		} else { //�� �޺κ��� �ƴϸ�
			currentIndex += MAX_SHOW_PROCESS;
			page++; //��� �� �Ѿ��
			for(i=currentIndex; i < page * MAX_SHOW_PROCESS; i++){//MAX_SHOW_PROCESS�� ��ŭ ���
				if(i< processCount){//����ϴ� ������ ���μ��� ������ �ȳ�����
					PrintProcess(data[i]);//����Ѵ�
				} else {
					printf("\n");//�������� ���.
				}
			} 
		}
	}

	//���� ������
	if(flag == BACK_PAGE){
		if(currentIndex < MAX_SHOW_PROCESS) {//ù �������� �׳� ���
			for(i=currentIndex; i < page * MAX_SHOW_PROCESS; i++)
					PrintProcess(data[i]);
		} else {
			currentIndex -= MAX_SHOW_PROCESS;
			page--;
			for(i=currentIndex; i < page * MAX_SHOW_PROCESS; i++){
					PrintProcess(data[i]);
			}
		}
	}

	printf(" page : %d \n", page);
}

// /proc/pid/psinfo ������ ��� topData ����ü�� ������ �����Ѵ�.
topData OpenPsinfo(int pid)
{
	int fd; //���� ������� ���� ����
	char fileName[1024]; //proc/PID/���� ��������
	psinfo_t data; //psinfo�� �о���� ���� ����ü
	topData t_data; //psinfo���� �ʿ��� �������� ��� ������ ����ü
	struct passwd *pw;

	//�����͸� ���� ������ �ӽú�����
	int lwp = 0; //�������� ����
	char *command; //���μ����� ������ Ŀ�ǵ�
	int size; //���μ����� �Ҵ�� �� �޸��� ��
	int res; //���μ����� ���� ���� ���� �޸��� ��
	timestruc_t time; //���μ����� ����� CPU �ð�
	int hour, min; //time���� �ð�, ���� ���� ����
	char t_result[8]; //hour + min�� ���ڿ��� ����
	int uid; //����� �̸��� ��� ���� uid�� ��´�.
	int nice; //�켱����
	char state;
	double cpu;
	double mem;
	
	memset(&data, 0, sizeof(psinfo_t)); //����ü �ʱ�ȭ
	sprintf(fileName, "/proc/%d/psinfo", pid); //���� pid�� ó���ϱ� ���� ���ڿ��� �����.
	
	if((fd = open(fileName, O_RDONLY)) < 0) //���� ����
	{
		printf("ERROR: %s open failed\n", fileName);
	}
	read(fd, &data, sizeof(psinfo_t)); //���� �б�

	//�ӽú����� ������ ����
	lwp = data.pr_nlwp;
	command = data.pr_fname;
	size = data.pr_size;
	res = data.pr_rssize;
	time = data.pr_time;
	hour = time.tv_sec / 60;
	min = time.tv_sec % 60;
	nice = getpriority(PRIO_PROCESS, pid);
	cpu = data.pr_pctcpu;
	mem = data.pr_pctmem;
	state = data.pr_lwp.pr_sname;
	if(min < 10) //hour + min�� ���ڿ��� ����
		sprintf(t_result, "%d:0%d", hour, min);
	else 
		sprintf(t_result, "%d:%d", hour, min);

	//passwd ������ �о� ����� �̸��� ��´�.
	uid = (int)data.pr_uid;
	pw = getpwuid(uid); 

	//������ �ٷ� topData ����ü�� ������ ��´�.
	t_data.pid = pid;
	t_data.lwp = (int)lwp;
	t_data.size = (int)size;
	t_data.res = (int)res;
	strcpy(t_data.command, command);
	strcpy(t_data.time, t_result);
	strcpy(t_data.username, (char*)pw->pw_name);
	t_data.nice = nice;
	t_data.percpu = cpu/0x8000*100;
	t_data.permem = mem/0x8000*100;
	t_data.state = state;
	close(fd);

	return t_data; //������ ���� topData ����ü ��ȯ
}

//����ڷκ��� pid, command �߿� ������ �˻��� �� �Է¹ް�
//�Է¹��� ������ �˻��Ͽ� ����Ѵ�.
void SearchData(char *str, topData *data)
{
	int i = 0; //�ݺ��� ���� ����
	int tf = -1; //�˻��� �׸��� pid��� 0, command��� 1
	char cmp[3][20] = {"pid", "command", "username"}; //str ���ڿ��� �񱳸� ���� �迭
	int value; //pid�� �˻��Ѵٸ� ã�� pid�� �Է¹޴´�.
	char strValue[20];//command�� �˻��Ѵٸ� ã�� command�� �Է¹޴´�.
	int flag; //���μ����� �����ϴ� �� ���θ� üũ
	int processCount = GetDataSize(data); //���μ��� ����
	

	//�˻��� �׸��� pid��� 0, command��� 1
	if( !strcmp(str, cmp[0]) )
		tf = 0;
	if( !strcmp(str, cmp[1]) )
		tf = 1;
	if( !strcmp(str, cmp[2]) )
		tf = 2;

	switch(tf){
		case 0: //�˻��� pid�� �Է¹޴´�
			printf("input PID : ");
			scanf("%d", &value);
			ClearReadBuffer();
			break;
		case 1: //�˻��� command�� �Է¹޴´�
			printf("input COMMAND : ");
			scanf("%s", strValue);
			ClearReadBuffer();
			break;
		case 2: //�˻��� command�� �Է¹޴´�
			printf("input USERNAME : ");
			scanf("%s", strValue);
			ClearReadBuffer();
			break;
	}

	flag = 0;
	while(i < processCount)
	{
		switch(tf){
			case 0: //pid�� �˻�
				if(data[i].pid == value){
					PrintProcess(data[i]);
					flag = 1;
				}
				break;
			case 1: //command�� �˻�
				if( !strcmp(data[i].command, strValue) ){
					PrintProcess(data[i]);
					flag = 1;
				}
				break;
			case 2: //username���� �˻�
				if( !strcmp(data[i].username, strValue) ){
					PrintProcess(data[i]);
					flag = 1;
				}
				break;
		}
		i++;
	}
	if(flag == 0){
		printf("not exist process\n");
	}
}

//������ �����ش�.
void PrintHelpInfo(){
	printf("'k' : kill process \n");
	printf("'s' : sort process\n");
	printf("'f' : search process\n");
	printf("'r' : update process\n");
	printf("'n' : show next page\n");
	printf("'b' : show back page\n");
	printf("'h' : help\n");
	printf("'q' : quit mytop program\n");
}
