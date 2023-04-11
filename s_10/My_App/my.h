#ifndef __MY_H
#define __MY_H

#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "i2c_hal.h"
#include "lcd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

//��Ӧ����

#define BT_State 2
#define C1_State 3
#define C2_State 4
#define C3_State 5
#define C4_State 6

//������ƽ��ȡ
#define KEY1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define KEY2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define KEY4 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

#define DATA_ADDR 0x08	//��8ҳ��ʼ�洢����

#define KEY_TIME 20	//20msɨ��һ��
#define ADC_TIME 100	//100ms��ȡһ��

//�����ṹ��
typedef struct
{
    uint8_t UP_LED;	//��������ָʾ��(1~8)
    uint8_t DOWN_LED;	//��������ָʾ��(1~8)
    float V_UP;	//��ѹ����(����һλС��)
    float V_DOWN;	//��ѹ����(����һλС��)
    float Now_V;	//��ǰ��ѹ
} PARAMETER_TypeDef;

//ʱ��ɨ��ṹ��
typedef struct
{
    uint16_t KEY_TIME_COUNT;	//����ɨ��
    uint16_t ADC_TIME_COUNT;	//ADCɨ��
} TASK_TypeDef;

//���ݽṹ��
typedef struct
{
    bool KEY1_DOWN_Flag;	//����1���±�־λ
    bool KEY2_DOWN_Flag;	//����2���±�־λ
    bool KEY3_DOWN_Flag;	//����3���±�־λ
    bool KEY4_DOWN_Flag;	//����4���±�־λ
    bool MENU_State;	//ҳ��1,2�л���־λ
    bool One_Flag;	//�Ƿ��һ���ϵ��־λ
    uint8_t High_Line_Flag;	//����ѡ��
    bool UP_LED_Flag;	//����ָʾ�Ʊ�־λ
    bool DOWN_LED_Flag;	//����ָʾ�Ʊ�־λ
    bool LED_SCAN_Flag;	//LED��˸��־λ
} DATA_TypeDef;



void LED_DIS(uint8_t num, uint8_t swch);	//LED��ʾ
void TASK_Init(void);	//��ʼ��
void LCD_function(void);	//LCD���ˢ��
void KEY_function(void);	//�������
uint8_t KEY_Pro(void);	//������ȡ
void EEPROM_Page_Write(uint8_t addr, uint8_t *date, uint8_t len);	//EEPROMҳд
void EEPROM_Page_Read(uint8_t addr, uint8_t *date, uint8_t len);	//EEPROMҳ��
void Parameter_Read(PARAMETER_TypeDef *para);	//��ȡEEPROM����
void Parameter_Write(PARAMETER_TypeDef *para);	//д��EEPROM����
void LCD_Dis1(void);	//��ҳ��
void LCD_Dis2(void);	//����ҳ��
void Run_function(void);	//��־λִ�й��ܺ���
void GetAdc(void);	//��ȡADC
void Add_V(PARAMETER_TypeDef *para, uint8_t dir);	//���ӵ�ѹ
void Add_LED(PARAMETER_TypeDef *para, uint8_t dir);	//����LED
void Arr_KG(uint8_t arr[][20], uint8_t NUM);	//��ӿո�
void Reduce_LED(PARAMETER_TypeDef *para, uint8_t dir);	//��С��ѹ
void Reduce_V(PARAMETER_TypeDef *para, uint8_t dir);	//��СLED
void LED_Togg(uint8_t num);	//LED��ת
void Judge_function(void);	//�ж��Ƿ񳬳����ƺ���
void LED_Flicker(void);	//LED��˸ִ��

#endif
