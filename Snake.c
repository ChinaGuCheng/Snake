
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include<time.h>
#include<conio.h>	//����̨�������

//#include "mmsystem.h"//��������ͷ�ļ� 
#pragma comment(lib,"winmm.lib")//��������ͷ�ļ��� 

//�ߵ�����������
typedef struct coordinate
{
	int x;
	int y;
}nate;

//װ��������Ľڵ�
typedef struct Node
{
	nate data;
	struct Node *next;
}Node;

//�ߵ�����    �����ͷ�ڵ�
typedef struct Nate
{
	Node head;
	int size;
}Nate;



#define W 50//һ��50��
#define H 20//һ��20��

int GuanQia = 1;	//��ǰ�ؿ�

nate GuanQiaData1[30] = { 0 };//��1�ص��ϰ�����������
nate GuanQiaData2[60] = { 0 };//��2�ص��ϰ�����������
nate GuanQiaData3[65] = { 0 };//��3�ص��ϰ�����������
nate GuanQiaData4[77] = { 0 };//��4�ص��ϰ�����������

int TongGuanLen = 20;//ͨ�س���

int power = 0;//��ͣ�Ŀ���  Ĭ�ϼ�
int exit1 = 0;	//�����ı�־
Nate *list;		//������
int MyThreadID;
nate ShiWu;		//ʳ�������
HANDLE hd;		//�߳̾��
HANDLE hd2;		//�߳�2���
int path = 2;	//�������ƶ��ķ��� 1�� 2�� 3�� 4��
int tmp = 200;//ˢ��Ƶ��

int ChuanQiangPower = 0;//��ǽģʽ 0Ϊ�� 1Ϊ��

//ͼ��
//int ic = ;
//char a = 178;
//char ico_0[3] = { 161,0 };

char *ico_0 = "\01";		//�߿�ͼ��
char *ico_1 = "\02";	//��ͷͼ��
char *ico_2 = "\04";	//��ͷ����һ��ͼ��
char *ico_3 = "\05";	//��ǰ��2������ͼ��
char *ico_4 = "\03";	//ʳ��ͼ��

//char *ico_0 = "X";		//�߿�ͼ��
//char *ico_1 = "O";	//��ͷͼ��
//char *ico_2 = "~";	//��ͷ����һ��ͼ��
//char *ico_3 = "#";	//��ǰ��2������ͼ��
//char *ico_4 = "$";	//ʳ��ͼ��

void test();

//�ϰ����ʼ��
void InitZhangAiWu();

//�ж��Ƿ�ײ���ϰ���
int Func01(int x, int y);

/*
*function: �ڿ���ָ̨����λ������ַ���
*������buf,������ַ�����startX��startYΪ����̨��X��Y����
*/
void printStr(char *buf, int startX, int startY)
{
	static flag = 0;
	HANDLE hd;
	COORD pos;
	while (flag == 1)
	{
		Sleep(1);
	}
	flag = 1;
	pos.X = startX;
	pos.Y = startY;
	hd = GetStdHandle(STD_OUTPUT_HANDLE);   /*��ȡ��׼����ľ��*/
	SetConsoleCursorPosition(hd, pos);      /*���ÿ���̨��������λ��*/
	//printf("the string is : %s at console(%d, %d) position !\n", buf, pos.X, pos.Y);
	printf("%s", buf);
	flag = 0;
}

//��ȡָ��λ������
Node *GetValue(int pos)
{
	Node *pro = &(list->head);	//ͷ�ڵ�
	for (int i = 0; i < pos; i++)
	{
		pro = pro->next;
	}
	return pro->next;
}


//����ͷ��
void InsertArr(int x, int y)
{
	if (list == NULL)
	{
		return;
	}

	Node *new = (Node *)malloc(sizeof(Node));
	//��ֵ
	new->data.x = x;
	new->data.y = y;
	//����
	new->next = list->head.next;
	list->head.next = new;
	//����+1
	list->size++;
}

//����ָ��λ��ɾ��
void DelArr(int pos)
{
	if (list == NULL)
	{
		return;
	}

	if (pos<0 || pos>list->size - 1)
	{
		return;
	}
	Node *pro = &(list->head);	//ͷ�ڵ�
	for (int i = 0; i < pos; i++)
	{
		pro = pro->next;
	}
	Node *tmp = pro->next;
	pro->next = tmp->next;
	tmp->next = NULL;
	free(tmp);

	list->size--;
}

//�����ж��Ƿ���������Ԫ����ͬ ����ͬ�ķ���0  û����ͬ�ķ���-1
int CmpArr(int x, int y)
{
	Node *pro = list->head.next;	//�׽ڵ�
	while (pro != NULL)
	{
		if (pro->data.x == x&&pro->data.y == y)
		{
			return 0;
		}
		pro = pro->next;
	}

	return -1;
}

//�����ж�  ����ͷ���
int CmpArr2(int x, int y)
{
	Node *pro = list->head.next->next->next;	//�׽ڵ���һ��
	while (pro != NULL)
	{
		if (pro->data.x == x&&pro->data.y == y)
		{
			return 0;
		}
		pro = pro->next;
	}

	return -1;
}

//���ƴ���1		//ֻ��ӡ�߿� �����
void HuiZhi1()
{
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (i == 0 || j == 0 || i == H - 1 || j == W - 1)
			{
				printStr(ico_0, j, i);
			}
		}
	}
}

//���ƴ���
void HuiZhi()
{

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (i == 0 || j == 0 || i == H - 1 || j == W - 1)
			{
				printStr(ico_0, j, i);
			}
			else
				printStr(" ", j, i);
		}
	}
	char buf[10] = { 0 };
	_itoa(GuanQia, buf, 10);
	printStr("��ǰ�� ", 55, 18);
	printStr(" ", 62, 18);
	printStr(buf, 62, 18);
	printStr("��", 64, 18);
}

//�����ʼ��
void InitSnake()
{
	list = (Nate *)calloc(1, sizeof(Nate));
	list->head.next = NULL;
	
	InsertArr(3, 2);
	InsertArr(3, 3);
	InsertArr(3, 4);
	//����Ĭ�ϵ��׽ڵ��������x=3 y=4
	list->size = 3;
}

//�������
void DelSnake()
{
	Node *pro = list->head.next;	//�׽ڵ�
	while (pro != NULL)
	{
		printStr(" ", pro->data.x, pro->data.y);
		pro = pro->next;
	}
}

//��������
void HuiZhiSnake()
{
	Node *pro = list->head.next;	//�׽ڵ�
	int i = 0;

	while (pro != NULL)
	{
		if (i == 0)
		{
			printStr(ico_1, pro->data.x, pro->data.y);
		}
		else if (i == 1)
		{
			printStr(ico_2, pro->data.x, pro->data.y);
		}
		else
		{
			printStr(ico_3, pro->data.x, pro->data.y);
		}
		
		pro = pro->next;
		i++;
	}

}

//����ʳ��
void HuiZhiShiWu()
{
	srand((unsigned int)time(NULL));
	int x, y;
	char buf_x[10] = { 0 };
	char buf_y[10] = { 0 };
	char buf_size[10] = { 0 };
	char buf_num[10] = { 0 };
	while (1)
	{
		x = rand() % W;//x����������
		y = rand() % H;//y���������
		if (x == W - 1 || x == 0 || y == 0 || y == H - 1)	//������߿� �ػ�
		{
			continue;
		}
		if (CmpArr(x, y) == 0)	//�������������һ����  �ػ�
		{
			continue;  
		}
		if (Func01(x, y) == 0)	//������ϰ���λ��һ�����ػ�
		{
			continue;
		}
		ShiWu.x = x;
		ShiWu.y = y;

		printStr(ico_4, ShiWu.x, ShiWu.y);
		
		//��ӡʳ������
		_itoa(ShiWu.x, buf_x, 10);
		_itoa(ShiWu.y, buf_y, 10);
		//���
		printStr("         ", 55, 3);
		printStr("  ", 64, 3);
		printStr("  ", 66, 3);
		printStr("  ", 68, 3);
		printStr("  ", 70, 3);
		//����
		printStr("ʳ������ ", 55, 3);
		printStr("x:", 64, 3);
		printStr(buf_x, 66, 3);
		printStr("y:", 68, 3);
		printStr(buf_y, 70, 3);

		
		_itoa(list->size, buf_size, 10);
		printStr("        ", 55, 6);
		printStr("   ", 64, 6);
		printStr("������:", 55, 6);
		printStr(buf_size, 64, 6);

		//��ӡʣ��ʳ�����
		_itoa(TongGuanLen - list->size + 3, buf_num, 10);
		printStr("             ", 55, 9);
		printStr("ʳ��ʣ�����:", 55, 9);
		printStr("  ", 68, 9);
		printStr(buf_num, 68, 9);

		break;
	}
	
}

//������
void FreeSnake()
{
	if (list == NULL)
	{
		return;
	}
	Node *pro = list->head.next;	//�׽ڵ�
	Node *tmp = NULL;
	while (pro != NULL)
	{
		tmp = pro->next;
		free(pro);
		pro = tmp;
	}
}

//������ʾ
void GameOver(int x)
{
	exit1 = 1;
	//1ײ������ 2.����
	if (x == 1)
	{
		//������
		FreeSnake();
		//�����߳�
		CloseHandle(hd);
		CloseHandle(hd2);
		printStr("G A M E O V E R", 55, 13);
		printStr(" ����һ��? Y/N ", 55, 14);
		char ch;
		while (1)
		{
			ch = _getch();
			if (ch == 'y' || ch == 'Y')
			{
				system("cls");
				test();
				return;
			}
			else if (ch == 'n' || ch == 'N')
			{
				exit(0);
			}
			Sleep(1000);
		}
	}
	if (x == 2)
	{	
		if (GuanQia >= 4)
		{
			system("cls");

			printStr("  �� ϲ ͨ ��  ", 32, 9);
			printStr("G A M E O V E R", 32, 13);
			
			while (1)
			{
				Sleep(1000);
			}
			return ;
		}
		//������
		FreeSnake();
		//�����߳�
		CloseHandle(hd);
		CloseHandle(hd2);
		GuanQia++;
		printStr("��ϲ����", 55, 13);
		printStr("�������������һ��", 55, 14);
		_getch();
		system("cls");

		test();
		return;
	}

}

//��ʳ���ж�
void ChiShiWu()
{
	
	switch (path)
	{
	case 1:
		if (list->head.next->data.x == ShiWu.x && list->head.next->data.y == ShiWu.y + 1)
		{
			//printStr("\01", ShiWu.x, ShiWu.y);//�����λ�ñ����һ����ͼ��
			InsertArr(ShiWu.x, ShiWu.y);	//��ͷ�������ʳ�������
			HuiZhiShiWu();	//����ʳ��
		}
		break;
	case 2:
		if (list->head.next->data.x == ShiWu.x && list->head.next->data.y == ShiWu.y - 1)
		{
			//printStr("\01", ShiWu.x, ShiWu.y);//�����λ�ñ����һ����ͼ��
			InsertArr(ShiWu.x, ShiWu.y);	//��ͷ�������ʳ�������
			HuiZhiShiWu();	//����ʳ��
		}
		break;
	case 3:
		if (list->head.next->data.x == ShiWu.x + 1 && list->head.next->data.y == ShiWu.y)
		{
			//printStr("\01", ShiWu.x, ShiWu.y);//�����λ�ñ����һ����ͼ��
			InsertArr(ShiWu.x, ShiWu.y);	//��ͷ�������ʳ�������
			HuiZhiShiWu();	//����ʳ��
		}
		break;
	case 4:
		if (list->head.next->data.x == ShiWu.x - 1 && list->head.next->data.y == ShiWu.y)
		{
			//printStr("\01", ShiWu.x, ShiWu.y);//�����λ�ñ����һ����ͼ��
			InsertArr(ShiWu.x, ShiWu.y);	//��ͷ�������ʳ�������
			HuiZhiShiWu();	//����ʳ��
		}
		break;
	default:
		break;
	}
}

//��ײ���
int JianCe()
{

	if (list->head.next == NULL)
	{
		return -2;
	}
	//ͷԪ��ײ����
	if (ChuanQiangPower == 0)	
	{
		if (list->head.next->data.x == 0 || list->head.next->data.x == W - 1 || list->head.next->data.y == 0 || list->head.next->data.y == H - 1)
		{
			GameOver(1);
			return -1;
		}
	}
	else//��ǽģʽ
	{
		if (list->head.next->data.x == 0)
		{
			//�ж�ʳ�������ǲ�������һ��
			list->head.next->data.x = W - 2;
			//���ƴ���
			HuiZhi1();
			if (ShiWu.x == W - 2 && ShiWu.y == list->head.next->data.y)
			{
				InsertArr(ShiWu.x - 1, ShiWu.y);	//��ͷ�������ʳ�������
				HuiZhiShiWu();	//����ʳ��
			}
			return 0;
		}
		if (list->head.next->data.y == 0)
		{
			list->head.next->data.y = H - 2;
			//���ƴ���
			HuiZhi1();
			if (ShiWu.x == list->head.next->data.x && ShiWu.y == H - 2)
			{
				InsertArr(ShiWu.x, ShiWu.y - 1);	//��ͷ�������ʳ�������
				HuiZhiShiWu();	//����ʳ��
			}
			return 0;
		}
		if (list->head.next->data.x == W - 1)
		{
			list->head.next->data.x = 1;
			//���ƴ���
			HuiZhi1();
			if (ShiWu.x == 1 && ShiWu.y == list->head.next->data.y)
			{
				InsertArr(ShiWu.x + 1, ShiWu.y);	//��ͷ�������ʳ�������
				HuiZhiShiWu();	//����ʳ��
			}
			return 0;
		}
		if (list->head.next->data.y == H - 1)
		{
			list->head.next->data.y = 1;
			//���ƴ���
			HuiZhi1();
			if (ShiWu.x == list->head.next->data.x && ShiWu.y == 1)
			{
				InsertArr(ShiWu.x, ShiWu.y + 1);	//��ͷ�������ʳ�������
				HuiZhiShiWu();	//����ʳ��
			}
			return 0;
		}
	}
	

	//ײ���Լ�������
	if (CmpArr2(list->head.next->data.x, list->head.next->data.y) == 0)
	{
		GameOver(1);
		return -1;
		
	}
	//ײ���ϰ���
	if (Func01(list->head.next->data.x, list->head.next->data.y) == 0)
	{
		GameOver(1);
		return -1;
	}
	//��ʳ���ж�
	ChiShiWu();

	return 0;

}

//�����ƶ��߳�
void MoveSnake()
{
	
	while (1)
	{
		if (exit1 == 1)
		{
			return;
		}
		//power����ͣʱ��==1
		while (power == 1)
		{
			Sleep(50);
		}
		switch (path)	//1�� 2�� 3�� 4��
		{
		case 1:
			//�����ƶ�
			InsertArr(list->head.next->data.x, list->head.next->data.y - 1);	//��ͷ����
			//����β�ط���ӡһ���ո� ���ͼ��
			printStr(" ", GetValue(list->size - 1)->data.x, GetValue(list->size - 1)->data.y);
			DelArr(list->size - 1);		//��βɾ��
			HuiZhiSnake();//������
			break;
		case 2:
			//�����ƶ�
			InsertArr(list->head.next->data.x, list->head.next->data.y + 1);	//��ͷ����
			//����β�ط���ӡһ���ո� ���ͼ��
			printStr(" ", GetValue(list->size - 1)->data.x, GetValue(list->size - 1)->data.y);
			DelArr(list->size - 1);		//��βɾ��
			HuiZhiSnake();//������
			break;
		case 3:
			//�����ƶ�
			InsertArr(list->head.next->data.x - 1, list->head.next->data.y);	//��ͷ����
			//����β�ط���ӡһ���ո� ���ͼ��
			printStr(" ", GetValue(list->size - 1)->data.x, GetValue(list->size - 1)->data.y);
			DelArr(list->size - 1);		//��βɾ��
			HuiZhiSnake();//������
			break;
		case 4:
			//�����ƶ�
			InsertArr(list->head.next->data.x + 1, list->head.next->data.y);	//��ͷ����
			//����β�ط���ӡһ���ո� ���ͼ��
			printStr(" ", GetValue(list->size - 1)->data.x, GetValue(list->size - 1)->data.y);
			DelArr(list->size - 1);		//��βɾ��
			HuiZhiSnake();//������
			break;
		default:
			break;
		}
		Sleep(tmp);
		if (JianCe() != 0)	//�߿���ײ���
		{
			return;
		}
		if (list->size >= TongGuanLen + 3)
		{
			GameOver(2);
			return;
		}
	}
}

//�û������߳�
void GetFangXiang()
{
	while (1)
	{
		
		if (exit1 == 1)
		{
			return;
		}
		int ch = 0;

		if (_kbhit())
		{
			ch = _getch();
			
		}

		if (ch == 'w' || ch == 'W' || ch == 72)
		{
			if (path != 2)	//���ԭ�������ƶ��� ����ֱ������
			{
				path = 1;//��
				Sleep(tmp);
			}
		}
		else if (ch == 's' || ch == 'S' || ch == 80)
		{
			if (path != 1)	//���ԭ�������ƶ��� ����ֱ������
			{
				path = 2;//��
				Sleep(tmp);
			}
		}
		else if (ch == 'a' || ch == 'A' || ch == 75)
		{
			if (path != 4)	//���ԭ�������ƶ��� ����ֱ������
			{
				path = 3;//��
				Sleep(tmp);
			}
		}
		else if (ch == 'd' || ch == 'D' || ch == 77)
		{
			if (path != 3)	//���ԭ�������ƶ��� ����ֱ������
			{
				path = 4;//��
				Sleep(tmp);
			}
		}
		else if (ch == 32)//�ո��
		{
			if (exit1 != 1)
			{
				if (power == 0)
				{
					printStr("          ", 55, 14);
					printStr("  pause...", 55, 14);
					power = 1;
				}
				else
				{
					printStr("          ", 55, 14);
					power = 0;
				}
			}
			
		}
		//o���� p����
		else if (ch == 'o' || ch == 'O')
		{
			if (tmp > 50)
			{
				tmp -= 10;
				char buf[10] = { 0 };
				_itoa(tmp, buf, 10);
				
				printStr("         ", 15, 22);
				printStr("��ǰ�ٶ�:", 15, 22);
				printStr("    ", 24, 22);
				printStr(buf, 24, 22);
				
			}
			
		}
		else if (ch == 'p' || ch == 'P')
		{
			if (tmp < 1000)
			{
				tmp += 10;
				char buf[10] = { 0 };
				_itoa(tmp, buf, 10);
				printStr("         ", 15, 22);
				printStr("��ǰ�ٶ�:", 15, 22);
				printStr("    ", 24, 22);
				printStr(buf, 24, 22);
				
			}
			
		}
		
		//Sleep(tmp / 10);
	}
}

//ȫ�ֳ�ʼ��
void test()
{
	
	system("cls");//����
	ChuanQiangPower = 1;
	TongGuanLen = 20;//ͨ�س���
	setbuf(stdin, NULL);
	exit1 = 0;//�����ı�־

	//tmp = 200;			//ˢ��Ƶ��
	path = 2;	//Ĭ�������ƶ�  //1��2��3��4��

	//���ƴ���
	HuiZhi();
	
	//��ʼ������
	InitSnake();
	
	//��������
	HuiZhiSnake();
	
	//�ϰ����ʼ��
	InitZhangAiWu();
	
	//����ʳ��
	HuiZhiShiWu();
	//��ӡ��ǰ�ٶ�
	char buf[10] = { 0 };
	_itoa(tmp, buf, 10);
	printStr("         ", 15, 22);
	printStr("��ǰ�ٶ�:", 15, 22);
	printStr("    ", 24, 22);
	printStr(buf, 24, 22);
	//���������ƶ��߳�
	hd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MoveSnake, NULL, 0, &MyThreadID);
	SetThreadPriority(hd, 10);
	//���������û������߳�
	hd2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GetFangXiang, NULL, 0, &MyThreadID);
	SetThreadPriority(hd2, 10);
}

int main(void)
{
	
	system("mode con cols=80 lines=25");//���ô��ڴ�С
	ChuanQiangPower = 0;	//��ǽģʽ
	int a, b;
	//��ӡ��ӭ�˵�
	printStr("    S   N   A   K   E    ", 23, 2);
	printStr("   w  e  l  c  o  m  e   ", 23, 4);
	printStr("WASD���Ʒ���", 29, 8);
	printStr("�ո����ͣ", 29, 9);
	printStr("O������", 29, 10);
	printStr("P������", 29, 11);
	printStr(" ��  ��  ��  ��  ��  ��  ", 25, 17);
	_getch();
	system("cls");

	printStr("                               ", 20, 8);
	printStr("��ѡ���ߵ��ٶ�(100-1000) :", 20, 8);
	while (1)
	{
		scanf("%d", &a);
		if (a >= 100 && a <= 1000)
		{
			tmp = a;
			break;
		}
		else
		{
			printStr("                                       ", 20, 10);
			printStr("ֻ������100-1000,���������� :", 20, 10);
			
		}
	}

	printStr("                      ", 20, 12);
	printStr("��ѡ��ؿ�(1-4) :", 20, 12);
	while (1)
	{
		scanf("%d", &b);
		if (b > 0 && b < 5)
		{
			GuanQia = b;	//���ó�ʼ��
			break;
		}
		else
		{
			printStr("                                  ", 20, 14);
			printStr("ֻ������1-4,���������� :", 20, 14);
			
		}
	}

	test();

	//�������ó������
	while (1)
	{
		//ѭ����������
		mciSendString(L"play BGM.mp3 wait", 0, 0, 0);
		Sleep(100);
		//Sleep(100000);
	}
	printf("\n");
	system("pause");
	return 0;
}


//�ж��Ƿ�ײ���ϰ���
int Func01(int x, int y)
{
	if (GuanQia == 1)
	{
		for (int i = 0; i < 30; i++)
		{
			if (GuanQiaData1[i].x == x&&GuanQiaData1[i].y == y)
			{
				return 0;
			}
		}
		return -1;
	}
	//��2��
	if (GuanQia == 2)
	{
		for (int i = 0; i < 60; i++)
		{
			if (GuanQiaData2[i].x == x&&GuanQiaData2[i].y == y)
			{
				return 0;
			}
		}
		return -1;
	}
	//������
	if (GuanQia == 3)
	{
		for (int i = 0; i < 65; i++)
		{
			if (GuanQiaData3[i].x == x&&GuanQiaData3[i].y == y)
			{
				return 0;
			}
		}
		return -1;
	}
	if (GuanQia == 4)
	{
		for (int i = 0; i < 77; i++)
		{
			if (GuanQiaData4[i].x == x&&GuanQiaData4[i].y == y)
			{
				return 0;
			}
		}
		return -1;
	}
	return -5;
}
//�ϰ����ʼ��
void InitZhangAiWu()
{
	//��1��
	if (GuanQia == 1)
	{
		int a = 10;
		for (int i = 0; i < 30; i++)
		{
			GuanQiaData1[i].x = a++;
			GuanQiaData1[i].y = 13;
		}
		a = 10;
		for (int i = 0; i < 30; i++)
		{
			printStr(ico_0, a++, 13);
		}
	}
	//��2��
	if (GuanQia == 2)
	{
		//���������ݸ�ֵ
		int a = 10;
		for (int i = 0; i < 30; i++)
		{
			GuanQiaData2[i].x = a++;
			GuanQiaData2[i].y = 13;
		}
		a = 10;
		for (int i = 30; i < 60; i++)
		{
			GuanQiaData2[i].x = a++;
			GuanQiaData2[i].y = 7;
		}
		//�����ӡ
		a = 10;
		for (int i = 0; i < 30; i++)
		{
			printStr(ico_0, a++, 13);
		}
		a = 10;
		for (int i = 0; i < 30; i++)
		{
			printStr(ico_0, a++, 7);
		}
	}

	//������
	if (GuanQia == 3)
	{
		//���������ݸ�ֵ
		int a = 10;
		for (int i = 0; i < 30; i++)
		{
			GuanQiaData3[i].x = a++;
			GuanQiaData3[i].y = 13;
		}
		a = 10;
		for (int i = 30; i < 60; i++)
		{
			GuanQiaData3[i].x = a++;
			GuanQiaData3[i].y = 7;
		}
		a = 8;
		for (int i = 60; i < 65; i++)
		{
			GuanQiaData3[i].x = 25;
			GuanQiaData3[i].y = a++;
		}

		//�����ӡ
		a = 10;
		for (int i = 0; i < 30; i++)
		{
			printStr(ico_0, a++, 13);
		}
		a = 10;
		for (int i = 0; i < 30; i++)
		{
			printStr(ico_0, a++, 7);
		}

		a = 8;
		for (int i = 0; i < 5; i++)
		{
			printStr(ico_0, 25, a++);
		}
	}
	//��4��
	if (GuanQia == 4)
	{
		//���������ݸ�ֵ
		int a = 10;
		for (int i = 0; i < 30; i++)
		{
			GuanQiaData4[i].x = a++;
			GuanQiaData4[i].y = 13;
		}
		a = 10;
		for (int i = 30; i < 60; i++)
		{
			GuanQiaData4[i].x = a++;
			GuanQiaData4[i].y = 7;
		}
		a = 8;
		for (int i = 60; i < 65; i++)
		{
			GuanQiaData4[i].x = 25;
			GuanQiaData4[i].y = a++;
		}
		//���ϵ���
		GuanQiaData4[65].x = 10;
		GuanQiaData4[65].y = 6;
		GuanQiaData4[66].x = 10;
		GuanQiaData4[66].y = 5;
		GuanQiaData4[67].x = 10;
		GuanQiaData4[67].y = 4;
		//���µ���
		GuanQiaData4[68].x = 10;
		GuanQiaData4[68].y = 14;
		GuanQiaData4[69].x = 10;
		GuanQiaData4[69].y = 15;
		GuanQiaData4[70].x = 10;
		GuanQiaData4[70].y = 16;

		//���ϵ���
		GuanQiaData4[71].x = 39;
		GuanQiaData4[71].y = 6;
		GuanQiaData4[72].x = 39;
		GuanQiaData4[72].y = 5;
		GuanQiaData4[73].x = 39;
		GuanQiaData4[73].y = 4;
		//���µ���
		GuanQiaData4[74].x = 39;
		GuanQiaData4[74].y = 14;
		GuanQiaData4[75].x = 39;
		GuanQiaData4[75].y = 15;
		GuanQiaData4[76].x = 39;
		GuanQiaData4[76].y = 16;
		//�����ӡ
		a = 10;
		for (int i = 0; i < 30; i++)
		{
			printStr(ico_0, a++, 13);
		}
		a = 10;
		for (int i = 0; i < 30; i++)
		{
			printStr(ico_0, a++, 7);
		}

		a = 8;
		for (int i = 0; i < 5; i++)
		{
			printStr(ico_0, 25, a++);
		}
		//��ӡ���Ϻ�
		printStr(ico_0, 10, 6);
		printStr(ico_0, 10, 5);
		printStr(ico_0, 10, 4);
		//��ӡ���º�
		printStr(ico_0, 10, 16);
		printStr(ico_0, 10, 15);
		printStr(ico_0, 10, 14);
		//��ӡ���Ϻ�
		printStr(ico_0, 39, 6);
		printStr(ico_0, 39, 5);
		printStr(ico_0, 39, 4);
		//��ӡ���º�
		printStr(ico_0, 39, 16);
		printStr(ico_0, 39, 15);
		printStr(ico_0, 39, 14);

	}
}