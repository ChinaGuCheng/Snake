
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include<time.h>
#include<conio.h>	//控制台输入输出

//#include "mmsystem.h"//导入声音头文件 
#pragma comment(lib,"winmm.lib")//导入声音头文件库 

//蛇单个坐标数据
typedef struct coordinate
{
	int x;
	int y;
}nate;

//装单个坐标的节点
typedef struct Node
{
	nate data;
	struct Node *next;
}Node;

//蛇的主体    链表的头节点
typedef struct Nate
{
	Node head;
	int size;
}Nate;



#define W 50//一行50个
#define H 20//一列20个

int GuanQia = 1;	//当前关卡

nate GuanQiaData1[30] = { 0 };//第1关的障碍物坐标数据
nate GuanQiaData2[60] = { 0 };//第2关的障碍物坐标数据
nate GuanQiaData3[65] = { 0 };//第3关的障碍物坐标数据
nate GuanQiaData4[77] = { 0 };//第4关的障碍物坐标数据

int TongGuanLen = 20;//通关长度

int power = 0;//暂停的开关  默认假
int exit1 = 0;	//结束的标志
Nate *list;		//蛇数据
int MyThreadID;
nate ShiWu;		//食物的坐标
HANDLE hd;		//线程句柄
HANDLE hd2;		//线程2句柄
int path = 2;	//控制蛇移动的方向 1上 2下 3左 4右
int tmp = 200;//刷新频率

int ChuanQiangPower = 0;//穿墙模式 0为假 1为真

//图标
//int ic = ;
//char a = 178;
//char ico_0[3] = { 161,0 };

char *ico_0 = "\01";		//边框图标
char *ico_1 = "\02";	//蛇头图案
char *ico_2 = "\04";	//蛇头后面一个图案
char *ico_3 = "\05";	//除前面2个所有图案
char *ico_4 = "\03";	//食物图案

//char *ico_0 = "X";		//边框图标
//char *ico_1 = "O";	//蛇头图案
//char *ico_2 = "~";	//蛇头后面一个图案
//char *ico_3 = "#";	//除前面2个所有图案
//char *ico_4 = "$";	//食物图案

void test();

//障碍物初始化
void InitZhangAiWu();

//判断是否撞上障碍物
int Func01(int x, int y);

/*
*function: 在控制台指定的位置输出字符串
*参数：buf,输出的字符串；startX、startY为控制台的X，Y坐标
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
	hd = GetStdHandle(STD_OUTPUT_HANDLE);   /*获取标准输出的句柄*/
	SetConsoleCursorPosition(hd, pos);      /*设置控制台光标输出的位置*/
	//printf("the string is : %s at console(%d, %d) position !\n", buf, pos.X, pos.Y);
	printf("%s", buf);
	flag = 0;
}

//获取指定位置坐标
Node *GetValue(int pos)
{
	Node *pro = &(list->head);	//头节点
	for (int i = 0; i < pos; i++)
	{
		pro = pro->next;
	}
	return pro->next;
}


//链表头插
void InsertArr(int x, int y)
{
	if (list == NULL)
	{
		return;
	}

	Node *new = (Node *)malloc(sizeof(Node));
	//赋值
	new->data.x = x;
	new->data.y = y;
	//入链
	new->next = list->head.next;
	list->head.next = new;
	//个数+1
	list->size++;
}

//链表指定位置删除
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
	Node *pro = &(list->head);	//头节点
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

//遍历判断是否与数组中元素相同 有相同的返回0  没有相同的返回-1
int CmpArr(int x, int y)
{
	Node *pro = list->head.next;	//首节点
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

//遍历判断  除蛇头外的
int CmpArr2(int x, int y)
{
	Node *pro = list->head.next->next->next;	//首节点下一个
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

//绘制窗口1		//只打印边框 不清空
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

//绘制窗口
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
	printStr("当前第 ", 55, 18);
	printStr(" ", 62, 18);
	printStr(buf, 62, 18);
	printStr("关", 64, 18);
}

//蛇身初始化
void InitSnake()
{
	list = (Nate *)calloc(1, sizeof(Nate));
	list->head.next = NULL;
	
	InsertArr(3, 2);
	InsertArr(3, 3);
	InsertArr(3, 4);
	//链表默认的首节点的坐标是x=3 y=4
	list->size = 3;
}

//清除蛇身
void DelSnake()
{
	Node *pro = list->head.next;	//首节点
	while (pro != NULL)
	{
		printStr(" ", pro->data.x, pro->data.y);
		pro = pro->next;
	}
}

//绘制蛇身
void HuiZhiSnake()
{
	Node *pro = list->head.next;	//首节点
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

//绘制食物
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
		x = rand() % W;//x坐标的随机数
		y = rand() % H;//y坐标随机数
		if (x == W - 1 || x == 0 || y == 0 || y == H - 1)	//随机到边框 重绘
		{
			continue;
		}
		if (CmpArr(x, y) == 0)	//如果跟蛇身坐标一样就  重绘
		{
			continue;  
		}
		if (Func01(x, y) == 0)	//如果跟障碍物位置一样就重绘
		{
			continue;
		}
		ShiWu.x = x;
		ShiWu.y = y;

		printStr(ico_4, ShiWu.x, ShiWu.y);
		
		//打印食物坐标
		_itoa(ShiWu.x, buf_x, 10);
		_itoa(ShiWu.y, buf_y, 10);
		//清除
		printStr("         ", 55, 3);
		printStr("  ", 64, 3);
		printStr("  ", 66, 3);
		printStr("  ", 68, 3);
		printStr("  ", 70, 3);
		//绘制
		printStr("食物坐标 ", 55, 3);
		printStr("x:", 64, 3);
		printStr(buf_x, 66, 3);
		printStr("y:", 68, 3);
		printStr(buf_y, 70, 3);

		
		_itoa(list->size, buf_size, 10);
		printStr("        ", 55, 6);
		printStr("   ", 64, 6);
		printStr("蛇身长度:", 55, 6);
		printStr(buf_size, 64, 6);

		//打印剩余食物个数
		_itoa(TongGuanLen - list->size + 3, buf_num, 10);
		printStr("             ", 55, 9);
		printStr("食物剩余个数:", 55, 9);
		printStr("  ", 68, 9);
		printStr(buf_num, 68, 9);

		break;
	}
	
}

//销毁蛇
void FreeSnake()
{
	if (list == NULL)
	{
		return;
	}
	Node *pro = list->head.next;	//首节点
	Node *tmp = NULL;
	while (pro != NULL)
	{
		tmp = pro->next;
		free(pro);
		pro = tmp;
	}
}

//结束提示
void GameOver(int x)
{
	exit1 = 1;
	//1撞到东西 2.过关
	if (x == 1)
	{
		//销毁蛇
		FreeSnake();
		//结束线程
		CloseHandle(hd);
		CloseHandle(hd2);
		printStr("G A M E O V E R", 55, 13);
		printStr(" 再来一局? Y/N ", 55, 14);
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

			printStr("  恭 喜 通 关  ", 32, 9);
			printStr("G A M E O V E R", 32, 13);
			
			while (1)
			{
				Sleep(1000);
			}
			return ;
		}
		//销毁蛇
		FreeSnake();
		//结束线程
		CloseHandle(hd);
		CloseHandle(hd2);
		GuanQia++;
		printStr("恭喜过关", 55, 13);
		printStr("按任意键进入下一关", 55, 14);
		_getch();
		system("cls");

		test();
		return;
	}

}

//吃食物判断
void ChiShiWu()
{
	
	switch (path)
	{
	case 1:
		if (list->head.next->data.x == ShiWu.x && list->head.next->data.y == ShiWu.y + 1)
		{
			//printStr("\01", ShiWu.x, ShiWu.y);//让这个位置变成蛇一样的图案
			InsertArr(ShiWu.x, ShiWu.y);	//蛇头插入这个食物的坐标
			HuiZhiShiWu();	//绘制食物
		}
		break;
	case 2:
		if (list->head.next->data.x == ShiWu.x && list->head.next->data.y == ShiWu.y - 1)
		{
			//printStr("\01", ShiWu.x, ShiWu.y);//让这个位置变成蛇一样的图案
			InsertArr(ShiWu.x, ShiWu.y);	//蛇头插入这个食物的坐标
			HuiZhiShiWu();	//绘制食物
		}
		break;
	case 3:
		if (list->head.next->data.x == ShiWu.x + 1 && list->head.next->data.y == ShiWu.y)
		{
			//printStr("\01", ShiWu.x, ShiWu.y);//让这个位置变成蛇一样的图案
			InsertArr(ShiWu.x, ShiWu.y);	//蛇头插入这个食物的坐标
			HuiZhiShiWu();	//绘制食物
		}
		break;
	case 4:
		if (list->head.next->data.x == ShiWu.x - 1 && list->head.next->data.y == ShiWu.y)
		{
			//printStr("\01", ShiWu.x, ShiWu.y);//让这个位置变成蛇一样的图案
			InsertArr(ShiWu.x, ShiWu.y);	//蛇头插入这个食物的坐标
			HuiZhiShiWu();	//绘制食物
		}
		break;
	default:
		break;
	}
}

//碰撞检测
int JianCe()
{

	if (list->head.next == NULL)
	{
		return -2;
	}
	//头元素撞到边
	if (ChuanQiangPower == 0)	
	{
		if (list->head.next->data.x == 0 || list->head.next->data.x == W - 1 || list->head.next->data.y == 0 || list->head.next->data.y == H - 1)
		{
			GameOver(1);
			return -1;
		}
	}
	else//穿墙模式
	{
		if (list->head.next->data.x == 0)
		{
			//判断食物坐标是不是在另一边
			list->head.next->data.x = W - 2;
			//绘制窗口
			HuiZhi1();
			if (ShiWu.x == W - 2 && ShiWu.y == list->head.next->data.y)
			{
				InsertArr(ShiWu.x - 1, ShiWu.y);	//蛇头插入这个食物的坐标
				HuiZhiShiWu();	//绘制食物
			}
			return 0;
		}
		if (list->head.next->data.y == 0)
		{
			list->head.next->data.y = H - 2;
			//绘制窗口
			HuiZhi1();
			if (ShiWu.x == list->head.next->data.x && ShiWu.y == H - 2)
			{
				InsertArr(ShiWu.x, ShiWu.y - 1);	//蛇头插入这个食物的坐标
				HuiZhiShiWu();	//绘制食物
			}
			return 0;
		}
		if (list->head.next->data.x == W - 1)
		{
			list->head.next->data.x = 1;
			//绘制窗口
			HuiZhi1();
			if (ShiWu.x == 1 && ShiWu.y == list->head.next->data.y)
			{
				InsertArr(ShiWu.x + 1, ShiWu.y);	//蛇头插入这个食物的坐标
				HuiZhiShiWu();	//绘制食物
			}
			return 0;
		}
		if (list->head.next->data.y == H - 1)
		{
			list->head.next->data.y = 1;
			//绘制窗口
			HuiZhi1();
			if (ShiWu.x == list->head.next->data.x && ShiWu.y == 1)
			{
				InsertArr(ShiWu.x, ShiWu.y + 1);	//蛇头插入这个食物的坐标
				HuiZhiShiWu();	//绘制食物
			}
			return 0;
		}
	}
	

	//撞到自己身体了
	if (CmpArr2(list->head.next->data.x, list->head.next->data.y) == 0)
	{
		GameOver(1);
		return -1;
		
	}
	//撞上障碍物
	if (Func01(list->head.next->data.x, list->head.next->data.y) == 0)
	{
		GameOver(1);
		return -1;
	}
	//吃食物判断
	ChiShiWu();

	return 0;

}

//蛇身移动线程
void MoveSnake()
{
	
	while (1)
	{
		if (exit1 == 1)
		{
			return;
		}
		//power在暂停时会==1
		while (power == 1)
		{
			Sleep(50);
		}
		switch (path)	//1上 2下 3左 4右
		{
		case 1:
			//往上移动
			InsertArr(list->head.next->data.x, list->head.next->data.y - 1);	//蛇头插入
			//在蛇尾地方打印一个空格 清除图标
			printStr(" ", GetValue(list->size - 1)->data.x, GetValue(list->size - 1)->data.y);
			DelArr(list->size - 1);		//蛇尾删除
			HuiZhiSnake();//绘制蛇
			break;
		case 2:
			//往下移动
			InsertArr(list->head.next->data.x, list->head.next->data.y + 1);	//蛇头插入
			//在蛇尾地方打印一个空格 清除图标
			printStr(" ", GetValue(list->size - 1)->data.x, GetValue(list->size - 1)->data.y);
			DelArr(list->size - 1);		//蛇尾删除
			HuiZhiSnake();//绘制蛇
			break;
		case 3:
			//往左移动
			InsertArr(list->head.next->data.x - 1, list->head.next->data.y);	//蛇头插入
			//在蛇尾地方打印一个空格 清除图标
			printStr(" ", GetValue(list->size - 1)->data.x, GetValue(list->size - 1)->data.y);
			DelArr(list->size - 1);		//蛇尾删除
			HuiZhiSnake();//绘制蛇
			break;
		case 4:
			//往右移动
			InsertArr(list->head.next->data.x + 1, list->head.next->data.y);	//蛇头插入
			//在蛇尾地方打印一个空格 清除图标
			printStr(" ", GetValue(list->size - 1)->data.x, GetValue(list->size - 1)->data.y);
			DelArr(list->size - 1);		//蛇尾删除
			HuiZhiSnake();//绘制蛇
			break;
		default:
			break;
		}
		Sleep(tmp);
		if (JianCe() != 0)	//边框碰撞检测
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

//用户输入线程
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
			if (path != 2)	//如果原来往下移动的 不能直接往上
			{
				path = 1;//上
				Sleep(tmp);
			}
		}
		else if (ch == 's' || ch == 'S' || ch == 80)
		{
			if (path != 1)	//如果原来往上移动的 不能直接往下
			{
				path = 2;//下
				Sleep(tmp);
			}
		}
		else if (ch == 'a' || ch == 'A' || ch == 75)
		{
			if (path != 4)	//如果原来往右移动的 不能直接往左
			{
				path = 3;//左
				Sleep(tmp);
			}
		}
		else if (ch == 'd' || ch == 'D' || ch == 77)
		{
			if (path != 3)	//如果原来往左移动的 不能直接往右
			{
				path = 4;//右
				Sleep(tmp);
			}
		}
		else if (ch == 32)//空格键
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
		//o加速 p减速
		else if (ch == 'o' || ch == 'O')
		{
			if (tmp > 50)
			{
				tmp -= 10;
				char buf[10] = { 0 };
				_itoa(tmp, buf, 10);
				
				printStr("         ", 15, 22);
				printStr("当前速度:", 15, 22);
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
				printStr("当前速度:", 15, 22);
				printStr("    ", 24, 22);
				printStr(buf, 24, 22);
				
			}
			
		}
		
		//Sleep(tmp / 10);
	}
}

//全局初始化
void test()
{
	
	system("cls");//清屏
	ChuanQiangPower = 1;
	TongGuanLen = 20;//通关长度
	setbuf(stdin, NULL);
	exit1 = 0;//结束的标志

	//tmp = 200;			//刷新频率
	path = 2;	//默认往下移动  //1上2下3左4右

	//绘制窗口
	HuiZhi();
	
	//初始化蛇身
	InitSnake();
	
	//绘制蛇身
	HuiZhiSnake();
	
	//障碍物初始化
	InitZhangAiWu();
	
	//绘制食物
	HuiZhiShiWu();
	//打印当前速度
	char buf[10] = { 0 };
	_itoa(tmp, buf, 10);
	printStr("         ", 15, 22);
	printStr("当前速度:", 15, 22);
	printStr("    ", 24, 22);
	printStr(buf, 24, 22);
	//创建蛇身移动线程
	hd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MoveSnake, NULL, 0, &MyThreadID);
	SetThreadPriority(hd, 10);
	//创建监视用户输入线程
	hd2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GetFangXiang, NULL, 0, &MyThreadID);
	SetThreadPriority(hd2, 10);
}

int main(void)
{
	
	system("mode con cols=80 lines=25");//设置窗口大小
	ChuanQiangPower = 0;	//穿墙模式
	int a, b;
	//打印欢迎菜单
	printStr("    S   N   A   K   E    ", 23, 2);
	printStr("   w  e  l  c  o  m  e   ", 23, 4);
	printStr("WASD控制方向", 29, 8);
	printStr("空格键暂停", 29, 9);
	printStr("O键加速", 29, 10);
	printStr("P键减速", 29, 11);
	printStr(" 按  任  意  键  继  续  ", 25, 17);
	_getch();
	system("cls");

	printStr("                               ", 20, 8);
	printStr("请选择蛇的速度(100-1000) :", 20, 8);
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
			printStr("只能输入100-1000,请重新输入 :", 20, 10);
			
		}
	}

	printStr("                      ", 20, 12);
	printStr("请选择关卡(1-4) :", 20, 12);
	while (1)
	{
		scanf("%d", &b);
		if (b > 0 && b < 5)
		{
			GuanQia = b;	//设置初始关
			break;
		}
		else
		{
			printStr("                                  ", 20, 14);
			printStr("只能输入1-4,请重新输入 :", 20, 14);
			
		}
	}

	test();

	//阻塞不让程序结束
	while (1)
	{
		//循环播放音乐
		mciSendString(L"play BGM.mp3 wait", 0, 0, 0);
		Sleep(100);
		//Sleep(100000);
	}
	printf("\n");
	system("pause");
	return 0;
}


//判断是否撞上障碍物
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
	//第2关
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
	//第三关
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
//障碍物初始化
void InitZhangAiWu()
{
	//第1关
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
	//第2关
	if (GuanQia == 2)
	{
		//给数组内容赋值
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
		//下面打印
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

	//第三关
	if (GuanQia == 3)
	{
		//给数组内容赋值
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

		//下面打印
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
	//第4关
	if (GuanQia == 4)
	{
		//给数组内容赋值
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
		//左上的竖
		GuanQiaData4[65].x = 10;
		GuanQiaData4[65].y = 6;
		GuanQiaData4[66].x = 10;
		GuanQiaData4[66].y = 5;
		GuanQiaData4[67].x = 10;
		GuanQiaData4[67].y = 4;
		//左下的竖
		GuanQiaData4[68].x = 10;
		GuanQiaData4[68].y = 14;
		GuanQiaData4[69].x = 10;
		GuanQiaData4[69].y = 15;
		GuanQiaData4[70].x = 10;
		GuanQiaData4[70].y = 16;

		//右上的竖
		GuanQiaData4[71].x = 39;
		GuanQiaData4[71].y = 6;
		GuanQiaData4[72].x = 39;
		GuanQiaData4[72].y = 5;
		GuanQiaData4[73].x = 39;
		GuanQiaData4[73].y = 4;
		//右下的竖
		GuanQiaData4[74].x = 39;
		GuanQiaData4[74].y = 14;
		GuanQiaData4[75].x = 39;
		GuanQiaData4[75].y = 15;
		GuanQiaData4[76].x = 39;
		GuanQiaData4[76].y = 16;
		//下面打印
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
		//打印左上横
		printStr(ico_0, 10, 6);
		printStr(ico_0, 10, 5);
		printStr(ico_0, 10, 4);
		//打印左下横
		printStr(ico_0, 10, 16);
		printStr(ico_0, 10, 15);
		printStr(ico_0, 10, 14);
		//打印右上横
		printStr(ico_0, 39, 6);
		printStr(ico_0, 39, 5);
		printStr(ico_0, 39, 4);
		//打印右下横
		printStr(ico_0, 39, 16);
		printStr(ico_0, 39, 15);
		printStr(ico_0, 39, 14);

	}
}