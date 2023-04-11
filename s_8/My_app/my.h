#ifndef __MY_H
#define __MY_H
#include "main.h"
#include "gpio.h"
#include "rtc.h"
#include "tim.h"
#include "my.h"
#include "lcd.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
/***************************�궨��*****************************/

//LCD Display
#define LCD_SPACE0	0
#define LCD_SPACE1	1
#define LCD_TEST	2
#define LCD_SPACE3	3
#define LCD_SPACE4	4
#define LCD_NUMBER	5
#define LCD_SPACE6	6
#define LCD_SPACE7	7
#define LCD_RTC	8
#define LCD_SPACE9	9

//ʱ��Ƭ
#define KEY_TIMEST	10	//10msɨ��һ��
#define RTC_TIMEST  200	//200msɨ��һ��

//������ȡ��ƽ״̬
#define KEY1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)	//PB0
#define KEY2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)	//PB1
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)	//PB2
#define KEY4 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)	//PA0

//�����ʼӶ��
#define KEY_OVER_TIME	1000	//����������Ҫ�ȴ���ʱ��
#define SJJ_OC_OVER_TIME	4000	//��������������Ҫ��ʱ��
#define SJJ_UD_OVER_TIME	6000	//������������Ҫ��ʱ��
#define SJJ_WAIT_OVER_TIME	2000	//������ͣ����Ҫ��ʱ��



/***************************����*****************************/

//ʱ��Ƭ�ṹ��
typedef struct
{
    uint8_t KEY_time;
    uint8_t RTC_TIME;
} TASK_TIMESTypeDef;

//ϵͳ����
typedef struct
{
    uint8_t Platform;	//��ǰƽ̨(1~4)
    bool LED1_State;	//LED1״̬(����4��ƽ̨)
    bool LED2_State;	//LED2״̬
    bool LED3_State;	//LED3״̬
    bool LED4_State;	//LED4״̬

    bool KEY_Timer_Start;	//�������� ��ʼ��ʱ��־λ
    uint16_t KEY_Timer_Count;	//�������º��ʱʱ��
    bool KEY_Timer_Over;	//�������¼�ʱ ��ɱ�־λ
    bool KEY_InFore_Flag;	//�����Ƿ���Ч(��������ڼ���Ч 1:��Ч 0:��Ч)

    bool SJJ_Close_Start;	//��������ʼ���� ��־λ
    bool SJJ_Close_Over;	//���������Ž��� ��־λ
    bool SJJ_Open_Start;	//��������ʼ���� ��־λ
    bool SJJ_Open_Over;	//���������Ž��� ��־λ
    uint16_t SJJ_OpenClose_Timer_Count;	//����������/���ż�ʱʱ��

    uint16_t SJJ_Wait_Timer_Count;	//������ͣ����ʱʱ��
    bool SJJ_Wait_Start;	//��ʼͣ��
    bool SJJ_Wait_Over;	//ͣ������

    bool SJJ_UP_Start;	//��������ʼ���� ��־λ
    bool SJJ_UP_Over;	//������������� ��־λ
    bool SJJ_DOWN_Start;	//��������ʼ�½� ��־λ
    bool SJJ_DOWN_Over;	//�������½���� ��־λ
    uint16_t SJJ_UpDown_Timer_Count;	//���������¼�ʱʱ��

    bool Platform_Over;	//����Ŀ��ƽ̨��־λ
    bool Platform_NumberBlink_Flag;	//������˸
    bool Platform_UPorDOWN;	//ѡ�� 1:��-->��/0:��-->��ɨ�� ��־λ
    uint8_t PWM_Out_UpDown_Flag;	//���µ��pwm����л���־λ
    uint8_t PWM_Out_OpenClose_Flag;	//���ص��pwm����л���־λ
} DATA_TypeDef;


extern DATA_TypeDef Data;

extern TASK_TIMESTypeDef TASK_TIMES;

extern uint8_t LCD_Line_BUFF[10][20];	//LCD��ʾ����



/***************************����*****************************/

void LCD_function(void);
void LCD_DrawMonoPict1(uint16_t Xpos, uint16_t Ypos, uint32_t BackColor, uint32_t TextColor, uint8_t *Pict);
void TASK_Init(void);
void RTC_function(void);
void PWM_SET_PA6(uint32_t freq, uint16_t duty);
void PWM_SET_PA7(uint32_t freq, uint16_t duty);
void KEY_SCAN(void);
uint8_t KEY_pro(void);
void LED_DIS(uint8_t num, uint8_t swch);
void Flag_Run(void);
void SJJ_UP(void);
uint8_t Check(void);
void SJJ_DOWN(void);
uint8_t Check_DOWN(void);
uint8_t Check_UP(void);
#endif
