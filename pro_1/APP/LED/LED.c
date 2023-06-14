#include "allhead.h"

#define DELAY_TIME 1000

//��ˮ��
void LED_TEST(void)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);	//��ת��ƽ
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);	//����
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);	//����
    HAL_Delay(DELAY_TIME);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_Delay(DELAY_TIME);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_Delay(DELAY_TIME);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_Delay(DELAY_TIME);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_Delay(DELAY_TIME);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_Delay(DELAY_TIME);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_Delay(DELAY_TIME);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_Delay(DELAY_TIME);
}

//ö��
enum {
	START_OPEN_1_3_5_7,	//��1,3,5LED
	START_OPEN_2_4_6_8,	//��2,4,6LED
	START_OPEN_ALL,	//�����е�
};

//����һ��ʼ״̬(�������ϵ�������ļ���)
uint8_t startNow = START_OPEN_ALL;

void LED_Dis1(void)
{
	switch(startNow)
	{
		case START_OPEN_ALL:	//�����ǰ״̬�����е�����
			startNow = START_OPEN_1_3_5_7;	//״̬�ĳ�1,3,5,7��
			lightAll_open(0);	//�ر����е�
			light1357_open(1);	//����1357
		break;
		case START_OPEN_1_3_5_7:
			startNow = START_OPEN_2_4_6_8;
			light1357_open(0);
			light2468_open(1);
		break;
		case START_OPEN_2_4_6_8:
			startNow = START_OPEN_ALL;
			lightAll_open(1);
		break;
	}
}
//����/�ر�1357
void light1357_open(uint8_t mode)
{
	if(1 == mode)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14,GPIO_PIN_RESET);	//����1,3,5,7
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14,GPIO_PIN_SET);	//�ر�1,3,5,7
	}
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);	//����
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);	//����
}
//����/�ر�2468
void light2468_open(uint8_t mode)
{
	if(1 == mode)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_15,GPIO_PIN_RESET);	//����2,4,6,8
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_15,GPIO_PIN_SET);	//�ر�2,4,6,8
	}
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);	//����
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);	//����
}
//����/�ر�����
void lightAll_open(uint8_t mode)
{
	if(1 == mode)
	{
		HAL_GPIO_WritePin(GPIOC,0xFF00,GPIO_PIN_RESET);	//��������
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC,0xFF00,GPIO_PIN_SET);	//�ر�����
	}
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);	//����
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);	//����
}

//��������������1��ʾ�򿪷����� 0��ʾ�رա�
void Buzzer_test(uint8_t swch)
{
	if(SET == swch)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);	//�򿪷�����
	}
	else if(RESET == swch)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);	//�رշ�����		
	}
}
