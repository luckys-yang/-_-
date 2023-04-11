#include "my.h"

uint8_t Password[3] = {1,2,3};	//Ĭ������
uint8_t Password_Error_Count = 0;	//����������
uint8_t num1 = 0,num2 = 0,num3 = 0;	//��������Ĭ������
uint8_t One_Open_Flag = 0;	//��һ���ϵ� 
DATA_TypeDef Data = {0};
uint16_t LCD_State = 0xFFFF;
uint8_t LCD_Line_BUFF[10][20] = {
	"                   ",
	"                   ",	//ҳ��1 psc ��1��
	"                   ",
	"                   ",	//ҳ��1 C1��3��
	"                   ",	//ҳ��1 C2 ҳ��2��4��	
	"                   ",	//ҳ��1 C3 ҳ��2��5��
	"                   ",
	"                   ",
	"                   ",
	"                   "
};


void LED_DIS(uint8_t num,uint8_t swch)
{
	if(SET == swch)
	{
		HAL_GPIO_WritePin(GPIOC,(uint16_t)num<<8,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC,(uint16_t)num<<8,GPIO_PIN_SET);
	}
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void LCD_function(void)
{
	for(uint8_t i = 0;i<10;i++)
	{
		if(LCD_State & (1<<i))
		{
			LCD_State &= (~(0x01<<i));
			LCD_DisplayStringLine(i*24,LCD_Line_BUFF[i]);
		}
	}
}

void LCD_Dis1(void)
{
	Data.mm_1 = '@';
	Data.mm_2 = '@';
	Data.mm_3 = '@';
	num1 = 0;	//��0
	num2 = 0;
	num3 = 0;
	sprintf((char*)LCD_Line_BUFF[BT_STATE],"       PSD          ");
	sprintf((char*)LCD_Line_BUFF[C1_STATE],"    B1:%c            ",Data.mm_1);
	sprintf((char*)LCD_Line_BUFF[C2_STATE],"    B2:%c            ",Data.mm_2);
	sprintf((char*)LCD_Line_BUFF[C3_STATE],"    B3:%c            ",Data.mm_3);
	
	LCD_State |= 0xFF;
}

void LCD_Dis2(void)
{
	sprintf((char*)LCD_Line_BUFF[BT_STATE],"       STA          ");
	sprintf((char*)LCD_Line_BUFF[C1_STATE],"    F:%dHz        ",Data.freq);
	sprintf((char*)LCD_Line_BUFF[C2_STATE],"    D:%.0f%%           ",Data.duty);
	sprintf((char*)LCD_Line_BUFF[C3_STATE],"                    ");
	
	LCD_State |= 0xFF;
}


uint8_t KEY_Proc(void)
{
	if((!KEY1) ||(!KEY2) ||(!KEY3) ||(!KEY4))
	{
		if(!KEY1)
			return 1;
		if(!KEY2)
			return 2;
		if(!KEY3)
			return 3;
		if(!KEY4)
			return 4;
	}
	return 0;
}

uint8_t KEY_UP,KEY_DOWN,KEY_VALUE;
uint16_t KEY_COUNT;

void KEY_function(void)
{
	static uint8_t Old_Value;
	KEY_VALUE = KEY_Proc();
	KEY_UP = ~KEY_VALUE & (KEY_VALUE^Old_Value);
	KEY_DOWN = KEY_VALUE & (KEY_VALUE^Old_Value);
	Old_Value = KEY_VALUE;
	
	if(KEY_DOWN)
	{
		KEY_COUNT = 0;
	}
	if(KEY_COUNT < 10)
	{
		switch(KEY_UP)
		{
			case 1:
			{
				if(Data.MENU_Flag || Data.LED2_Count_5s)	//���ҳ��ʧЧ
				{
					break;
				}
				Data.KEY1_DOWN_Flag = 1;
				break;
			}
			case 2:
			{
				if(Data.MENU_Flag || Data.LED2_Count_5s)	//���ҳ��ʧЧ
				{
					break;
				}
				Data.KEY2_DOWN_Flag = 1;
				break;
			}
			case 3:
			{
				if(Data.MENU_Flag || Data.LED2_Count_5s)	//���ҳ��ʧЧ
				{
					break;
				}				
				Data.KEY3_DOWN_Flag = 1;
				break;
			}
			case 4:
			{
				if(Data.MENU_Flag || Data.LED2_Count_5s)	//���ҳ��ʧЧ
				{
					break;
				}				
				Data.KEY4_DOWN_Flag = 1;
				break;
			}
			default:break;			
		}
	}
}


void Flag_Run_function(void)
{
	if(Data.KEY1_DOWN_Flag)
	{
		Data.KEY1_DOWN_Flag = 0;
		Data.mm_1 = num1;
		sprintf((char*)LCD_Line_BUFF[C1_STATE],"    B1:%c            ",Data.mm_1 + '0');	//����ת�ַ�
		LCD_State |= (0x01<<C1_STATE);
		num1++;
		if(num1>9)
		{
			num1 = 0;
		}
	}
	if(Data.KEY2_DOWN_Flag)
	{
		Data.KEY2_DOWN_Flag = 0;
		Data.mm_2 = num2;
		sprintf((char*)LCD_Line_BUFF[C2_STATE],"    B2:%c            ",Data.mm_2 + '0');	//����ת�ַ�
		LCD_State |= (0x01<<C2_STATE);
		num2++;
		if(num2>9)
		{
			num2 = 0;
		}
	}
	if(Data.KEY3_DOWN_Flag)
	{
		Data.KEY3_DOWN_Flag = 0;
		Data.mm_3 = num3;
		sprintf((char*)LCD_Line_BUFF[C3_STATE],"    B3:%c            ",Data.mm_3 + '0');	//����ת�ַ�
		LCD_State |= (0x01<<C3_STATE);
		num3++;
		if(num3>9)
		{
			num3 = 0;
		}
	}	
	if(Data.KEY4_DOWN_Flag)
	{
		Data.KEY4_DOWN_Flag = 0;
		if(!Data.MENU_Flag)	//������ҳ��
		{
			if((Data.mm_1 == Password[0]) && (Data.mm_2 == Password[1]) && (Data.mm_3 == Password[2]))	//������ȷ
			{
				//���2KHz����10%ռ�ձ���ת���״̬ҳ��
				TIM2->ARR = 500;
				TIM2->CCR2 = 50;
				Data.freq = 1000000/TIM2->ARR;
				Data.duty = (float)TIM2->CCR2/TIM2->ARR*100;
				Data.MENU_Flag = 1;
				Data.Run_Count_5s = 1;
				LED_DIS(0x01,SET);	//����
				LCD_Dis2();
			}
			else
			{
				//�����������Ϊ@ ����+1
				Password_Error_Count++;
				LCD_Dis1();
			}
		}
	}
	if(Password_Error_Count >= 3)	//�������3�μ�����
	{
		Password_Error_Count = 0;	//��0
		Data.LED2_Count_5s = 1;	
	}	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t timer_100ms;
	static uint8_t timer_20ms;
	static uint16_t timer_5s;
	static uint16_t timer_LED_5s;
	
	if(htim == &htim6)
	{
		timer_20ms++;
		timer_100ms++;
		
		if(20 == timer_20ms)
		{
			timer_20ms = 0;
			KEY_function();
		}
		
		if(100 == timer_100ms)
		{
			timer_100ms = 0;
			KEY_COUNT++;
		}
		if(Data.Run_Count_5s)
		{
			timer_5s++;
			if(5000 == timer_5s)
			{
				timer_5s = 0;
				Data.Run_Count_5s = 0;
				LED_DIS(0x01,RESET);	//Ϩ��
				TIM2->ARR = 1000;	//�ָ�1KHz 50%ռ�ձ�
				TIM2->CCR2 = 500;
				Data.MENU_Flag = 0;
				LCD_Dis1();	//�ص�����ҳ��
			}
		}
		if(Data.LED2_Count_5s)
		{
			timer_LED_5s++;
			
			if(0 == (timer_LED_5s % 100))	//���100ms��˸
			{
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_9);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
			}
			if(5000 == timer_LED_5s)
			{
				timer_LED_5s = 0;
				Data.LED2_Count_5s = 0;
				LED_DIS(0x02,RESET);	//Ϩ��
			}
		}

	}
}

void USART1_function(void)
{
		
	if(RX_Over_Flag)
	{
		RX_Over_Flag = 0;
		One_Open_Flag++;
		if(One_Open_Flag >= 200)
		{
			One_Open_Flag = 2;
		}
		if(One_Open_Flag > 1)
		{
			if(7 == RX_LEN)	//����������
			{
				//��Ϊ�����������ַ���Ҫת����
				//���͸�ʽ��xxx-yyy
				if((Password[0] == (RX_BUFF[0]-'0')) && 
					(Password[1] == (RX_BUFF[1]-'0')) && 
				(Password[2] == (RX_BUFF[2]-'0')) && ('-' == RX_BUFF[3]))
				{
					//�ж��Ƿ���0~9֮��
					if((RX_BUFF[4] >= '0' && RX_BUFF[4] <= '9') &&
					   (RX_BUFF[5] >= '0' && RX_BUFF[5] <= '9') &&
					   (RX_BUFF[6] >= '0' && RX_BUFF[6] <= '9'))
					{
						Password[0] = RX_BUFF[4] - '0';
						Password[1] = RX_BUFF[5] - '0';
						Password[2] = RX_BUFF[6] - '0';
						HAL_UART_Transmit(&huart1,(uint8_t*)"�޸ĳɹ�!\r\n",sizeof("�޸ĳɹ�\r\n"),300);
					}
					else
					{
						HAL_UART_Transmit(&huart1,(uint8_t*)"�����뺬�зǷ��ַ�,�޸�ʧ��!\r\n",sizeof("�����뺬�зǷ��ַ�,�޸�ʧ��!\r\n"),300);
					}
				}
				else
				{
					HAL_UART_Transmit(&huart1,(uint8_t*)"�������,�޸�ʧ��!\r\n",sizeof("�������,�޸�ʧ��!\r\n"),300);
				}
			}
			else
			{
				HAL_UART_Transmit(&huart1,(uint8_t*)"���ȹ���,�޸�ʧ��!\r\n",sizeof("���ȹ���,�޸�ʧ��!\r\n"),300);
			}
		}
		memset((uint8_t*)RX_BUFF,0,sizeof(RX_BUFF));
		HAL_UART_Receive_DMA(&huart1,(uint8_t*)RX_BUFF,RX_MAX_LEN);	//���´򿪽���
	}
}

void TASK_Init(void)
{
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	LED_DIS(0xFF,RESET);
	HAL_TIM_Base_Start_IT(&htim6);
	TIM2->CCR2 = 500;
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);	//Ĭ��1000Hz
	LCD_Dis1();
}
