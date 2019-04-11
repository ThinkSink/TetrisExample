#include <stdio.h>
#include<windows.h> 
#include <conio.h>
#include<time.h>


using namespace std;

//地图数组
int map[250];
//I型方块
int blockI[4] = { 1,1,1,1};
//T型方块
int blockT[9] = { 0,0,0,1,1,1,0,1,0 };
//L型方块
int blockL[9] = { 0,1,0,0,1,0,0,1,1 };
//O型方块
int blockO[9] = { 0,0,0,0,1,1,0,1,1 };
//S型方块
int blockS[9] = { 0,0,0,0,1,1,1,1,0};
//Z型方块
int blockZ[9] = { 0,0,0,1,1,0,0,1,1};
//J型方块
int blockJ[9] = { 0,1,0,0,1,0,1,1,0 };
//下一个方块
int nextblock[2];
//游戏结束标志 1为游戏进行中，0为游戏失败
int endgame;
//记录行进中方块形状和位置,xblock[0]记录形状，0为I型，1为其他型,2为O型
int xblock[2];
//计分
int score;
//初始化数据
void init();
//将光标移动到指定位置
void gotoxy(int x, int y);
//绘制边框
void drawborder();
//绘制地图
void drawmap();
//方块生成
int ranblock();
//绘制方块
void drawblock(int block[9]);
//绘制下一个方块
void drawnextblock(int x);
//向地图上绘制方块
void drawmapblock(int block[9]);
//方块下落
void downblock();
//核心逻辑
void run();

//初始化数据
void init()
{
	//map数组初始化赋值，0-239赋值为0，240-249赋值为2
	for (int i = 0; i < 240; i++)	
		map[i] = 0;
	for (int i = 240; i < 250; i++)
		map[i] = 2;
	endgame = 1;
	nextblock[0] = 0;
	nextblock[1] = ranblock();
	xblock[0] = 0;
	xblock[1] = 4;
	score = 0;
	drawborder();

}
//将光标移动到指定位置
void gotoxy(int x,int y)
{
	COORD c;
	c.X = x+23 ;
	c.Y = y+2;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, c);
	CONSOLE_CURSOR_INFO CursorInfo;
	//获取控制台光标信息
	GetConsoleCursorInfo(handle, &CursorInfo);
	//隐藏控制台光标,设置控制台光标状态
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);
}
//绘制边框
void drawborder()
{
	//绘制地图边框
	for (int i = -1; i < 21; i++)
	{
		gotoxy(i, -1);
		printf("-");
		gotoxy(i, 20);
		printf("-");
		if (i < 19) {
			gotoxy(-2, i + 1);
			//全角竖线
			printf("丨");
			gotoxy(20, i + 1);
			printf("丨");
		}
	}
	//绘制预览方块边框
	for (int i = 0; i < 10; i++)
	{
		gotoxy(i + 30, 1);
		printf("-");
		gotoxy(i + 30, 6);
		printf("-");
		if (i < 4)
		{
			gotoxy(29,i+2);
			printf("丨");
			gotoxy(39, i + 2);
			printf("丨");
		}
	}
	gotoxy(30, 8);
	printf("下一个方块");
	gotoxy(25, 19);
	printf("制作者：意之消沉");
}
//绘制地图
void drawmap()
{
	gotoxy(25, 14);
	printf("                   ");
	gotoxy(25, 14);
	printf("游戏得分为:%d",score);
	for (int i = 40; i < 240; i++)
	{
		gotoxy(i%10*2, i/10 - 4);
		//map数组中0为空
		if (map[i] == 0)
			printf(" ");
		//1为移动中方块
		if (map[i]== 1)
			printf("口");
		//2为停止的方块
		if (map[i] == 2) 
			printf("口");
	}
}

//随机方块生成
int ranblock() {
	int x;
	srand((int)time(0));
	x = rand() % 7;
	return x;
}
//绘制方块(除了I型的其他方块)
void drawblock(int block[9]) {
	for (int i = 0; i < 3; i++)
	{
		gotoxy(i * 2 + 31, 3);
		if (block[i] == 0)		
			printf(" ");	
		else
			printf("口");
		gotoxy(i * 2 + 31, 4);
		if (block[i + 3] == 0)
			printf(" ");
		else
			printf("口");
		gotoxy(i * 2 + 31, 5);
		if (block[i + 6] == 0)
			printf(" ");
		else
			printf("口");
	}
}
//绘制下一个方块
void drawnextblock(int x) 
{
	for (int i = 0; i < 4; i++)
	{
		gotoxy(i * 2 + 31, 2);
		printf(" ");
		gotoxy(i * 2 + 31, 3);
		printf(" ");
		gotoxy(i * 2 + 31, 4);
		printf(" ");
		gotoxy(i * 2 + 31, 5);
		printf(" ");
	}
	switch (x)
	{
	case 0:
		gotoxy(31, 4);
		printf("口");
		gotoxy(33, 4);
		printf("口");
		gotoxy(35, 4);
		printf("口");
		gotoxy(37, 4);
		printf("口");
		break;
	case 1:
		drawblock(blockT);
		break;
	case 2:
		drawblock(blockL);
		break;
	case 3:
		drawblock(blockO);
		break;
	case 4:
		drawblock(blockS);
		break;
	case 5:
		drawblock(blockZ);
		break;
	case 6:
		drawblock(blockJ);
		break;
	}
}

//向地图上绘制方块
void drawmapblock(int block[9])
{
	for (int i = 0; i < 3; i++)
	{
		map[i + 13] = block[i];
		map[i + 23] = block[i + 3];
		map[i + 33] = block[i + 6];
	}
	//记录方块坐标和类型
	xblock[0] = 1;
	xblock[1] = 24;
}
//方块下落
void downblock()
{
	//碰撞检测标识
	int touch = 1;
	for (int i = 239; i >= 0; i--)
	{
		if (map[i] == 1 && map[i + 10] == 2)
			touch = 0;
	}
	if (touch == 1)
	{
		for (int i = 239; i >= 0; i--)
		{
			if (map[i] == 1)
			{
				map[i + 10] = map[i];
				map[i] = 0;
			}
		}
		xblock[1] = xblock[1] + 10;
	}
	else if (touch == 0)
	{
		for (int i = 0; i < 240; i++)
		{
			if (map[i] == 1)
				map[i] = 2;
		}
	}
}


//核心逻辑
void run()
{
	while (endgame)
	{
		//游戏结束
		for (int i=20; i < 30; i++)
		{
			if (map[i] == 2||map[i+10]==2)
			{
				gotoxy(8, 6);
				printf("                           ");
				gotoxy(8, 7);
				printf("                           ");
				gotoxy(8, 8);
				printf("                          ");
				gotoxy(8, 7);
				printf("  游戏结束，按任意键退出  ");
				_getch();
				endgame = 0;
			}
		}
		//方块消除
		for (int i = 0; i < 24; i++)
		{
			int sign = 0;
			for (int j = 0; j < 10; j++)
			{
				if (map[i * 10 + j] == 2)
					sign++;
			}
			if (sign == 10)
			{
				for (int j = 0; j < 10; j++)
				{
					map[i * 10 + j] = 0;
				}
				for (int z = i*10+9; z >=0; z--)
				{
					if (map[z] == 2)
					{
						map[z + 10] = 2;
						map[z] = 0;
					}
				}
				score++;
				drawmap();
			}
		}
		//新方块生成标识
		int newblock = 1;
		for (int i = 0; i < 240; i++)
		{
			if (map[i] == 1)
				newblock = 0;
		}
		//生成新方块
		if (newblock == 1)
		{
			switch (nextblock[0])
			{
			case 0:
				map[23] = 1;
				map[24] = 1;
				map[25] = 1;
				map[26] = 1;
				xblock[0] = 0;
				xblock[1] = 24;
				break;
			case 1:
				drawmapblock(blockT);
				break;
			case 2:
				drawmapblock(blockL);
				break;
			case 3:
				drawmapblock(blockO);
				xblock[0] = 2;
				break;
			case 4:
				drawmapblock(blockS);
				break;
			case 5:
				drawmapblock(blockZ);
				break;
			case 6:
				drawmapblock(blockJ);
				break;
			}
			nextblock[0] = nextblock[1];
			drawnextblock(nextblock[0]);
			nextblock[1] = ranblock();

		}
		else if (newblock == 0)
		{
			//方向控制，形态变换
			int t = 20000,t1=0;
			while (t>0)
			{
				int ch;
				if (t1<10&&_kbhit())
				{
					//需要调用两次，因为功能键第一次返回0或224
					ch = _getch();
					ch = _getch();
					//按下左键，判断左边是否有已经下落的方块和是否已到达左边界，如果没有将正在下落的方块左移
					if (ch == 75)
					{
						
							for (int i = 0; i < 24; i++)
							{
								if (map[i * 10] == 1)
									ch = 0;
							}
							if (ch!=0&&xblock[0] == 0&&map[xblock[1]+1]==1)
							{
								if (map[xblock[1] - 2] == 2)
									ch = 0;
							}
							if (ch != 0 && xblock[0] == 0 && map[xblock[1] - 10] == 1)
							{
								if (map[xblock[1] - 1] == 2)
									ch = 0;
							}
							if (ch != 0 && xblock[0] != 0)
							{
								for (int i = xblock[1] - 11; i < xblock[1] - 8; i++)
								{
									if ((map[i] == 1 && map[i - 1] == 2)|| (map[i + 10] == 1 && map[i + 9] == 2)||( map[i + 20] == 1 && map[i + 19] == 2))
										ch = 0;
								}
							}
							if (ch != 0)
							{
								for (int i = 0; i < 240; i++)
								{
									if (map[i] == 1)
									{
										map[i - 1] = map[i];
										map[i] = 0;
									}
								}
								xblock[1] = xblock[1] - 1;
								drawmap();
							}
							
					}
					//按下上按键，判断形态变换后是否重叠已经停止的方块，如没有形态变换
					if (ch == 72)
					{

						for (int i = 0; i < 24; i++)
						{
							if (i * 10 == xblock[1] || (i * 10 + 9) == xblock[1])
								ch = 0;
						}

						if (ch != 0 && xblock[0] == 0 && map[xblock[1] + 1] == 1 && map[xblock[1] + 10] != 2 && map[xblock[1] - 10] != 2 && map[xblock[1] - 20] != 2)
						{
							map[xblock[1] + 1] = 0;
							map[xblock[1] + 2] = 0;
							map[xblock[1] - 1] = 0;
							map[xblock[1] + 10] = 1;
							map[xblock[1] - 10] = 1;
							map[xblock[1] - 20] = 1;
							ch = 0;
						}
						if (ch != 0 && xblock[0] == 0 && map[xblock[1] - 10] == 1 && map[xblock[1] + 1] != 2 && map[xblock[1] + 2] != 2 && map[xblock[1] - 1] != 2)
						{
							map[xblock[1] + 1] = 1;
							map[xblock[1] + 2] = 1;
							map[xblock[1] - 1] = 1;
							map[xblock[1] + 10] = 0;
							map[xblock[1] - 10] = 0;
							map[xblock[1] - 20] = 0;
						}
						if (xblock[0] == 2)
							ch = 0;
						int tempblock[3][3];
						if (ch != 0 && xblock[0] == 1)
						{
							for (int i = 0; i < 3; i++)
							{
								for (int j = 0; j < 3; j++)
								{
									if (map[xblock[1] - 11 + j+i*10] != 2)
										tempblock[i][j] = map[xblock[1] - 11 + j + i * 10];
									else
										tempblock[i][j] = 0;
								}
							}
							for (int i = 0; i < 3; i++)
							{
								for (int j = 0; j < 3; j++)
								{
									if (tempblock[j][2 - i] == 1 && map[xblock[1] - 11 + j + i * 10] == 2)
										ch = 0;
								}
							}
						}
						if (ch != 0 && xblock[0] == 1)
						{
							for (int i = 0; i < 3; i++)
							{
								for (int j = 0; j < 3; j++)
								{
									if ( map[xblock[1] - 11 + j + i * 10] != 2)
										map[xblock[1] - 11 + j + i * 10] = tempblock[j][2-i];
								}
							}
						}
						drawmap();
						t1 = t1 + 1;
					}
					//按下右键，判断右边是否有已经下落的方块和是否已到达右边界，如果没有将正在下落的方块右移
					if (ch == 77)
					{
						for (int i = 0; i < 24; i++)
						{
							if (map[i * 10 + 9] == 1)
								ch = 0;

						}
						if (ch != 0 && xblock[0] == 0 && map[xblock[1] + 1] == 1)
						{
							if (map[xblock[1] +3] == 2)
								ch = 0;
						}
						if (ch != 0 && xblock[0] == 0 && map[xblock[1] - 10] == 1)
						{
							if (map[xblock[1] + 1] == 2)
								ch = 0;
						}
						if (ch != 0 && xblock[0] != 0)
						{
							for (int i = xblock[1] - 11; i < xblock[1] - 8; i++)
							{
								if ((map[i] == 1 && map[i + 1] == 2) || (map[i + 10] == 1 && map[i + 11] == 2) || (map[i + 20] == 1 && map[i + 21] == 2))
									ch = 0;
							}
						}
						if (ch != 0)
						{
							for (int i = 239; i >= 0; i--)
							{
								if (map[i] == 1)
								{
									map[i + 1] = map[i];
									map[i] = 0;
								}
							}
							xblock[1] = xblock[1] + 1;
								
						}
						drawmap();
					}
					//按下按键，方块下落
					if (ch == 80)
					{
						downblock();
						drawmap();
						break;
					}
				}	
				t = t - 1;
			}
			//自动下落
			if (t <= 0)
			{
				downblock();
				drawmap();
			}
			
		}	
	}
}

//入口函数
int main()
{
	init();
	run();
	return 0;
}