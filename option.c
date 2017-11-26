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

int OptSort(topData *data, int flag) {
	int i, j;



	switch(flag) {
		case SORT_SIZE:
//			for(i=0; i< sizeof(data)
			break;
		case SORT_PID:
			break;
		case SORT_RES:
			break;
		default :
			printf("signal error!\n");
			return -1;
	}

	return 0;
}
