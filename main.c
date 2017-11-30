#include "stdhdr.h"

int main(void) {
	topData data[MAX_DATA_SIZE]; //�� pid�� �ش��ϴ� ������ ���� �迭
	char optionChoice; //�ɼ��� �Է¹��� ����

	system("clear");
	InitData(data); //�ʱ�ȭ �Լ�
	PrintMainInfo(); //���� ȭ�� ���
	GetPsInfo(data); //process������ data �迭�� ����
	PrintPsInfo(data, CURRENT_PAGE); //���μ��� ������ ���
	while(1){

		optionChoice = fgetc(stdin); //����ڿ��� option�� �Է¹޾� optionChoice�� ���� ����
		ClearReadBuffer(); //���۸� ����

		system("clear");
		PrintMainInfo();
		//�ɼ�ó��
		switch(optionChoice) {
			case 'k': //���μ��� ���� �ɼ�
				PrintPsInfo(data, CURRENT_PAGE);
				OptKill(); //���μ����� SIGKILL �ñ׳��� ������
				break;
			case 's': //���� �ɼ�
				{
					PrintPsInfo(data, CURRENT_PAGE);
					char choice; //� ������� ������ �� �Է¹޴� ����
					int processCount = GetDataSize(data); //���μ����� ������ ���´�
					printf("sorting (size:'s', pid:'p', res:'r') : ");
					choice = getc(stdin); //�ɼ��� �Է¹޴´�
					ClearReadBuffer(); //���۸� ����
					switch(choice) {
						case 's': //���μ����� �Ҵ�� �� �޸��� ������ ����
							if(OptSort(data, processCount, SORT_SIZE) < 0)
								printf("sort part error\n");
							break;
						case 'p': //pid�� ����
							if(OptSort(data, processCount, SORT_PID) < 0)
								printf("sort part error\n");
							break;
						case 'r': //���μ����� ���� ���� ���� �޸��� ������ ����
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
			case 'f': //�˻� �ɼ�
				PrintPsInfo(data, CURRENT_PAGE);
				{
					char str[20];
					printf("�˻��� �׸��� �Է��ϼ���(pid, command) : ");
					scanf("%s", str);
					SearchData(str, data); //pid �Ǵ� command�� �˻�
				}
				break;
			case 'r': //���ΰ�ħ
				GetPsInfo(data);
				PrintPsInfo(data, INIT_PAGE);
				break;
			case 'n': //ȭ�� �ѱ��
				PrintPsInfo(data, FRONT_PAGE);
				break;
			case 'b': //���� ȭ������ �ѱ��
				PrintPsInfo(data, BACK_PAGE);
				break;
			case 'h': //����
				PrintHelpInfo();
				break;
			case 'q': //���α׷� ����
				system("clear");
				exit(1);
				break;
			default :
				PrintPsInfo(data, CURRENT_PAGE);
				break;
		}
	//	PrintPsInfo(data, CURRENT_PAGE);
	//	GetPsInfo(data); //process������ data�� ���´�. ����� �̰� �־������, ������Ʈ ����� �ʿ䰡����.. �ϴ� �ּ�ó��
	}
	return 0;
}

