#include "allhead.h"

//�ⲿ�жϺ�����stm32g4xx_hal_gpio.c����������������
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0)	//�����PB0����
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == 0)
		{
			light2468_open(1);
		}
	}
	if(GPIO_Pin == GPIO_PIN_1)	//�����PB1����
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == 0)
		{
			light2468_open(0);
		}		
	}
	if(GPIO_Pin == GPIO_PIN_2)	//�����PB2����
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == 0)
		{
			lightAll_open(1);
		}		
	}
	
}
