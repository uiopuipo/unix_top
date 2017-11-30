#include "stdhdr.h"

int main(void) {
	topData data[MAX_DATA_SIZE]; //각 pid에 해당하는 정보를 담을 배열
	char optionChoice; //옵션을 입력받을 변수

	system("clear");
	InitData(data); //초기화 함수
	PrintMainInfo(); //메인 화면 출력
	GetPsInfo(data); //process정보를 data 배열에 저장
	PrintPsInfo(data, CURRENT_PAGE); //프로세스 정보를 출력
	while(1){

		optionChoice = fgetc(stdin); //사용자에게 option을 입력받아 optionChoice에 값을 넣음
		ClearReadBuffer(); //버퍼를 비운다

		system("clear");
		PrintMainInfo();
		//옵션처리
		switch(optionChoice) {
			case 'k': //프로세스 종료 옵션
				PrintPsInfo(data, CURRENT_PAGE);
				OptKill(); //프로세스에 SIGKILL 시그널을 보낸다
				break;
			case 's': //정렬 옵션
				{
					PrintPsInfo(data, CURRENT_PAGE);
					char choice; //어떤 방식으로 정렬할 지 입력받는 변수
					int processCount = GetDataSize(data); //프로세스의 갯수를 얻어온다
					printf("sorting (size:'s', pid:'p', res:'r') : ");
					choice = getc(stdin); //옵션을 입력받는다
					ClearReadBuffer(); //버퍼를 비운다
					switch(choice) {
						case 's': //프로세스에 할당된 총 메모리의 양으로 정렬
							if(OptSort(data, processCount, SORT_SIZE) < 0)
								printf("sort part error\n");
							break;
						case 'p': //pid로 정렬
							if(OptSort(data, processCount, SORT_PID) < 0)
								printf("sort part error\n");
							break;
						case 'r': //프로세스에 의해 사용된 물리 메모리의 양으로 정렬
							if(OptSort(data, processCount, SORT_RES) < 0)
								printf("sort part error\n");
							break;
						default:
							break;
					}
				}
				system("clear");
				PrintMainInfo();
				PrintPsInfo(data, CURRENT_PAGE);
				break;
			case 'f': //검색 옵션
				PrintPsInfo(data, CURRENT_PAGE);
				{
					char str[20];
					printf("검색할 항목을 입력하세요(pid, command) : ");
					scanf("%s", str);
					SearchData(str, data); //pid 또는 command로 검색
				}
				break;
			case 'r': //새로고침
				GetPsInfo(data);
				PrintPsInfo(data, INIT_PAGE);
				break;
			case 'n': //화면 넘기기
				PrintPsInfo(data, FRONT_PAGE);
				break;
			case 'b': //이전 화면으로 넘기기
				PrintPsInfo(data, BACK_PAGE);
				break;
			case 'h': //도움말
				PrintHelpInfo();
				break;
			case 'q': //프로그램 종료
				system("clear");
				exit(1);
				break;
			default :
				PrintPsInfo(data, CURRENT_PAGE);
				break;
		}
	//	PrintPsInfo(data, CURRENT_PAGE);
	//	GetPsInfo(data); //process정보를 data에 얻어온다. 여기다 이걸 넣어버리면, 업데이트 기능이 필요가없다.. 일단 주석처리
	}
	return 0;
}

