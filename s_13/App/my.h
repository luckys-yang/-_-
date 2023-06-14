#ifndef __MY_H
#define __MY_H

#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "lcd.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"


#define KEY1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define KEY2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define KEY4 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

#define BT_STATE 1
#define C1_STATE 3
#define C2_STATE 4
#define C3_STATE 5



typedef struct 
{
	bool KEY1_DOWN_Flag;	//1
	bool KEY2_DOWN_Flag;	//2
	bool KEY3_DOWN_Flag;	//3
	bool KEY4_DOWN_Flag;	//4
	uint8_t mm_1;	//�����һλ
	uint8_t mm_2;	//�����һλ
	uint8_t mm_3;	//�����һλ
	uint16_t freq;	//Ƶ��
	float duty;	//ռ�ձ�
	bool MENU_Flag;	//Ĭ��0������ҳ�� 1�����ҳ��
	bool Run_Count_5s;	//5�뿪ʼ������־λ
	uint16_t Run_Timer_Count;	//����ֵ
	bool LED2_Count_5s;	//LED2��ʼ������־λ
	uint16_t LED2_Timer_Count;	//����ֵ
	
	
}DATA_TypeDef;

void LCD_Dis2(void);
void LCD_Dis1(void);
void TASK_Init(void);
void LED_DIS(uint8_t num,uint8_t swch);
void KEY_function(void);
uint8_t KEY_Proc(void);
void LCD_function(void);
void Flag_Run_function(void);
void USART1_function(void);
#endif