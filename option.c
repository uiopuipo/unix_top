#include "stdhdr.h"

//현재 실행되고있는 파일 구조체를 가져온다.
int  optKill(topData *top ){
	int targetPID;

	printf("PID to signal/kill [default pid = 22599] ");
	scanf("%d",&targetPID);
	printf("dd%d",targetPID);

	
//	if(){

//	}else {
		
//	}
	return targetPID;
}

//프로그램 종료
int optEnd(char d){
	printf("%c",d);
	if( d == 'q') { //q를 누르면 1을 리턴 
		return 1;
	}
	return 0; //아니면 0을 리턴
}
