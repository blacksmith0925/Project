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


//��ָ��λ�ô�ӡ����
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

//�����ʼ������
void PInterface()
{
    int i=0;
    for(;i<40;i++){
        if(i<=29){
            PStar(0,i);//��ӡ����
            PStar(29,i);
            PStar(39,i);
            PStar(i,0);//��ӡ����
            PStar(i,29);
        }else{
		PStar(i,0);//��ӡ����
		PStar(i,29);
		}
    }
	LCD_ShowString(10,360,51*24,24,24,"This is a snakegame! Have a good time!");
	LCD_ShowString(380,100,6*24,24,24,"score:");
}


//��ʼ������
void InitSnake(int (*Snake)[2],int SnakeLength){
    int i =0;
    for(;i<SnakeLength;i++){
        Snake[i][0] = 12-i;
        Snake[i][1] = 4;
        PBox(Snake[i][0],Snake[i][1]);
    }
}

//����ʳ��
void CreateFood(int *Food,int Snake[400][2],int SnakeLength){
    int Cfood[2];
    srand(RTC_GetCounter());
    for(;;){
        int foodflag =1;
        int i;
        Cfood[0] = rand()%28+1; //����λ��
        Cfood[1] = rand()%28+1;
        for(i=0;i<SnakeLength;i++){
            if(Cfood[0]==Snake[i][0]&&Cfood[1]==Snake[i][1]){       //�ж�������ɵĹ�ʵ��λ���Ƿ��������λ���ص�������ص�����������λ��
                foodflag =0;
                break;
            }
        }
        if(foodflag==1)//�����˷���Ҫ���ʳ������
        {
			PCircle(Cfood[0],Cfood[1]);//��ӡʳ��
			Food[0] = Cfood[0];//Ϊʳ�︳ֵ
			Food[1] = Cfood[1];
			break;
        }
    }
}

//��ӡ����
void PScore(int SnakeLength){
    int score = (SnakeLength-SNAKE_LENGTH)*10;//��ǰ�����ȼ�ȥ��ʼֵ��*10
	LCD_ShowString(380,100,6*24,24,24,"score:");
	LCD_ShowNum(380,130,score,4,24);
}

//��Ϸ��������
void GameOver(int (*Snake)[2],int *SnakeLength,int *Food){

	LCD_ShowString(25,180,51*24,24,24,"gameover! It will start 5 sec later!");
	delay_ms(5000);

	LCD_Clear(WHITE);	//�����Ļ
	*SnakeLength = SNAKE_LENGTH;	//����������
	PInterface(); //��ʼ������
	InitSnake(Snake,*SnakeLength);//��ʼ������
	CreateFood(Food,Snake,*SnakeLength);//������ʵ
	flag = 1;//���÷���
	PScore(*SnakeLength);//��ӡ����
}

//�жϺ���
int Judge(int Sx,int Sy,int *Food, int (*Snake)[2],int* SnakeLength){
    int i;
    if(Sx==0||Sx==29||Sy==0||Sy==29){    //�ж��Ƿ�ײǽ
        GameOver(Snake,SnakeLength,Food);   //������Ϸ
        return 0;
    }
    for(i=3;i<*SnakeLength;i++){
        if(Sx == Snake[i][0]&&Sy == Snake[i][1]){   //�ж��Ƿ�ײ���Լ�
            GameOver(Snake,SnakeLength,Food);   //������Ϸ
            return 0;
        }
    }
    if(Sx==Food[0]&&Sy==Food[1]){
        CreateFood(Food,Snake,*SnakeLength);//����ʳ��
		(*SnakeLength)++;//���ӳ���
		PScore(*SnakeLength);//���·���
    }
    return 1;
}

//���x,y�����ַ�
void Clear(int x,int y){
    LCD_ShowChar(x*12,y*12,' ',12,0);
}

//���������������ٶ�
void AddSpeed(int SnakeLength)//���������������ٶ�
{
if(WaitTime==0)//��WaitTime���ı�ʱ�����л�ԭ
    {
    u32 NewTime;
    NewTime = 32767-(SnakeLength-SNAKE_LENGTH)*1000;//��ȡ�µĵȴ�ʱ��
    if(NewTime>=6553)//����µĵȴ�ʱ��>=0.2s
        {
        WaitTime=NewTime;//���µĵȴ�ʱ�丳ֵ��WaitTime
        }
    else//�µĵȴ�ʱ��
        {
        WaitTime =6553;
        }
    }
}

//�ƶ�������
void MoveSnake(int Sx,int Sy,int (*Snake)[2],int SnakeLength){
    int j;
    Clear(Snake[SnakeLength-1][0],Snake[SnakeLength-1][1]);     //�����β
    for(j=SnakeLength;j>=2;j--){     //���������������е��������������ƶ�һ��λ�ã������������һ�оͿճ����ˣ����ڴ洢�µ�ͷ��λ������
        Snake[j-1][0] = Snake[j-2][0];
        Snake[j-1][1] = Snake[j-2][1];
    }
    Snake[0][0] = Sx;
    Snake[0][1] = Sy;
    PBox(Snake[0][0],Snake[0][1]);
}

//���ݷ��������һ����ͷ��λ��
void NextStape(int *Sx,int *Sy)
{
	switch (flag)
	  {
		  case 1: (*Sx)++;break; //��
		  case 2: (*Sy)--;break; //��
		  case 3: (*Sx)--;break; //��
		  case 4: (*Sy)++;break; //��
	  }
}

//��Ϸ����
void rungame(void)
{
	int Snake[40][2];  //Snake[0][0]��Snake[0][1]��ʾ��ͷ������(x,y)λ��
    int SnakeLength = SNAKE_LENGTH;
	u32 NowTime;    //����ʱ�����
	
	int Food[2];
	
	PInterface();
    InitSnake(Snake,SnakeLength);

    CreateFood(Food,Snake,SnakeLength);//������ʵ

    while(1){
        int Sx = Snake[0][0];   //Sx��Sy ��ʾ������ͷ����һ��λ��
		int Sy = Snake[0][1];
        AddSpeed(SnakeLength);  //���������峤�ȸı��ٶ�
        NextStape(&Sx,&Sy);     //���ݷ��򣬼�����һ����ͷλ��
        if(Judge(Sx,Sy,Food,Snake,&SnakeLength)!=0)     //�ж��²������
        {
            MoveSnake(Sx,Sy,Snake,SnakeLength);     //�ƶ�������
        }
        NowTime = RTC_GetDivider();//��ȡ��ǰʱ��,�������Ϊ��RTC��DIV�Ĵ����Ի�ñ������ȷ��ʱ����
        for(;NowTime-RTC_GetDivider()<WaitTime;){}   //��ʱ��
    }
}








