#ifndef __MY_H
#define __MY_H
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "lcd.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define KEY1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define KEY2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define KEY4 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)
#define TEST 0	//��ӡ����

typedef struct
{
    uint8_t HightLight_Flag;	//������־λĬ��R
    float Adc_Value;	//ADCֵ
    bool LCD_OVER_FLAG;	//LCDˢ�±�־λ
    uint8_t Key_Down_State[8];	//����״̬
    bool Adc_Over_Flag;	//ADC������ɱ�־λ
    uint8_t H_or_L_State;	//�ߵ�Ƶ״̬
    bool Fre_5s_Flag;	//Ƶ��5s��־λ
    uint8_t Lock_State;	//�Ƿ�����־λ
    uint8_t Lcd_Dis_Choose_State;	//������ʾ״̬
} MYDATA_TypeDef;

typedef struct
{
    uint16_t PA1_Fre;	//PA1���Ƶ��
    uint8_t Data_P;	//ʵʱռ�ձ�
    float Data_V;	//ʵʱ�ٶ�
    uint8_t Para_R;	//����R
    uint8_t Para_K;	//����K
    uint16_t Recd_N;	//���ģʽ�л�����
    float Recd_MH;	//���ģʽ�¸�Ƶģʽ�ٶ����ֵ
    float Recd_ML;	//���ģʽ�µ�Ƶģʽ�ٶ���Сֵ

} PARAMETER_TypeDef;

typedef struct
{
    bool IC_OVER_FLAG;	//�����ٶ���ɱ�־λ
    uint32_t One_Value;	//��1��
    uint32_t Two_Value;	//��2��
    uint32_t Three_Value;	//��3��
    uint32_t Four_Value;	//��4��
    uint16_t Fre_Value;	//Ƶ��
    float Duty_Value;	//ռ�ձ�
    uint8_t IC_RUN_STATE;	//����״̬
} PWMDATA_TypeDef;


extern MYDATA_TypeDef MyData;
extern PARAMETER_TypeDef ParameterData;
extern PWMDATA_TypeDef PwmData;

void vLed_Control2(uint8_t swch, uint8_t num);
void vAdc_Get_function(void);
void vRun_Flag_function(void);
uint8_t ucKey_Scan(void);
void vKey_function(void);
void vHardware_Init(void);
void vLed_Control(uint8_t led_num);
void vLcd_Display_function(void);
#endif