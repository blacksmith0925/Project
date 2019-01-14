#ifndef __SnakeGame_H
#define __SnakeGame_H
#include "sys.h"

void PStar(int x,int y);
void PBox(int x,int y);
void PCircle(int x,int y);
void PInterface(void);	//�����ʼ������
void InitSnake(int (*Snake)[2],int SnakeLength); 	//�����ʼ��
void MoveSnake(int Sx,int Sy,int (*Snake)[2],int SnakeLength);
void NextStape(int *Sx,int *Sy);//���ݷ��������һ����ͷ��λ��

void rungame(void);

#endif

