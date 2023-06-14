#ifndef __MY_H
#define __MY_H
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "lcd.h"
#include "i2c_hal.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"


#define KEY1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define KEY2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define KEY4 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

#define BT_STATE 1
#define C1_STATE 3
#define C2_STATE 5
#define C3_STATE 7
#define D1_STATE 4
#define D2_STATE 6
#define D3_STATE 8

#define ADDR 0x00


typedef struct
{
    bool KEY1_DOWN_Flag;	//1
    bool KEY2_DOWN_Flag;	//2
    bool KEY3_DOWN_Flag;	//3
    bool KEY4_DOWN_Flag;	//4
    bool MENU_Flag;	//ҳ���־λ
    bool ADC_Over_Flag;	//ADC�ɼ���ɱ�־λ
    uint8_t HightLight_State;	//��������
    bool Grade_Check_Flag;	//Һλ�ȼ������ı��־λ
    bool Inquire_Flag;	//���ڲ�ѯ��־λ
} DATA_TypeDef;

typedef struct
{
    uint8_t Height;	//Һλ�߶�
    float ADC;	//adcֵ
    uint8_t Level;	//�ȼ�
    uint8_t Threshold_1;	//��ֵ1
    uint8_t Threshold_2;	//��ֵ2
    uint8_t Threshold_3;	//��ֵ3
} RARAMETER_TypeDef;

void KEY_function(void);
uint8_t KEY_Proc(void);
void LCD_function(void);
void LCD_Dis1(void);
void LCD_Dis2(void);
void LED_DIS(uint8_t num, uint8_t swch);
void TASK_Init(void);
void ADC_Filtering(void);
void Flag_Run_function(void);
void EEPROM_Read(uint8_t addr, uint8_t *date, uint8_t len);
void EEPROM_Write(uint8_t addr, uint8_t *date, uint8_t len);
void Parameter_Read(void);
void Parameter_Write(void);
void Judge_Grade(void);
void USART1_function(void);
void LED_Togg(uint8_t num);
#endif