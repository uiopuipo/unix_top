#include "stdhdr.h"
///////////////
//Option Part//
///////////////

void OptKill(){
	int targetPID; //시그널을 보낼 pid
	short flag; //오류 처리를 위한 변수

	printf("PID to signal/kill [default pid = 1234] ");
	scanf("%d",&targetPID); //시그널을 보낼 pid입력
	ClearReadBuffer(); //버퍼를 비운다

	if(flag = kill(targetPID, SIGKILL) < 0 ){//입력한 pid의 프로세스를 죽인다.
		printf("%d PID is not exist or you have to get permission \n",targetPID);
	} else { 
		printf("%d PID is deleted \n",targetPID);
	}
}

//정렬 하고싶은 배열, 배열의 개수, flag(정렬 기준)을 인자로 받는다.
int OptSort(topData *data, int size, int flag){
	int i, j; //반복문 제어 변수
	topData key; 

	//오류처리 부분
	if(size <= 0) { //size 오류시
		printf("OptSort: size error\n");
		return -1;
	} else if(data == NULL) { //구조체가 널일 경우
		printf("OptSort: data error\n");
		return -1;
	}

	switch(flag) {//flag값에 따라 정렬할 것을 결정한다.
		case SORT_SIZE: //size로 정렬 
			for(i=1; i< size; i++){
				key = data[i];
				for(j=i-1; j>=0 && key.size > data[j].size ; j--){
					data[j+1] = data[j];
				}
				data[j+1] = key;
			}
			break;
		case SORT_PID://pid로 정렬
			for(i=1; i< size; i++){
				key = data[i];
				for(j=i-1; j>=0 && key.pid > data[j].pid; j--){
					data[j+1] = data[j];
				}
				data[j+1] = key;
			}
			break;
		case SORT_RES://res로 정렬
			for(i=1; i< size; i++){
				key = data[i];
				for(j=i-1; j>=0 && key.res > data[j].res ; j--){
					data[j+1] = data[j];
				}
				data[j+1] = key;
			}
			break;
		default : //flag값을 잘못 넣었을 시 오류출력
			printf("OptSort Flag error!\n");
			return -1;
			break;
	}

	return 0;
}

/////////////////
//Function Part//
/////////////////

//버퍼를 비우기 위한 함수.
void ClearReadBuffer(){
	if(stdin->_cnt)
		while(getchar() != '\n');
}

//data 배열 초기화.
void InitData(topData *data) {
	int i;

	for(i=0; i<MAX_DATA_SIZE; i++){
		data[i].pid = -1;//pid를 -1로 설정
	}
}

//프로세스 개수를 구한다.
int GetDataSize(topData *data){
	int i;
	int count=0;

	for(i=0; i<MAX_DATA_SIZE; i++){
		if(data[i].pid == -1)//pid가 -1이 될 때까지 반복
			break;
		count++;
	}
	return count;
}

//프로세스 정보를 넣어둔 topData 구조체를 받아 내용을 출력한다.
void PrintProcess(topData data){
	printf(" PID:%-6d  USERNAME:%-10s  LWP:%-4d  SIZE:%-6d  RES:%-6d  TIME:%-6s  COMMAND:%-10s\n " ,data.pid, data.username, data.lwp, data.size, data.res, data.time, data.command);
}

// /proc 디렉토리를 순회하면서 각 pid마다 파일을 열어 정보를 얻어올 준비를 한다.
void GetPsInfo(topData *data){
	DIR *dp; //디렉토리 포인터
	int pid; 
	int count = 0; //배열의 갯수를 세기위한 변수
	struct dirent *dent; //디렉토리 포인터가 가리키는 파일의 정보를 담는다.
    char dirName[] = "/proc";

	dp = opendir(dirName); // /proc 디렉토리를 연다.
    if(dp == NULL) {
		printf("ERROR: Could not open directory(%s).\n", dirName);
		exit(1);
	}

	while((dent=readdir(dp))) { //디렉토리 정보를 읽어 dent가 가리키게 한다.
		pid = atoi(dent->d_name); //proc의 서브 디렉토리 이름은 pid이다.
		data[count] = OpenPsinfo(pid); //data 배열에 프로세스 정보를 담는다.
		count++;
	}
	data[count].pid = -1; //뒷부분에는 pid값을 -1로 해줌. 새로고침 할 때 이전에 받아온 데이터를 걸러내기 위해서.


    if(closedir(dp) < 0) {
		printf("ERROR: Could not close directory(%s).\n", dirName);
		exit(2);
	}
}

//top프로그램 화면 위에 보여주는 부분 출력
void PrintMainInfo(){
	printf("----------------------------------------------------------------------------------\n");
	printf("                                   Top program            \n");
	printf("                                  Help press 'h'          \n");
	printf("----------------------------------------------------------------------------------\n");
	
	printf("    PID         USERNAME              LWP       SIZE         RES         TIME          COMMAND\n ");
}

//프로세스 정보 출력
//flag로 초기화, 이전/현재/다음 페이지 명령을 구분한다
void PrintPsInfo(topData *data, int flag) {
	static int currentIndex=0;
	static int page = 1;
	int i;
	int processCount = GetDataSize(data);
	
	// 새로고침 할 시 초기화
	if(flag == INIT_PAGE){ //초기화일때
		currentIndex =0;//현재 index 0
		page = 1;//page 1로 만듬
		flag = CURRENT_PAGE; //현재 페이지를 출력하기위해 시그널을 바꿈.
	}

	//현재 페이지
	if(flag  == CURRENT_PAGE){
		for(i=currentIndex; i < page * MAX_SHOW_PROCESS; i++){//현재 페이지를 출력
			if(i < processCount){//출력하는 개수가 프로세스 개수를 안넘으면
				PrintProcess(data[i]);//출력
			}else {//넘으면
				printf("\n");//공백으로 출력
			}
		}
	}

	//다음 페이지
	if(flag == FRONT_PAGE){
		if(currentIndex + MAX_SHOW_PROCESS >= processCount) { //맨 뒷부분이면
			for(i=currentIndex;i< page * MAX_SHOW_PROCESS;i++){ //현 위치에서
				if(i < processCount){//출력하는 개수가 프로세스 개수를 안넘으면
					PrintProcess(data[i]);//프로세스 나머지 부분 출력
				}else {//넘으면
					printf("\n");//공백으로 출력
				}
			}
		} else { //맨 뒷부분이 아니면
			currentIndex += MAX_SHOW_PROCESS;
			page++; //출력 후 넘어가기
			for(i=currentIndex; i < page * MAX_SHOW_PROCESS; i++){//MAX_SHOW_PROCESS값 만큼 출력
				if(i< processCount){//출력하는 개수가 프로세스 개수를 안넘으면
					PrintProcess(data[i]);//출력한다
				} else {
					printf("\n");//공백으로 출력.
				}
			} 
		}
	}

	//이전 페이지
	if(flag == BACK_PAGE){
		if(currentIndex < MAX_SHOW_PROCESS) {//첫 페이지면 그냥 출력
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

// /proc/pid/psinfo 파일을 열어서 topData 구조체에 정보를 저장한다.
topData OpenPsinfo(int pid)
{
	int fd; //파일 입출력을 위한 변수
	char fileName[1024]; //proc/PID/각종 정보파일
	psinfo_t data; //psinfo를 읽어오기 위한 구조체
	topData t_data; //psinfo에서 필요한 정보만을 골라서 저장할 구조체
	struct passwd *pw;

	//데이터를 꺼내 저장할 임시변수들
	int lwp = 0; //쓰레드의 개수
	char *command; //프로세스를 실행한 커맨드
	int size; //프로세스에 할당된 총 메모리의 양
	int res; //프로세스에 의해 사용된 물리 메모리의 양
	timestruc_t time; //프로세스가 사용한 CPU 시간
	int hour, min; //time에서 시간, 분을 빼올 변수
	char t_result[8]; //hour + min을 문자열로 저장yy
	int uid; //사용자 이름을 얻기 위해 uid를 얻는다.
	
	memset(&data, 0, sizeof(psinfo_t)); //구조체 초기화
	sprintf(fileName, "/proc/%d/psinfo", pid); //여러 pid를 처리하기 위해 문자열을 만든다.
	
	if((fd = open(fileName, O_RDONLY)) < 0) //파일 열기
	{
		printf("ERROR: %s open failed\n", fileName);
	}
	read(fd, &data, sizeof(psinfo_t)); //파일 읽기

	//임시변수에 정보를 저장
	lwp = data.pr_nlwp;
	command = data.pr_fname;
	size = data.pr_size;
	res = data.pr_rssize;
	time = data.pr_time;
	hour = time.tv_sec / 60;
	min = time.tv_sec % 60; 
	if(min < 10) //hour + min을 문자열로 저장
		sprintf(t_result, "%d:0%d", hour, min);
	else 
		sprintf(t_result, "%d:%d", hour, min);

	//passwd 파일을 읽어 사용자 이름을 얻는다.
	uid = (int)data.pr_uid;
	pw = getpwuid(uid); 

	//실제로 다룰 topData 구조체에 정보를 담는다.
	t_data.pid = pid;
	t_data.lwp = (int)lwp;
	t_data.size = (int)size;
	t_data.res = (int)res;
	strcpy(t_data.command, command);
	strcpy(t_data.time, t_result);
	strcpy(t_data.username, (char*)pw->pw_name);
	close(fd);

	return t_data; //정보를 담은 topData 구조체 반환
}

//사용자로부터 pid, command 중에 무엇을 검색할 지 입력받고
//입력받은 정보를 검색하여 출력한다.
void SearchData(char *str, topData *data)
{
	int i = 0; //반복문 제어 변수
	int tf = -1; //검색할 항목이 pid라면 0, command라면 1
	char cmp[3][20] = {"pid", "command", "username"}; //str 문자열과 비교를 위한 배열
	int value; //pid로 검색한다면 찾을 pid를 입력받는다.
	char strValue[20];//command로 검색한다면 찾을 command를 입력받는다.
	int flag; //프로세스가 존재하는 지 여부를 체크
	int processCount = GetDataSize(data); //프로세스 개수
	

	//검색할 항목이 pid라면 0, command라면 1
	if( !strcmp(str, cmp[0]) )
		tf = 0;
	if( !strcmp(str, cmp[1]) )
		tf = 1;
	if( !strcmp(str, cmp[2]) )
		tf = 2;

	switch(tf){
		case 0: //검색할 pid를 입력받는다
			printf("input PID : ");
			scanf("%d", &value);
			ClearReadBuffer();
			break;
		case 1: //검색할 command를 입력받는다
			printf("input COMMAND : ");
			scanf("%s", strValue);
			ClearReadBuffer();
			break;
		case 2: //검색할 command를 입력받는다
			printf("input USERNAME : ");
			scanf("%s", strValue);
			ClearReadBuffer();
			break;
	}

	flag = 0;
	while(i < processCount)
	{
		switch(tf){
			case 0: //pid로 검색
				if(data[i].pid == value){
					PrintProcess(data[i]);
					flag = 1;
				}
				break;
			case 1: //command로 검색
				if( !strcmp(data[i].command, strValue) ){
					PrintProcess(data[i]);
					flag = 1;
				}
				break;
			case 2: //username으로 검색
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

//도움말을 보여준다.
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
