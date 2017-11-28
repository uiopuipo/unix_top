#include "stdhdr.h"



int main(void) {
	topData data[MAX_DATA_SIZE];
	char optionChoice;


	system("clear");
	InitData(data);
	PrintMainInfo();
	GetPsInfo(data); //process정보를 data에 얻어온다.
	PrintPsInfo(data, CURRENT_PAGE);
	while(1){

		optionChoice = fgetc(stdin); //option 입력 시 optionChoice에 값을 넣음
		ClearReadBuffer();

		system("clear");
		PrintMainInfo();
		//옵션처리
		switch(optionChoice) {
			case 'k': //프로세스 종료 옵션
				PrintPsInfo(data, CURRENT_PAGE);
				OptKill();
				break;
			case 's': //정렬 옵션
				{
					PrintPsInfo(data, CURRENT_PAGE);
					char choice;
					int processCount = GetDataSize(data);
					printf("sorting (size:'s', pid:'p', res:'r') : ");
					choice = getc(stdin);
					ClearReadBuffer();
					switch(choice) {
						case 's':
							if(OptSort(data, processCount, SORT_SIZE) < 0)
								printf("sort part error\n");
							break;
						case 'p':
							if(OptSort(data, processCount, SORT_PID) < 0)
								printf("sort part error\n");
							break;
						case 'r':
							if(OptSort(data, processCount, SORT_RES) < 0)
								printf("sort part error\n");
							break;
						default:
							break;
					}
				}
				PrintPsInfo(data, CURRENT_PAGE);
				break;
			case 'f': //검색 옵션
				PrintPsInfo(data, CURRENT_PAGE);
				{
					char str[20];
					printf("검색할 항목을 입력하세요(pid, command) : ");
					scanf("%s", str);
					SearchData(str, data);
				}
				break;
			case 'r': // 업데이트
				GetPsInfo(data);
				PrintPsInfo(data, INIT_PAGE);
				break;
			case 'n': //화면 넘기
				PrintPsInfo(data, FRONT_PAGE);
				break;
			case 'b': //이전 화면으로 넘기기
				PrintPsInfo(data, BACK_PAGE);
				break;
			case 'h':
				PrintHelpInfo();
				break;
			case 'q': //q를 누르면 프로그램 종료
				system("clear");
				exit(1);
				break;
			default :
				break;
		}
	//	PrintPsInfo(data, CURRENT_PAGE);
	//	GetPsInfo(data); //process정보를 data에 얻어온다. 여기다 이걸 넣어버리면, 업데이트 기능이 필요가없다.. 일단 주석처리
	}


	return 0;
}
