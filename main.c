#include "stdhdr.h"



int main(void) {
	topData data[MAX_DATA_SIZE];
	char optionChoice;


	system("clear");
	InitData(data);
	PrintMainInfo();
	GetPsInfo(data); //process������ data�� ���´�.
	PrintPsInfo(data, CURRENT_PAGE);
	while(1){

		optionChoice = fgetc(stdin); //option �Է� �� optionChoice�� ���� ����
		ClearReadBuffer();

		system("clear");
		PrintMainInfo();
		//�ɼ�ó��
		switch(optionChoice) {
			case 'k': //���μ��� ���� �ɼ�
				PrintPsInfo(data, CURRENT_PAGE);
				OptKill();
				break;
			case 's': //���� �ɼ�
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
			case 'f': //�˻� �ɼ�
				PrintPsInfo(data, CURRENT_PAGE);
				{
					char str[20];
					printf("�˻��� �׸��� �Է��ϼ���(pid, command) : ");
					scanf("%s", str);
					SearchData(str, data);
				}
				break;
			case 'r': // ������Ʈ
				GetPsInfo(data);
				PrintPsInfo(data, INIT_PAGE);
				break;
			case 'n': //ȭ�� �ѱ�
				PrintPsInfo(data, FRONT_PAGE);
				break;
			case 'b': //���� ȭ������ �ѱ��
				PrintPsInfo(data, BACK_PAGE);
				break;
			case 'h':
				PrintHelpInfo();
				break;
			case 'q': //q�� ������ ���α׷� ����
				system("clear");
				exit(1);
				break;
			default :
				break;
		}
	//	PrintPsInfo(data, CURRENT_PAGE);
	//	GetPsInfo(data); //process������ data�� ���´�. ����� �̰� �־������, ������Ʈ ����� �ʿ䰡����.. �ϴ� �ּ�ó��
	}


	return 0;
}
