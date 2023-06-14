#ifndef __MY_H
#define __MY_H
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "lcd.h"
#include "i2c_hal.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"


#define KEY1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define KEY2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define KEY4 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

#define ADDR 0x00

#define C1_STATE 2
#define C2_STATE 4
#define C3_STATE 6
#define C4_STATE 8
#define C5_STATE 9

#define KEY_TIME 10	//����ɨ��ʱ��
#define PWMIC_TIME 100	//PWM����ʱ��

typedef struct
{
    uint32_t CH1_pluse_val;	//ͨ��1��Ƶ��
    uint32_t CH1_duty;	//ͨ��1��Ч��ƽֵ
    uint32_t CH2_pluse_val;	//ͨ��2��Ƶ��
    uint32_t CH2_duty;	//ͨ��2��Ч��ƽֵ
} PWM_OCTypeDef;

typedef struct
{
    bool KEY1_DOWN_Flag;	//����1
    bool KEY2_DOWN_Flag;	//����2
    bool KEY3_DOWN_Flag;	//����3
    bool KEY4_DOWN_Flag;	//����4
    bool UARTorLOCAL_Flag;	//���ڻ��Ǳ����л���־λ
    bool CH_SWITCH_Flag;	//ͨ���л�
} DATA_TypeDef;

typedef struct
{
    uint32_t CH2_IC_freq;	//���벶��ͨ��2��Ƶ��
    uint32_t CH3_IC_freq;	//���벶��ͨ��3��Ƶ��
    uint8_t Mult_CH2;	//ͨ��2��Ƶ��(Ĭ��2)
    uint8_t Mult_CH3;	//ͨ��3��Ƶ��
} PWM_ICTypeDef;

typedef struct
{
    uint32_t CH_ONE;	//��¼4�ε�ֵ
    uint32_t CH_TWO;
    uint32_t CH_THREE;
    uint32_t CH_FOUR;
    uint8_t RUN_FLAG;	//���б�־λ
} PWM_PARAMETER_TypeDef;

typedef struct
{
    uint32_t TASK_KEY;
    uint32_t TASK_TIMIC;
} TASK_TIMESTypeDef;

void TASK_init(void);
void LCD_Dis(void);
void LCD_function(void);
void LED_Write_all(uint8_t num);
uint32_t get_time_task(uint32_t time);
uint32_t get_time(void);
void TASK_TIME_init(void);
void KEY_function(void);
uint8_t KEY_Proc(void);
void EEPROM_Read(uint8_t addr, uint8_t *date, uint8_t len);
void EEPROM_Write(uint8_t addr, uint8_t *date, uint8_t len);
void PWM_Read(void);
void PWM_Wirte(void);
void USART1_function(void);
void FlagRun_function(void);
void LED_Dis(uint8_t num, uint8_t swch);
void TIM_IN_function(void);
void Add_Space(uint8_t date[][20], uint8_t index);
void PWM_SET_PA7(uint32_t freq, uint16_t duty);
void PWM_SET_PA6(uint32_t freq, uint16_t duty);
void GPIO_OUT(uint16_t pin,GPIO_PinState PinState);
void PWM_OC(uint16_t pin);

#endif