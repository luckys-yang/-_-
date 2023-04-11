#ifndef __MY_H
#define __MY_H
#include "main.h"
#include "gpio.h"
#include "i2c_hal.h"
#include "adc.h"
#include "tim.h"
#include "lcd.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

#define KEY_TIME 20
#define ADC_TIME 50


#define KEY1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define KEY2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define KEY4 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

#define BT_State 0
#define C1_State 2
#define C2_State 4


typedef struct
{
    uint16_t KEY_TIMER_COUNT;
    uint16_t ADC_TIMER_COUNT;
} TASK_TIMERTypeDef;

typedef struct
{
    bool KEY1_DOWN_Flag;	//����1
    bool KEY2_DOWN_Flag;	//����2
    bool KEY3_DOWN_Flag;	//����3
    bool KEY4_DOWN_Flag;	//����4
    bool MODE_Flag;	//ģʽ 0���Զ� 1���ֶ�
    uint8_t MENU_Flag;	//ҳ��1:0 ҳ��2:1
    bool SWITCH_Flag;	//�л�Ƶ�ʱ�־λ
} DATA_TypeDef;

typedef struct
{
    uint16_t PA6_duty;	//�ֶ�ģʽ��PA6ռ�ձ�
    uint16_t PA7_duty;	//�ֶ�ģʽ��PA7ռ�ձ�
    uint16_t auto_duty1;	//�Զ�ģʽ�µ�ռ�ձ�1
    uint16_t auto_duty2;	//�Զ�ģʽ�µ�ռ�ձ�2
    float ADC_value;	//ADC����ֵ
} PWM_DUTY_TypeDef;


void PWM_OUT(void);
void Flag_Run_fuction(void);
void LCD_Dis1(void);
void LCD_Dis2(void);
void LCD_function(void);
void Get_ADC(void);
uint8_t KEY_Pro(void);
void KEY_function(void);
void TASK_Init(void);
void LED_DIS(uint8_t num, uint8_t swch);

#endif
