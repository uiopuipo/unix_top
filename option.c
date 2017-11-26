#include "stdhdr.h"

//현재 실행되고있는 파일 구조체를 가져온다.
short  optKill( ){
	int targetPID;
	short flag;



	printf("PID to signal/kill [default pid = 22599] ");
	scanf("%d",&targetPID);
	if(flag = kill(targetPID, SIGKILL) == -1){
		printf("%d PID is not exist \n",targetPID);
	} else { 
		printf("%d PID is deleted \n",targetPID);
	}

	
	return targetPID;
}

