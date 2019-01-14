#ifndef __SnakeGame_H
#define __SnakeGame_H
#include "sys.h"

void PStar(int x,int y);
void PBox(int x,int y);
void PCircle(int x,int y);
void PInterface(void);	//界面初始化函数
void InitSnake(int (*Snake)[2],int SnakeLength); 	//蛇身初始化
void MoveSnake(int Sx,int Sy,int (*Snake)[2],int SnakeLength);
void NextStape(int *Sx,int *Sy);//根据方向计算下一步蛇头的位置

void rungame(void);

#endif

