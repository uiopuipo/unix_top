#include "stdhdr.h"



int main(void) {
	int pid,men;
	char str[20];
	topData data[MAX_DATA_SIZE];
	char optionChoice;


	system("clear");
	InitData(data);
	PrintMainInfo();
	GetPsInfo(data); //process������ data�� ���´�.
	PrintProcess(data);
	while(1){

		optionChoice = fgetc(stdin); //option �Է� �� optionChoice�� ���� ����
		ClearReadBuffer();

		
		//�ɼ�ó��
		switch(optionChoice) {
			case 'k': //���μ��� ���� �ɼ�
				system("clear");
				PrintMainInfo();
				OptKill();
				break;
			case 's': //���� �ɼ�
				system("clear");
				PrintMainInfo();
				{
					char choice;
					int size = processCount; //���߿� static���� ���� �־ size ���� ����
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
			case 'f': //�˻� �ɼ�
				system("clear");
				PrintMainInfo();
				PrintPsInfo(data, CURRENT_PAGE);
				printf("�˻��� �׸��� �Է��ϼ���(pid, command) : ");
				scanf("%s", str);
				SearchData(str, data);
				break;
			case 'r': // ������Ʈ
				system("clear");
				PrintMainInfo();
				GetPsInfo(data);
				PrintPsInfo(data, INIT_PAGE);
				break;
			case 'n': //ȭ�� �ѱ�
				system("clear");
				PrintMainInfo();
				PrintPsInfo(data, FRONT_PAGE);
				break;
			case 'b': //���� ȭ������ �ѱ��
				system("clear");
				PrintMainInfo();
				PrintPsInfo(data, BACK_PAGE);
				break;
			case 'h':
				system("clear");
				PrintMainInfo();
				PrintHelpInfo();
				break;
			case 'q': //q�� ������ ���α׷� ����
				system("clear");
				exit(1);
				break;
			default :
				break;
		}
//		PrintPsInfo(data, CURRENT_PAGE);
//		GetPsInfo(data); //process������ data�� ���´�. ����� �̰� �־������, ������Ʈ ����� �ʿ䰡����.. �ϴ� �ּ�ó��
	}


	return 0;
}
