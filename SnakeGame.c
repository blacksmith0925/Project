#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

const int SNAKE_LENGTH = 3;
int WaitTime = 500;

char Dire = 77;

//隐藏光标
void Hide_Cursor(HANDLE hConsole)
{
    CONSOLE_CURSOR_INFO cursor_info = {1,0};
    SetConsoleCursorInfo(hConsole,&cursor_info);
}

//设置光标位置
void Goto(int x,int y)
{
    short a = (short) x*2;
    COORD pos = {a, (short)y };
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole,pos);
    Hide_Cursor(hConsole);
}

//设置颜色
void SetColor(int color)
{
    HANDLE hConsole  = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,color);
}

//在指定位置打印符号
void PStar(int x,int y)
{
    Goto(x,y);
    printf("*");
}
void PBox(int x,int y)
{
    Goto(x,y);
    printf("■");
}
void PCircle(int x,int y){
    Goto(x,y);
    printf("●");
}

//界面初始化函数
void PInterface()
{
    int i=0;
    SetColor(15);
    for(;i<50;i++){
        if(i<=39){
            PStar(0,i);//打印竖线
            PStar(39,i);
            PStar(49,i);
            PStar(i,0);//打印横线
            PStar(i,39);
        }else{
		PStar(i,0);//打印横线
		PStar(i,39);
		}
    }
    Goto(42,4);//打印信息
	printf("贪吃蛇游戏");
	Goto(42,33);
	printf("上下左右");
	Goto(42,35);
	printf("控制移动");

}

//初始化界面
void Init()
{
    system("title 贪吃蛇");
    system("mode con: cols=100 lines=40");
    PInterface();
}

//初始化蛇身
void InitSnake(int (*Snake)[2],int SnakeLength){
    int i =0;
    for(;i<SnakeLength;i++){
        Snake[i][0] = 12-i;
        Snake[i][1] = 4;
        SetColor(10);
        PBox(Snake[i][0],Snake[i][1]);
    }
}

//产生食物
void CreateFood(int *Food,int Snake[400][2],int SnakeLength){
    int Cfood[2];
    srand((unsigned)time(NULL));
    for(;;){
        int flag =1;
        int i;
        Cfood[0] = rand()%38+1; //生成位置
        Cfood[1] = rand()%38+1;
        for(i=0;i<SnakeLength;i++){
            if(Cfood[0]==Snake[i][0]&&Cfood[1]==Snake[i][1]){       //判断随机生成的果实的位置是否与蛇身的位置重叠，如果重叠则重新生成位置
                flag =0;
                break;
            }
        }
        if(flag==1)//产生了符合要求的食物坐标
        {
            SetColor(12);
			PCircle(Cfood[0],Cfood[1]);//打印食物
			Food[0] = Cfood[0];//为食物赋值
			Food[1] = Cfood[1];
			break;
        }
    }
}

//打印分数
void PScore(int SnakeLength){
    int score = (SnakeLength-SNAKE_LENGTH)*10;//当前蛇身长度减去初始值再*10
	Goto(43,15);
	printf("得分");
	Goto(43,17);
	printf("%d",score);
}

//游戏结束函数
void GameOver(int (*Snake)[2],int *SnakeLength,int *Food){
    Goto(10,20);
	printf("游戏结束！游戏将在三秒后重新开始！");
	Sleep(3000);//暂停三秒
	system("cls");//清空屏幕
	*SnakeLength = SNAKE_LENGTH;	//重置蛇身长度
	Init(); //初始化界面
	InitSnake(Snake,*SnakeLength);//初始化蛇身
	CreateFood(Food,Snake,*SnakeLength);//产生果实
	Dire = 77;//重置方向
	PScore(*SnakeLength);//打印分数
}

//判断函数
int Judge(int Sx,int Sy,int *Food, int (*Snake)[2],int* SnakeLength){
    int i;
    if(Sx==0||Sx==39||Sy==0||Sy==39){    //判断是否撞墙
        GameOver(Snake,SnakeLength,Food);   //结束游戏
        return 0;
    }
    for(i=3;i<*SnakeLength;i++){
        if(Sx == Snake[i][0]&&Sy == Snake[i][1]){   //判断是否撞到自己
            GameOver(Snake,SnakeLength,Food);   //结束游戏
            return 0;
        }
    }
    if(Sx==Food[0]&&Sy==Food[1]){
        CreateFood(Food,Snake,*SnakeLength);//创造食物
		(*SnakeLength)++;//增加长度
		PScore(*SnakeLength);//更新分数
    }
    return 1;
}

//清除x,y处的字符
void Clear(int x,int y){
    Goto(x,y);
    putchar('\0');
}

//根据蛇身长度增加速度
void AddSpeed(int SnakeLength)//根据蛇身长度增加速度
{
if(WaitTime==0)//当WaitTime被改变时，进行还原
    {
    int NewTime;
    NewTime=500-(SnakeLength-SNAKE_LENGTH)*10;//获取新的等待时间
    if(NewTime>=50)//如果新的等待时间>=50
        {
        WaitTime=NewTime;//将新的等待时间赋值给WaitTime
        }
    else//新的等待时间<=50
        {
        WaitTime=50;
        }
    }
}

//移动蛇身函数
void MoveSnake(int Sx,int Sy,int (*Snake)[2],int SnakeLength){
    int j;
    Clear(Snake[SnakeLength-1][0],Snake[SnakeLength-1][1]);     //清除蛇尾
    for(j=SnakeLength;j>=2;j--){     //遍历蛇身，将数组中的蛇身坐标往后移动一个位置，这样子数组第一行就空出来了，用于存储新的头部位置坐标
        Snake[j-1][0] = Snake[j-2][0];
        Snake[j-1][1] = Snake[j-2][1];
    }
    Snake[0][0] = Sx;
    Snake[0][1] = Sy;
    SetColor(10);
    PBox(Snake[0][0],Snake[0][1]);
}

//线程监听键盘输入
DWORD WINAPI ListenKey(LPVOID IpParam){
    for(;;){
        char flag = getch();
        switch(flag){
            case 72:if(Dire!=80){Dire=flag;WaitTime=0;break;}
                    else{break;}
            case 80:if(Dire!=72){Dire=flag;WaitTime=0;break;}
                    else{break;}
            case 75:if(Dire!=77){Dire=flag;WaitTime=0;break;}
                    else{break;}
            case 77:if(Dire!=75){Dire=flag;WaitTime=0;break;}
                    else{break;}
        }
    }
    return 0;
}

//根据方向计算下一步蛇头的位置
void NextStape(int *Sx,int *Sy)
{
	switch (Dire)
	  {
	  case 72: (*Sy)--;break; //上
	  case 80: (*Sy)++;break; //下
	  case 75: (*Sx)--;break; //左
	  case 77: (*Sx)++;break; //右
	  }
}

int main()
{
    int Snake[400][2];  //Snake[0][0]和Snake[0][1]表示蛇头的坐标(x,y)位置
    int SnakeLength = SNAKE_LENGTH;
    int i;
    int Food[2];

    Init();
    InitSnake(Snake,SnakeLength);
    clock_t NowTime;    //声明时间变量
    CreateThread(NULL,0,ListenKey,0,0,0);   //开启线程
    CreateFood(Food,Snake,SnakeLength);//产生果实

    for(int i=0;;i++){
        int Sx = Snake[0][0];   //Sx和Sy 表示的是蛇头的下一个位置
        int Sy = Snake[0][1];
        AddSpeed(SnakeLength);  //根据蛇身体长度改变速度
        NextStape(&Sx,&Sy);     //根据方向，计算下一步蛇头位置
        if(Judge(Sx,Sy,Food,Snake,&SnakeLength)!=0)     //判断下步的情况
        {
            MoveSnake(Sx,Sy,Snake,SnakeLength);     //移动蛇身体
        }
        NowTime = clock();//获取当前时间
        for(;clock()-NowTime<WaitTime;){}   //计时器
    }

    getch();
    return 0;
}
