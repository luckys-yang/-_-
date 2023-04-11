#ifndef __MY_H
#define __MY_H
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "i2c_hal.h"
#include "lcd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "rtc.h"

#define KEY1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define KEY2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define KEY4 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

#define ADDR 0x00	//�洢��ַ

#define KEY_TIME 10

#define C1_STATE 0
#define C2_STATE 2
#define C3_STATE 4
#define C4_STATE 6
#define C5_STATE 8
#define C6_STATE 9

typedef struct
{
    uint32_t TASK_KEY;
} TASK_TIMESTypeDef;

typedef struct
{
    bool KEY1_DOWN_FLAG;	//����1
    bool KEY2_DOWN_FLAG;	//����2
    bool KEY3_DOWN_FLAG;	//����3
    bool KEY4_DOWN_FLAG;	//����4
    bool DATA_OVER_FLAG;	//���մ������ݴ�����ɱ�־λ
    bool Alarm_OVER_FLAG;	//���ӵ�ʱ��־λ
    bool RUN_START_FLAG;	//���忪ʼ���������־λ
    bool RUN_START_OVER_FLAG;	//�������������ɱ�־λ
} DATA_TypeDef;

typedef struct
{
    uint16_t PA1_duty;
    uint16_t PA2_duty;
    uint8_t RX_hour;	//ʱ
    uint8_t RX_min;	//��
    uint8_t RX_sec;	//��
    uint8_t CHxNumber;	//�ĸ��ܽ�
    uint8_t RunTime_Count;	//����ʱ��
} PARAMETER_TypeDef;

void TASK_init(void);
void LCD_Dis1(void);
void Add_Space(uint8_t date[][20], uint8_t State);
void LCD_function(void);
void LED_Dis(uint8_t num, uint8_t swch);
void TASK_TIME_init(void);
uint32_t get_task_time(uint32_t time);
uint32_t get_time(void);
void RtcGetTime_function(void);
void FlagRun_function(void);
void KEY_function(void);
uint8_t KEY_Proc(void);
void PWM_Out(uint16_t Pin);
void GPIO_Out(uint16_t Pin, GPIO_PinState PinState);
void EEPROM_Write(uint8_t addr, uint8_t *date, uint8_t len);
void EEPROM_Read(uint8_t addr, uint8_t *date, uint8_t len);
void Parameter_Read(void);
void Parameter_Write(void);
void USART1_function(void);
int8_t Hour_DealWith(uint8_t *date);
int8_t Min_DealWith(uint8_t *date);
int8_t Sec_DealWith(uint8_t *date);
void Alarm_SetTime(uint16_t hour, uint16_t min, uint16_t sec);

#endif