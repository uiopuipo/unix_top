#include "stdhdr.h"

///////////////
//Option Part//
///////////////
//현재 실행되고있는 파일 구조체를 가져온다.
void OptKill(){
	int targetPID;
	short flag;

	printf("PID to signal/kill [default pid = 1234] ");
	scanf("%d",&targetPID); //kill할 pid입력. 
	ClearReadBuffer();

	if(flag = kill(targetPID, SIGKILL) < 0 ){
		printf("%d PID is not exist or you have to get permission \n",targetPID);
	} else { 
		printf("%d PID is deleted \n",targetPID);
	}
}

//정렬하고싶은 배열, 배열의 개수, FLAG값을 인자로 받는다.
int OptSort(topData *data, int size, int flag){
	int i, j;
	int key; //비교할 값


	if(size <= 0) {
		printf("OptSort: size error\n");
		return -1;
	} else if(data == NULL) {
		printf("OptSort: data error\n");
		return -1;
	}

	switch(flag) {
		case SORT_SIZE:
			printf("sorting size\n"); 
			
			for(i=1; i< size; i++){
				key = data[i].size;
				for(j=i-1; j>=0 && key > data[j].size ; j--){
					ChangePos(&data[j+1],&data[j]);
				}
			}
			break;
		case SORT_PID:
			printf("sorting pid\n"); 
			for(i=1; i< size; i++){
				key = data[i].pid;
				for(j=i-1; j>=0 && key > data[j].pid; j--){
					ChangePos(&data[j+1],&data[j]);
				}
			}
			break;
		case SORT_RES:
			printf("sorting res\n"); 
			for(i=1; i< size; i++){
				key = data[i].res;
				for(j=i-1; j>=0 && key > data[j].res ; j--){
					ChangePos(&data[j+1],&data[j]);
				}
			}
			break;
		default :
			printf("OptSort Flag error!\n");
			return -1;
			break;
	}

	return 0;
}

/////////////////
//Function Part//
/////////////////

void ChangePos(topData *a, topData *b) {
	topData temp;

	temp.pid = a->pid;
	temp.lwp = a->lwp;
	strcpy(temp.command, a->command);
	strcpy(temp.time, a->time);
	temp.size = a->size;
	temp.res = a->res;

	a->pid = b->pid;
	a->lwp = b->lwp;
	strcpy(a->command, b->command);
	strcpy(a->time, b->time);
	a->size = b->size;
	a->res = b->res;

	b->pid = temp.pid;
	b->lwp = temp.lwp;
	strcpy(b->command, temp.command);
	strcpy(b->time, temp.time);
	b->size = temp.size;
	b->res = temp.res;
}

//버퍼를 비우기 위한 함수.
void ClearReadBuffer(){
	if(stdin->_cnt)
		while(getchar() != '\n');
}

//data 배열 초기화.
void InitData(topData *data) {
	int i;

	for(i=0;i<MAX_DATA_SIZE;i++){
		data[i].pid=-1;//pid를 -1로 설정함.
	}
}

//프로세스 개수를 구함. 전역변수 processCount에 넣게끔 만듬 (나중에 바꿀수도 있음)
void GetDataSize(topData *data){
	int i;
	int count=0;

	for(i=0;i<MAX_DATA_SIZE;i++){
		if(data[i].pid == -1)//pid가 -1일때까지 loop
			break;
		count++;
	}

	processCount = count; //가져온 프로세스 개수
}

//하나의 프로세스값 출력
void PrintProcess(topData data){
	printf("PID:%d ", data.pid);
	printf("LWP:%d COMMAND:%s ", data.lwp, data.command);
	printf("SIZE:%d RES:%d ", data.size, data.res);
	printf("TIME: %s ", data.time);
	printf("COMMAND: %s \n", data.command);
}



void PrintData(int pid, topData *data)
{
	printf("%-4d | PID : %-7d| LWP %-4d| SIZE : %-7d| RES : %-7d| TIME : %-7s| MEM : %4d| COMMAND : %-10s\n ", processCount ,data[pid].pid, data[pid].lwp, data[pid].size, data[pid].res, data[pid].time, data[pid].mem, data[pid].command);
	printf("---------------------------------------------------------------------------------------------------------------\n");
}
void GetPsInfo(topData *data){
	DIR *dp;
	int pid;
	int count=0;
	struct dirent *dent;
    char dirName[] = "/proc";

	dp = opendir(dirName); // /proc 디렉토리를 열어 
    if(dp == NULL) {
		printf("ERROR: Could not open directory(%s).\n", dirName);
		exit(1);
	}

	while((dent=readdir(dp))) {//디렉토리 정보를 읽어 dent가 가리키게 한다.
		pid = atoi(dent->d_name);
		data[count] = OpenPsinfo(pid);
		count++;
	}
	data[count].pid = -1; //뒷부분에는 pid값을 -1로 해줌. Update할 시 이전에 받아온데이터를 걸러내기 위해서.

	if(count == 0) {
		printf("프로세스의 개수가 %d개 입니다.\n",count);
	} else {
		processCount = count; //프로세스 개수 설정
	}

    if(closedir(dp) < 0) {
		printf("ERROR: Could not close directory(%s).\n", dirName);
		exit(2);
	}
}

//top프로그램 화면 위에 보여주는부분 출력
void PrintMainInfo(){
	printf("------------------------\n");
	printf("This is a Top program \n");
	printf("Help press 'h'\n");
	printf("------------------------\n");
}

//프로세스 정보 출력.
void PrintPsInfo(topData *data, int signal) {
	static int currentIndex=0;
	static int page = 1;
	int i;

	// 업데이트시 초기화
	if(signal == INIT_PAGE){
		currentIndex =0;
		page = 1;
		signal = CURRENT_PAGE;
	}
	if(signal == CURRENT_PAGE){
		for(i=currentIndex; i<page*15;i++){
			PrintProcess(data[i]);
		}
	}

	if(signal == FRONT_PAGE){//앞페이지를 누를 때 
		if(currentIndex + 15 > processCount) { // 맨 뒷부분이면
			for(i=currentIndex;i<processCount;i++){ //그냥 현위치에서
				PrintProcess(data[i]);
			}
		} else {//맨 뒷부분이 아니면
			currentIndex += 15;
			page++; //출력 후 넘어가
			for(i=currentIndex; i < page * 15; i++){
				if(i< processCount){
					PrintProcess(data[i]);

				}
			} 
		}
	}

	if(signal ==BACK_PAGE){//뒷페이지를 눌렀을 시
		if(currentIndex < 15) {//첫페이지면 그냥 출력.
			for(i=currentIndex; i < page * 15; i++)
					PrintProcess(data[i]);
		} else {
			currentIndex -= 15;
			page--;
			for(i=currentIndex; i < page*15; i++){
					PrintProcess(data[i]);
			}
		}
	}

	printf("page : %d \n", page);
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

int Getch(void){
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
	ClearReadBuffer();
	return ch;
}

void SearchData(char *str, topData *data)
{
	int i = 0;
	int tf = -1;
	char cmp[2][20] = {"pid", "command"};
	int value;
	char strValue[20];
	int flag;

	if( !strcmp(str, cmp[0]) )
		tf = 0;
	if( !strcmp(str, cmp[1]) )
		tf = 1;

	switch(tf){
		case 0:
			printf("input PID : ");
			scanf("%d", &value);
			ClearReadBuffer();
			break;
		case 1:
			printf("input COMMAND : ");
			scanf("%s", strValue);
			ClearReadBuffer();
			break;
	}

	flag =0;
	while(i < processCount)
	{
		switch(tf){
			case 0:
				if(data[i].pid == value){	
					PrintProcess(data[i]);
					flag = 1;
				}
				break;
			case 1:
				if( !strcmp(data[i].command, strValue) ){
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
