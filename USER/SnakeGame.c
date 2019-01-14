#include <stdio.h>
#include <stdlib.h>
#include "SnakeGame.h"
#include "lcd.h"
#include "rtc.h" 
#include "sys.h"
#include "delay.h"

const int SNAKE_LENGTH = 3;
u32 WaitTime = 32767;
int flag = 1;


//在指定位置打印符号
void PStar(int x,int y)
{
    LCD_ShowChar(x*12,y*12,'*',12,0);
}
void PBox(int x,int y)
{
    LCD_ShowChar(x*12,y*12,'#',12,0);
}
void PCircle(int x,int y){
    LCD_ShowChar(x*12,y*12,'@',12,0);
}

//界面初始化函数
void PInterface()
{
    int i=0;
    for(;i<40;i++){
        if(i<=29){
            PStar(0,i);//打印竖线
            PStar(29,i);
            PStar(39,i);
            PStar(i,0);//打印横线
            PStar(i,29);
        }else{
		PStar(i,0);//打印横线
		PStar(i,29);
		}
    }
	LCD_ShowString(10,360,51*24,24,24,"This is a snakegame! Have a good time!");
	LCD_ShowString(380,100,6*24,24,24,"score:");
}


//初始化蛇身
void InitSnake(int (*Snake)[2],int SnakeLength){
    int i =0;
    for(;i<SnakeLength;i++){
        Snake[i][0] = 12-i;
        Snake[i][1] = 4;
        PBox(Snake[i][0],Snake[i][1]);
    }
}

//产生食物
void CreateFood(int *Food,int Snake[400][2],int SnakeLength){
    int Cfood[2];
    srand(RTC_GetCounter());
    for(;;){
        int foodflag =1;
        int i;
        Cfood[0] = rand()%28+1; //生成位置
        Cfood[1] = rand()%28+1;
        for(i=0;i<SnakeLength;i++){
            if(Cfood[0]==Snake[i][0]&&Cfood[1]==Snake[i][1]){       //判断随机生成的果实的位置是否与蛇身的位置重叠，如果重叠则重新生成位置
                foodflag =0;
                break;
            }
        }
        if(foodflag==1)//产生了符合要求的食物坐标
        {
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
	LCD_ShowString(380,100,6*24,24,24,"score:");
	LCD_ShowNum(380,130,score,4,24);
}

//游戏结束函数
void GameOver(int (*Snake)[2],int *SnakeLength,int *Food){

	LCD_ShowString(25,180,51*24,24,24,"gameover! It will start 5 sec later!");
	delay_ms(5000);

	LCD_Clear(WHITE);	//清空屏幕
	*SnakeLength = SNAKE_LENGTH;	//重置蛇身长度
	PInterface(); //初始化界面
	InitSnake(Snake,*SnakeLength);//初始化蛇身
	CreateFood(Food,Snake,*SnakeLength);//产生果实
	flag = 1;//重置方向
	PScore(*SnakeLength);//打印分数
}

//判断函数
int Judge(int Sx,int Sy,int *Food, int (*Snake)[2],int* SnakeLength){
    int i;
    if(Sx==0||Sx==29||Sy==0||Sy==29){    //判断是否撞墙
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
    LCD_ShowChar(x*12,y*12,' ',12,0);
}

//根据蛇身长度增加速度
void AddSpeed(int SnakeLength)//根据蛇身长度增加速度
{
if(WaitTime==0)//当WaitTime被改变时，进行还原
    {
    u32 NewTime;
    NewTime = 32767-(SnakeLength-SNAKE_LENGTH)*1000;//获取新的等待时间
    if(NewTime>=6553)//如果新的等待时间>=0.2s
        {
        WaitTime=NewTime;//将新的等待时间赋值给WaitTime
        }
    else//新的等待时间
        {
        WaitTime =6553;
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
    PBox(Snake[0][0],Snake[0][1]);
}

//根据方向计算下一步蛇头的位置
void NextStape(int *Sx,int *Sy)
{
	switch (flag)
	  {
		  case 1: (*Sx)++;break; //右
		  case 2: (*Sy)--;break; //上
		  case 3: (*Sx)--;break; //左
		  case 4: (*Sy)++;break; //下
	  }
}

//游戏运行
void rungame(void)
{
	int Snake[40][2];  //Snake[0][0]和Snake[0][1]表示蛇头的坐标(x,y)位置
    int SnakeLength = SNAKE_LENGTH;
	u32 NowTime;    //声明时间变量
	
	int Food[2];
	
	PInterface();
    InitSnake(Snake,SnakeLength);

    CreateFood(Food,Snake,SnakeLength);//产生果实

    while(1){
        int Sx = Snake[0][0];   //Sx和Sy 表示的是蛇头的下一个位置
		int Sy = Snake[0][1];
        AddSpeed(SnakeLength);  //根据蛇身体长度改变速度
        NextStape(&Sx,&Sy);     //根据方向，计算下一步蛇头位置
        if(Judge(Sx,Sy,Food,Snake,&SnakeLength)!=0)     //判断下步的情况
        {
            MoveSnake(Sx,Sy,Snake,SnakeLength);     //移动蛇身体
        }
        NowTime = RTC_GetDivider();//获取当前时间,具体操作为读RTC的DIV寄存器以获得比秒更精确的时间间隔
        for(;NowTime-RTC_GetDivider()<WaitTime;){}   //计时器
    }
}








