#include "my.h"

TASK_TIMESTypeDef TASK_TIMES;
uint16_t LCD_state = 0xFFFF;	//�б仯�ı��
RTC_TimeTypeDef rtc_time;	//ʱ��ṹ��
RTC_DateTypeDef rtc_date;	//���ڽṹ��

uint8_t Dang[] = {0x00,0x00,0x00,0x00,0x10,0x60,0xC0,0xC0,0x80,
				  0x00,0xF8,0x00,0x00,0x00,0x00,0xF0,0x00,0x00,
				  0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,0x08,
				  0x18,0x18,0x18,0x18,0x18,0x98,0x58,0xFF,0x00,
	              0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0xFF,
				  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,
	              0x03,0x01,0x00,0x00,0x3F,0x18,0x18,0x18,0x18,
	              0x1F,0x18,0x18,0x18,0x18,0x1F,0x18,0x18,0x00};/*"��",0*/

uint8_t Qian[] = {0x00,0x80,0x00,0x00,0x00,0xFE,0x00,0x00,0xF0,
				  0x10,0x10,0x10,0xF0,0x10,0x10,0x10,0xF0,0x10,
				  0x10,0x10,0x10,0x90,0x10,0x00,0x00,0x00,0x81,
				  0x83,0x42,0xFF,0x00,0x00,0x47,0xC4,0x44,0x44,
	              0x47,0x44,0x44,0x44,0x47,0x44,0xC4,0x04,0x84,
				  0x07,0x02,0x00,0x00,0x00,0x01,0x00,0x10,0x3F,
	              0x00,0x04,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,
	              0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x07,0x02,0x00};/*"ǰ",0*/			  
		
uint8_t Ping[] = {0x00,0x00,0x00,0xF8,0x00,0x00,0x20,0xC0,0x80,
				  0x80,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
				  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				  0xFF,0x18,0x18,0x18,0x18,0x18,0x98,0x58,0xFF,
				  0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
				  0x18,0x08,0x00,0x00,0x00,0x08,0x1F,0x00,0x04,
				  0x06,0x02,0x01,0x00,0x10,0x3F,0x00,0x00,0x00,
				  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*"ƽ",0*/

uint8_t Tai[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,
				 0x18,0xF8,0x10,0x00,0xC0,0x40,0x40,0x40,0x40,
				 0x40,0x40,0xC0,0x40,0x00,0x00,0x00,0x08,0x1C,
				 0x06,0x02,0x01,0x00,0x00,0x00,0xF0,0x0F,0x00,
				 0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
				 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				 0x01,0x06,0x0C,0x1F,0x18,0x10,0x00,0x07,0x02,
				 0x02,0x02,0x02,0x02,0x02,0x03,0x02,0x02,0x00};/*"̨",0*/	

uint8_t LCD_Line_BUFF[10][20] =
{
    "                    ",	//0
    "                    ",	//1
    "                    ",	//2
    "                    ",	//3
    "                    ",	//4
    "         1          ",	//5
    "                    ",	//6
    "                    ",	//7
    "      00:00:00      ",	//8
    "                    "	//9
};

//LCD���
void LCD_function(void)
{
    uint8_t i;

    for(i = 0; i < 10; i++)
    {
		if(i != 2)	//��ֹ������������ʾ����
		{
			if(LCD_state & (0x01 << i))	//������з����仯(�ϵ�Ĭ��ȫ���������仯)
			{
				LCD_state &= (~(1 << i));	//���״̬
				LCD_DisplayStringLine(i * 24, LCD_Line_BUFF[i]);	//������ʾ����
			}			
		}
    }
}

//������ʾ����
/*
����1��x����(0~240)
����2��y����(319~0)
����3��������ɫ
����4��������ɫ
����5����Ҫ��ʾ����ģ����
*/
void LCD_DrawMonoPict1(uint16_t Xpos, uint16_t Ypos, uint32_t BackColor, uint32_t TextColor, uint8_t *Pict)
{
    uint8_t index = 0, i = 0, Xpos_Temp;
    Xpos_Temp = Xpos;	//�����ʼ����
    Ypos = 319 - (24 * Ypos);	//���ƶ��ٸ���������ʾ24*24һ�������ʾ13��
    LCD_SetCursor(Xpos, Ypos); 	//д������
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

    for(index = 0; index < 72; index++)
    {
        //��Ҫ�ı�Y����(����)��0-24  24-48  48-72
        if(24 == index || 48 == index)
        {
            Xpos = Xpos_Temp;	//x����ָ���ʼ
            Ypos = Ypos - 8;	//Y����8������
        }
        for(i = 0; i < 8; i++)
        {
            if((Pict[index] & (1 << i)) == 0x00)	//�����Ӧλ��������0x00����ʾ������ɫ
            {
                LCD_WriteRAM(BackColor);
            }
            else	//��Ϊ0��ʾ����
            {
                LCD_WriteRAM(TextColor);
            }
        }
        LCD_SetCursor(Xpos++, Ypos); 	//д������
        LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    }
}

//��ʼ��
void TASK_Init(void)
{
    LCD_Init();	//LCD��ʼ��
    LCD_SetBackColor(Black);	//����
    LCD_SetTextColor(White);	//������ɫ
	LED_DIS(0xFF,RESET);	//LEDĬ����
    HAL_TIM_Base_Start_IT(&htim6);	//��ʱ��6�����ж�
    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);	//��ȡʱ��
    HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);	//��ȡ����
    Data.Platform = 1;	//Ĭ��ƽ̨Ϊ1
    Data.SJJ_Open_Over = 1;	//Ĭ�����������Ž���״̬
    Data.Platform_UPorDOWN = 1;	//Ĭ����
    Data.KEY_InFore_Flag = 1;	//����Ĭ����Ч
    LED_DIS(0xF0, SET);	//��ˮ��Ĭ��ȫ������ʾ�ɰ�
	
	LCD_DisplayStringLine(Line2,(uint8_t*)"                    ");	//�ȿո񸲸�����ʾ
	LCD_DrawMonoPict1(Line2,4,Black,White,Dang);	//"��"
	LCD_DrawMonoPict1(Line2,5,Black,White,Qian);	//"ǰ"
	LCD_DrawMonoPict1(Line2,6,Black,White,Ping);	//"ƽ"
	LCD_DrawMonoPict1(Line2,7,Black,White,Tai);		//"̨"
}


//RTC��ʾ(Ĭ��MX����ʱ�䣺12:50:55)
void RTC_function(void)
{
    static uint8_t Seconds_last;
    if(!TASK_TIMES.RTC_TIME)	//200ms��ȡһ��RTCʱ��
    {
        TASK_TIMES.RTC_TIME = RTC_TIMEST;
        HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);	//��ȡʱ��
        HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);	//��ȡ����

        if(Seconds_last != rtc_time.Seconds)
        {
            Seconds_last = rtc_time.Seconds;
            sprintf((char *)LCD_Line_BUFF[LCD_RTC], "      %02d:%02d:%02d      ", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
            LCD_state |= (0x01 << LCD_RTC);
        }
    }
}

//����
uint8_t KEY_pro(void)
{
    uint8_t value;
    if((!KEY1) || (!KEY2) || (!KEY3) || (!KEY4))
    {
        if(!KEY1)
            value = 1;
        if(!KEY2)
            value = 2;
        if(!KEY3)
            value = 3;
        if(!KEY4)
            value = 4;

        return value;
    }
    return 0;
}


uint8_t KEY_UP;	//�����ɿ�˲����Ч
uint8_t KEY_DOWN;	//����˲����Ч
uint8_t KEY_VALUE;	//������ֵ
uint16_t KEY_COUNT = 0;	//�̰���ʱ


//ִ�ж�Ӧ������־λ״̬
void KEY_SCAN(void)
{
    static uint8_t KEY_Old;

    if(!TASK_TIMES.KEY_time)
    {
        TASK_TIMES.KEY_time = KEY_TIMEST;
        KEY_VALUE = KEY_pro();
        KEY_UP = ~(KEY_VALUE) & (KEY_Old ^ KEY_VALUE);	//̧��˲������Ч
        KEY_DOWN = (KEY_VALUE) & (KEY_Old ^ KEY_VALUE);	//����˲����Ч
        KEY_Old = KEY_VALUE;
        if(KEY_DOWN)
        {
            KEY_COUNT = 0;
        }

        if(KEY_COUNT < 10)	//С��1000ms�̰����ǵȴ�����
        {
            if(0 == Data.KEY_InFore_Flag)
            {
                return;	//�˳�
            }
            switch(KEY_UP)
            {
            case 1:
            {
                if(1 == Data.Platform)	//��ǰƽ̨���°���ûЧ��
                {
                    break;
                }
                Data.LED1_State = 1;	//ȥƽ̨1
                Data.KEY_Timer_Start = 1;
                Data.KEY_Timer_Count = 0;	//��0�ȴ�ʱ��
                break;
            }

            case 2:
            {
                if(2 == Data.Platform)	//��ǰƽ̨���°���ûЧ��
                {
                    break;
                }
                Data.LED2_State = 1;	//ȥƽ̨2
                Data.KEY_Timer_Start = 1;
                Data.KEY_Timer_Count = 0;	//��0�ȴ�ʱ��
                break;
            }

            case 3:
            {
                if(3 == Data.Platform)	//��ǰƽ̨���°���ûЧ��
                {
                    break;
                }
                Data.LED3_State = 1;	//ȥƽ̨3
                Data.KEY_Timer_Start = 1;
                Data.KEY_Timer_Count = 0;	//��0�ȴ�ʱ��
                break;
            }

            case 4:
            {
                if(4 == Data.Platform)	//��ǰƽ̨���°���ûЧ��
                {
                    break;
                }
                Data.LED4_State = 1;	//ȥƽ̨4
                Data.KEY_Timer_Start = 1;
                Data.KEY_Timer_Count = 0;	//��0�ȴ�ʱ��
                break;
            }

            default:
                break;
            }
        }
    }
}



DATA_TypeDef Data;	//���ݽṹ��

//�жϱ�־λ��ɹ���
void Flag_Run(void)
{

    if(Data.LED1_State)
    {
        LED_DIS(0x01, SET);	//����LED1
    }
    if(Data.LED2_State)
    {
        LED_DIS(0x02, SET);	//����LED1
    }
    if(Data.LED3_State)
    {
        LED_DIS(0x04, SET);	//����LED1
    }
    if(Data.LED4_State)
    {
        LED_DIS(0x08, SET);	//����LED1
    }
    if(Data.KEY_Timer_Start)	//���µȴ���ʼ��ʱ
    {
        if(KEY_OVER_TIME <= Data.KEY_Timer_Count)	//1s
        {
            Data.KEY_Timer_Start = 0;	//�����־λ
            Data.KEY_Timer_Over = 1;
        }
    }
    if(Data.KEY_Timer_Over)	//���µȴ����
    {
        Data.KEY_Timer_Over = 0;
        Data.SJJ_Close_Start = 1;
        LED_DIS(0xF0, RESET);	//��ȫ���ʾ����
        if(0 == Check_UP())
            Data.Platform_UPorDOWN = 0;
        else
            Data.Platform_UPorDOWN = 1;
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);	//PA5�͵�ƽ
        Data.PWM_Out_OpenClose_Flag = 1;	//����pwm
        Data.KEY_InFore_Flag = 0;	//������Ч
        Data.SJJ_OpenClose_Timer_Count = 0;	//��0
    }
    if(Data.SJJ_Close_Start)	//��ʼ����������
    {
        if(SJJ_OC_OVER_TIME <= Data.SJJ_OpenClose_Timer_Count)	//4s
        {
            Data.SJJ_Close_Start = 0;	//�����־λ
            Data.PWM_Out_OpenClose_Flag = 3;	//���ŵ��pwm�����Ч
            Data.SJJ_Close_Over = 1;
        }

    }

    if(Data.SJJ_Wait_Start)	//��ʼͣ����ʱ
    {
        Data.KEY_InFore_Flag = 1;	//������Ч
        if(SJJ_WAIT_OVER_TIME <= Data.SJJ_Wait_Timer_Count)	//2s
        {
            Data.SJJ_Wait_Start = 0;
            Data.SJJ_Wait_Over = 1;
        }
    }
    if(Data.SJJ_Wait_Over)	//ͣ�����
    {
        Data.SJJ_Wait_Over = 0;
        Data.SJJ_Close_Over = 1;
        Data.KEY_InFore_Flag = 0;	//������Ч
    }
    if(Data.Platform_NumberBlink_Flag)	//��˸�����
    {
        Data.Platform_NumberBlink_Flag = 0;
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);	//PA5�ߵ�ƽ
        Data.PWM_Out_UpDown_Flag = 3;	//���µ�������Ч
        Data.SJJ_OpenClose_Timer_Count = 0;	//��0
        Data.PWM_Out_OpenClose_Flag = 2;	//���ŵ��pwm���
        Data.SJJ_Open_Start = 1;	//��ʼ����
    }
    if(Data.SJJ_Open_Start)	//��ʼ����������
    {
        if(SJJ_OC_OVER_TIME <= Data.SJJ_OpenClose_Timer_Count)	//4s
        {
            Data.SJJ_Open_Start = 0;	//�����־λ
            Data.PWM_Out_OpenClose_Flag = 3;	//���ŵ��pwm�����Ч
            Data.SJJ_Open_Over = 1;
        }
    }
    if(Data.Platform_UPorDOWN)	//��
    {
        SJJ_UP();
    }
    else if(0 == Data.Platform_UPorDOWN)	//��
    {
        SJJ_DOWN();
    }
}

//�������½���Ŀ��
void SJJ_DOWN(void)
{
    if(Data.SJJ_Close_Over)	//�������������
    {
        Data.SJJ_Close_Over = 0;

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);	//PA4�͵�ƽ�½�
        Data.PWM_Out_UpDown_Flag = 2;
        Data.SJJ_UpDown_Timer_Count = 0;	//��0
        Data.SJJ_DOWN_Start = 1;	//��ʼ�½���־λ
    }
    if(Data.SJJ_DOWN_Start)	//��ʼ�½�
    {
        if(SJJ_UD_OVER_TIME <= Data.SJJ_UpDown_Timer_Count)	//6s
        {
            Data.SJJ_DOWN_Start = 0;
            LED_DIS(0xF0, RESET);
            Data.SJJ_DOWN_Over = 1;
        }
    }
    if(Data.SJJ_DOWN_Over)	//�½����
    {
        Data.Platform--;	//ƽ̨--
        if(Check() == 0 )	//δ����
        {
            Data.SJJ_UpDown_Timer_Count = 0;	//��0
            Data.SJJ_DOWN_Start = 1;	//�����½�
        }
        Data.SJJ_DOWN_Over = 0;
    }
    if(Data.SJJ_Open_Over)	//��������жϵ�ǰƽ̨������û��Ŀ��ƽ̨���жϵ�ǰ����(��Ϊ��ĿҪ�����Ϻ���)
    {

        if(Check_UP())	//�о�ͣ��2s����
        {
            Data.SJJ_Wait_Timer_Count = 0;
            Data.SJJ_Wait_Start = 1;
            Data.Platform_UPorDOWN = 1;	//����
        }
        else	//û�о��ж�������û��
        {

            if(Check_DOWN())	//���������Ŀ����ʼ�½�
            {
                Data.SJJ_Wait_Timer_Count = 0;
                Data.SJJ_Wait_Start = 1;
                Data.Platform_UPorDOWN = 0;	//����
            }
            else	//û��
            {

                LED_DIS(0xF0, SET);
                Data.KEY_InFore_Flag = 1;	//������Ч
            }
        }

        Data.SJJ_Open_Over = 0;
    }
}

//������������Ŀ��
void SJJ_UP(void)
{

    if(Data.SJJ_Close_Over)	//�������������
    {
        Data.SJJ_Close_Over = 0;

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);	//PA4�ߵ�ƽ����
        Data.PWM_Out_UpDown_Flag = 1;
        Data.SJJ_UpDown_Timer_Count = 0;	//��0
        Data.SJJ_UP_Start = 1;
    }
    if(Data.SJJ_UP_Start)	//��ʼ����
    {
        if(SJJ_UD_OVER_TIME <= Data.SJJ_UpDown_Timer_Count)	//6s
        {
            Data.SJJ_UP_Start = 0;
            LED_DIS(0xF0, RESET);
            Data.SJJ_UP_Over = 1;
        }
    }
    if(Data.SJJ_UP_Over)	//�������
    {
        Data.Platform++;	//ƽ̨++
        if(Check() == 0 )	//δ����
        {
            Data.SJJ_UpDown_Timer_Count = 0;	//��0
            Data.SJJ_UP_Start = 1;	//��������
        }
        Data.SJJ_UP_Over = 0;
    }
    if(Data.SJJ_Open_Over)	//��������жϵ�ǰƽ̨���滹��û�У����������û���ж�����
    {

        if(Check_UP())	//�о�ͣ��2s����
        {
            Data.SJJ_Wait_Timer_Count = 0;
            Data.SJJ_Wait_Start = 1;
        }
        else	//û�о��ж�������û��
        {

            if(Check_DOWN())	//���������Ŀ����ʼ�½�
            {
                Data.SJJ_Wait_Timer_Count = 0;
                Data.SJJ_Wait_Start = 1;
                Data.Platform_UPorDOWN = 0;	//����
            }
            else	//û����ָ��ɰ�״̬
            {

                LED_DIS(0xF0, SET);
                Data.KEY_InFore_Flag = 1;	//������Ч
            }
        }

        Data.SJJ_Open_Over = 0;
    }
}

//��⵱ǰƽ̨������û��Ŀ��ƽ̨
uint8_t Check_UP(void)
{
    uint8_t temp = 0;
    for(uint8_t i = Data.Platform + 1; i <= 4; i++)
    {
        if(2 == i)
        {
            if(Data.LED2_State)
            {
                temp++;
            }
        }
        if(3 == i)
        {
            if(Data.LED3_State)
            {
                temp++;
            }
        }
        if(4 == i)
        {
            if(Data.LED4_State)
            {
                temp++;
            }
        }
    }

    return temp;
}

//��⵱ǰƽ̨������û��Ŀ��ƽ̨
uint8_t Check_DOWN(void)
{
    uint8_t temp;
    for(uint8_t j = Data.Platform - 1; j > 0; j--)
    {
        if(1 == j)
        {
            if(Data.LED1_State)
            {
                temp++;
            }
        }
        if(2 == j)
        {
            if(Data.LED2_State)
            {
                temp++;
            }
        }
        if(3 == j)
        {
            if(Data.LED3_State)
            {
                temp++;
            }
        }
        if(4 == j)
        {
            if(Data.LED4_State)
            {
                temp++;
            }
        }

    }

    return temp;
}

//����Ƿ񵽴�Ŀ��ƽ̨  ����0��ʾδ���� >0��ʾ����
uint8_t Check(void)
{
    uint8_t i = 0;
    switch(Data.Platform)
    {
    case 1:
    {
        if(Data.LED1_State)
        {
            Data.LED1_State = 0;
            LED_DIS(0x01, RESET);	//Ϩ��LED
            Data.Platform_Over = 1;	//�����־λ��1
            Data.SJJ_DOWN_Start = 0;
            Data.SJJ_UP_Start = 0;
            i++;
        }
        break;
    }
    case 2:
    {
        if(Data.LED2_State)
        {
            Data.LED2_State = 0;
            LED_DIS(0x02, RESET);	//Ϩ��LED
            Data.Platform_Over = 1;	//�����־λ��1
            Data.SJJ_DOWN_Start = 0;
            Data.SJJ_UP_Start = 0;
            i++;
        }
        break;
    }
    case 3:
    {
        if(Data.LED3_State)
        {
            Data.LED3_State = 0;
            LED_DIS(0x04, RESET);	//Ϩ��LED
            Data.Platform_Over = 1;
            Data.SJJ_DOWN_Start = 0;
            Data.SJJ_UP_Start = 0;
            i++;
        }
        break;
    }
    case 4:
    {
        if(Data.LED4_State)
        {
            Data.LED4_State = 0;
            LED_DIS(0x08, RESET);	//Ϩ��LED
            Data.Platform_Over = 1;
            Data.SJJ_DOWN_Start = 0;
            Data.SJJ_UP_Start = 0;
            i++;
        }
        break;
    }
    default:
        break;
    }
    return i;
}




//LED ����/Ϩ������
void LED_DIS(uint8_t num, uint8_t swch)
{
    if(SET == swch)
    {
        HAL_GPIO_WritePin(GPIOC, (uint16_t)num << 8, GPIO_PIN_RESET);	//����
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, (uint16_t)num << 8, GPIO_PIN_SET);	//��
    }
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);	//����
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);	//����
}

typedef struct
{
    uint32_t pulse_val;	//Ƶ������ֵ
    uint32_t duty_val;	//��Ч��ƽ����ֵ
} TIM_OCTypeDef;	//����Ƚ�

TIM_OCTypeDef TIM3_CH1;	//ͨ��1(PA6)
TIM_OCTypeDef TIM3_CH2;	//ͨ��2(PA7)

//����PA6Ƶ��ռ�ձ�
void PWM_SET_PA6(uint32_t freq, uint16_t duty)
{
    TIM3_CH1.pulse_val = 1000000 / freq;	//����Ƶ������ֵ
    TIM3_CH1.duty_val = TIM3_CH1.pulse_val * duty / 100;	//������Ч��ƽ����ֵ
    TIM3->CCR1 = TIM3_CH1.pulse_val;
}

//����PA7Ƶ��ռ�ձ�
void PWM_SET_PA7(uint32_t freq, uint16_t duty)
{
    TIM3_CH2.pulse_val = 1000000 / freq;	//����Ƶ������ֵ
    TIM3_CH2.duty_val = TIM3_CH2.pulse_val * duty / 100;	//������Ч��ƽ����ֵ
    TIM3->CCR2 = TIM3_CH2.pulse_val;
}


//����Ƚϻص�����
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t PWM_CH1_Flag = 1;
    static uint8_t PWM_CH2_Flag = 1;

    if(htim == &htim3)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)	//ͨ��1
        {
            if(PWM_CH1_Flag)
            {
                TIM3->CCR1 += TIM3_CH1.duty_val;	//�ߵ�ƽ����ʱ��
            }
            else
            {
                TIM3->CCR1 += TIM3_CH1.pulse_val - TIM3_CH1.duty_val;	//�͵�ƽ����ʱ��
            }
            PWM_CH1_Flag = !PWM_CH1_Flag;	//ȡ��
        }

        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)	//ͨ��2
        {
            if(PWM_CH2_Flag)
            {
                TIM3->CCR2 += TIM3_CH2.duty_val;	//�ߵ�ƽ����ʱ��
            }
            else
            {
                TIM3->CCR2 += TIM3_CH2.pulse_val - TIM3_CH2.duty_val;	//�͵�ƽ����ʱ��
            }
            PWM_CH2_Flag = !PWM_CH2_Flag;	//ȡ��
        }
    }

}

uint32_t sys_time = 0;	//ϵͳʱ��

//��ʱ��6�ص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint16_t count_1s = 0;
    static uint16_t count_200ms = 0;
    static uint8_t LED_Index = 1;

    if(htim == &htim6)
    {
        sys_time++;
        Data.KEY_Timer_Count++;	//�������µȴ�ʱ��
        Data.SJJ_OpenClose_Timer_Count++;	//������ʱ��
        Data.SJJ_UpDown_Timer_Count++;	//����ʱ��
        Data.SJJ_Wait_Timer_Count++;	//ͣ��ʱ��

        if(TASK_TIMES.KEY_time)
        {
            TASK_TIMES.KEY_time--;
        }
        if(TASK_TIMES.RTC_TIME)
        {
            TASK_TIMES.RTC_TIME--;
        }
        if(0 == (sys_time % 100))	//�൱��100ms++һ��
        {
            KEY_COUNT++;	//�����̰�����
        }
        if(Data.Platform_Over)	//ƽ̨������˸
        {
            count_1s++;
            if(count_1s == 10)
            {
                sprintf((char *)LCD_Line_BUFF[LCD_NUMBER], "                    ");
                LCD_state |= (0x01 << LCD_NUMBER);
            }
            if(count_1s == 400)
            {
                sprintf((char *)LCD_Line_BUFF[LCD_NUMBER], "         %d          ", Data.Platform);
                LCD_state |= (0x01 << LCD_NUMBER);
            }
            if(count_1s == 600)
            {
                sprintf((char *)LCD_Line_BUFF[LCD_NUMBER], "                     ");
                LCD_state |= (0x01 << LCD_NUMBER);
            }
            if(count_1s == 1000)
            {
                count_1s = 0;
                Data.Platform_Over = 0;
                Data.Platform_NumberBlink_Flag = 1;
                sprintf((char *)LCD_Line_BUFF[LCD_NUMBER], "         %d          ", Data.Platform);
                LCD_state |= (0x01 << LCD_NUMBER);
            }

        }
		//����
		if(1 == Data.PWM_Out_UpDown_Flag)	
		{
			Data.PWM_Out_UpDown_Flag = 0;
			PWM_SET_PA6(1000, 80);	//PA6����Ƶ��1000ռ�ձ�80   
			HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);	//ͨ��1			
		}
		else if(2 == Data.PWM_Out_UpDown_Flag)
		{
			Data.PWM_Out_UpDown_Flag = 0;
			PWM_SET_PA6(1000, 60);	//PA6�½�Ƶ��1000ռ�ձ�60   
			HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);	//ͨ��1			
		}
		else if(3 == Data.PWM_Out_UpDown_Flag)
		{
			Data.PWM_Out_UpDown_Flag = 0;
			PWM_SET_PA6(1000, 0);	//PA6Ƶ��1000ռ�ձ�0 ��Ч״̬   
			HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);	//ͨ��1				
		}
		else
		{
			;
		}
		
		//����
	    if(1 == Data.PWM_Out_OpenClose_Flag)	
		{
			Data.PWM_Out_OpenClose_Flag = 0;
			PWM_SET_PA7(2000, 50);	//PA7����Ƶ�� 2000ռ�ձ�50
			HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2);	//ͨ��1			
		}
		else if(2 == Data.PWM_Out_OpenClose_Flag)
		{
			Data.PWM_Out_OpenClose_Flag = 0;
			PWM_SET_PA7(2000, 60);	//PA7����Ƶ��2000ռ�ձ�60   
			HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2);	//ͨ��1			
		}
		else if(3 == Data.PWM_Out_OpenClose_Flag)
		{
			Data.PWM_Out_OpenClose_Flag = 0;
			PWM_SET_PA7(2000, 0);	//PA7Ƶ��2000ռ�ձ�0 ��Ч״̬   
			HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2);	//ͨ��1				
		}
		else{
			;
		}
		

		if(Data.SJJ_UP_Start)	//����ʱ����ˮ����
        {
            count_200ms++;
            if(200 == count_200ms)
            {
                count_200ms = 0;
                switch(LED_Index)
                {
                case 1:
                    LED_DIS(0x10, SET);LED_Index = 2;break;
                case 2:
                    LED_DIS(0x20, SET);LED_Index = 3;break;
                case 3:
                    LED_DIS(0x40, SET);LED_Index = 4;break;
                case 4:
                    LED_DIS(0x80, SET);LED_Index = 5;break;
                case 5:
                    LED_DIS(0x10, RESET);LED_Index = 6;break;
                case 6:
                    LED_DIS(0x20, RESET);LED_Index = 7;break;
                case 7:
                    LED_DIS(0x40, RESET);LED_Index = 8;break;
                case 8:
                    LED_DIS(0x80, RESET);LED_Index = 1;break;
                }
            }
        }
        else if(Data.SJJ_DOWN_Start)	//�½�ʱ����ˮ����
        {
            count_200ms++;
            if(200 <= count_200ms)
            {
                count_200ms = 0;
                switch(LED_Index)
                {
                case 1:
                    LED_DIS(0x80, SET);LED_Index = 2;break;
                case 2:
                    LED_DIS(0x40, SET);LED_Index = 3;break;
                case 3:
                    LED_DIS(0x20, SET);LED_Index = 4;break;
                case 4:
                    LED_DIS(0x10, SET);LED_Index = 5;break;
                case 5:
                    LED_DIS(0x80, RESET);LED_Index = 6;break;
                case 6:
                    LED_DIS(0x40, RESET);LED_Index = 7;break;
                case 7:
                    LED_DIS(0x20, RESET);LED_Index = 8;break;
                case 8:
                    LED_DIS(0x10, RESET);LED_Index = 1;break;
                }
            }
        }
        else	//�����ָ�
        {
            LED_Index = 1;
        }		
    }
}
