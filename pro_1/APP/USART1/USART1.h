#ifndef __USART1_H
#define __USART1_H
#include "main.h"

//�������������ֽ�
#define USART1_RX_LEN 100

extern char USART1_RX_BUF[USART1_RX_LEN];	//��Ž������ݵ���������
extern uint8_t RX_flag;	//���ձ�־λ
extern char USART1_NewData;	//���½��յ��ֽ�

//��Ž������ݽṹ��
typedef struct USART1_Receive{

	char c1[3];
	int x;
	int y;

}RX_Data;
extern RX_Data d1;

void PrintTest(void);
void TXTest(void);

#endif






