#include "stdhdr.h"

//현재 실행되고있는 파일 구조체를 가져온다.
void OptKill(){
	int targetPID;
	short flag;

	printf("PID to signal/kill [default pid = 1234] ");
	scanf("%d",&targetPID); //kill할 pid입력. 

	if(flag = kill(targetPID, SIGKILL) < 0 ){
		printf("%d PID is not exist \n",targetPID);
	} else { 
		printf("%d PID is deleted \n",targetPID);
	}
}

//정렬하고싶은 배열, 배열의 개수, FLAG값을 인자로 받는다.
int OptSort(topData *data, int size int flag){
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
			
			for(i=1; i< size; i++){
				key = data[i].size;
				for(j=i-1; j>=0 && key > data[j].size ; j--){
						ChangePos(data[j+1],data[j]);
				}
			}
			break;
		case SORT_PID:
			for(i=1; i< size; i++){
				key = data[i].pid;
				for(j=i-1; j>=0 && key > data[j].pid; j--){
						ChangePos(data[j+1],data[j]);
				}
			}
			break;
		case SORT_RES:
			for(i=1; i< size; i++){
				key = data[i].res;
				for(j=i-1; j>=0 && key > data[j].res ; j--){
						ChangePos(data[j+1],data[j]);
				}
			}
			break;
		default :
			printf("Flag error!\n");
			return -1;
			break;
	}

	return 0;
}

void ChangePos(topData *a, topData *b) {
	topData temp;

	temp.pid = a->pid;
	temp.lwp = a->lwp;
	strcpy(temp.command, a->command);
	strcpy(temp.time, a->time);
	temp.size = a->size;
	temp.res = a->res;

//	a.pid = b.pid;
//	a.lwp = b.lwp;
//	strcpy(a.command, b.command);
//	strcpy(a.time, b.time);
//	a.size = b.size;
//	a.res = b.res;
//
//	b.pid = temp.pid;
//	b.lwp = temp.lwp;
//	strcpy(b.command, temp.command);
//	strcpy(b.time, temp.time);
//	b.size = temp.size;
//	b.res = temp.res;
}
