#include "stdhdr.h"



int main(void) {
	int pid,men;
	char str[20];
	topData data[MAX_DATA_SIZE];
	char optionChoice;


	system("clear");
	InitData(data);
	PrintMainInfo();
	GetPsInfo(data); //process정보를 data에 얻어온다.
	PrintProcess(data);
	while(1){

		optionChoice = fgetc(stdin); //option 입력 시 optionChoice에 값을 넣음
		ClearReadBuffer();

		
		//옵션처리
		switch(optionChoice) {
			case 'k': //프로세스 종료 옵션
				system("clear");
				PrintMainInfo();
				OptKill();
				break;
			case 's': //정렬 옵션
				system("clear");
				PrintMainInfo();
				{
					char choice;
					int size = processCount; //나중에 static없앨 수도 있어서 size 변수 선언
					printf("size : :%d\n", size);
					printf("sorting (size:'s', pid:'p', res:'r') : ");
					choice = getc(stdin);
					ClearReadBuffer();
					switch(choice) {
						case 's':
							printf("you choose s\n");
							if(OptSort(data, size, SORT_SIZE) < 0)
								printf("sort part error\n");
							break;
						case 'p':
							printf("you choose p\n");
							if(OptSort(data, size, SORT_PID) < 0)
								printf("sort part error\n");
							break;
						case 'r':
							printf("you choose r\n");
							if(OptSort(data, size, SORT_RES) < 0)
								printf("sort part error\n");
							break;
						default:
							break;
					}

				}
				break;
			case 'f': //검색 옵션
				system("clear");
				PrintMainInfo();
				printf("검색할 항목을 입력하세요(pid, command) : ");
				scanf("%s", str);
				SearchData(str, data);
				break;
			case 'r': // 업데이트
				system("clear");
				PrintMainInfo();
				GetPsInfo(data);
				PrintPsInfo(data, INIT_PAGE);
				break;
			case 'n': //화면 넘기
				system("clear");
				PrintMainInfo();
				PrintPsInfo(data, FRONT_PAGE);
				break;
			case 'b': //이전 화면으로 넘기기
				system("clear");
				PrintMainInfo();
				PrintPsInfo(data, BACK_PAGE);
				break;
			case 'h':
				system("clear");
				PrintMainInfo();
				PrintHelpInfo();
				break;
			case 'q': //q를 누르면 프로그램 종료
				system("clear");
				exit(1);
				break;
			default :
				break;
		}
//		PrintPsInfo(data, CURRENT_PAGE);
	}


	return 0;
}
