#include "my.h"


TASK_TIMERTypeDef task_timer = {0};
PWM_DUTY_TypeDef pwm_duty = {1000, 500, 0, 0};	//��ʼ��ΪPA6PA7��10ռ�ձ�
DATA_TypeDef Data = {0};
uint32_t sys_time = 0;

uint8_t KEY_UP, KEY_DOWN, KEY_VALUE;
uint16_t KEY_COUNT;

uint32_t ADC_BUFF[10];	//�ɼ�ADC10�ν��Ϊһ��


uint16_t LCD_State = 0xFFFF;

uint8_t LCD_Line_BUFF[10][20] =
{
    "                   1",	//���� 0
    "                    ",
    "                    ",	//���� 2
    "                    ",
    "                    ",	//���� 4
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    "
};




//LCD��ʾ
void LCD_function(void)
{
    for(uint8_t i = 0; i < 10; i++)
    {
        if(LCD_State & (1 << i))
        {
            LCD_State &= (~(0x01 << i));
            LCD_DisplayStringLine(i * 24, LCD_Line_BUFF[i]);
        }
    }
}




void LCD_Dis1(void)
{
    sprintf((char *)LCD_Line_BUFF[BT_State], "       Data         ");
    sprintf((char *)LCD_Line_BUFF[C1_State], "     V:%.2fv", pwm_duty.ADC_value);
    if(Data.MODE_Flag)	//�ֶ�
    {
        sprintf((char *)LCD_Line_BUFF[C2_State], "     Mode:MAMU      ");
    }
    else	//�Զ�
    {
        sprintf((char *)LCD_Line_BUFF[C2_State], "     Mode:AUTO      ");
    }
    LCD_State |= 0xFF;
}

void LCD_Dis2(void)
{
    sprintf((char *)LCD_Line_BUFF[BT_State], "       Para         ");
    sprintf((char *)LCD_Line_BUFF[C1_State], "     PA6:%d%%        ", pwm_duty.PA6_duty / 100);
    sprintf((char *)LCD_Line_BUFF[C2_State], "     PA7:%d%%        ", pwm_duty.PA7_duty / 50);
    LCD_State |= 0xFF;
}

//ADC��ȡ+�˲�
void Get_ADC(void)
{
    uint32_t temp;
    uint8_t count = 0;
    uint32_t adc;
    if(!task_timer.ADC_TIMER_COUNT)
    {
        for(uint8_t i = 0; i < 10 - 1; i++)
        {
            count = 0;
            for(uint8_t j = 0; j < 10 - 1 - i; j++)
            {
                if(ADC_BUFF[j] > ADC_BUFF[j + 1])	//����
                {
                    temp = ADC_BUFF[j];
                    ADC_BUFF[j] = ADC_BUFF[j + 1];
                    ADC_BUFF[j + 1] = temp;
                    count = 1;
                }
                if(0 == count)
                    break;
            }
        }
        for(uint16_t k = 1; k < 10 - 1; k++)
        {
            adc += ADC_BUFF[k];
        }
        pwm_duty.ADC_value = (float)(adc / 8) * 3.3f / 4096;
        pwm_duty.auto_duty1 =  pwm_duty.ADC_value / 3.3f * 10000;	//�Զ�ģʽ��ռ�ձ�TIM16 ����ΪARR��10000��
        pwm_duty.auto_duty2 = pwm_duty.ADC_value / 3.3f * 5000;	//�Զ�ģʽ��ռ�ձ�TIM17����ΪARR��5000��
    }
    if(!Data.MENU_Flag)
    {
        LCD_Dis1();
    }
}


uint8_t KEY_Pro(void)
{
    if((!KEY1) || (!KEY2) || (!KEY3) || (!KEY4))
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

void KEY_function(void)
{
    static uint8_t KEY_OldVALUE;
    if(!task_timer.KEY_TIMER_COUNT)
    {
        task_timer.KEY_TIMER_COUNT = KEY_TIME;
        KEY_VALUE = KEY_Pro();

        KEY_UP = ~KEY_VALUE & (KEY_VALUE ^ KEY_OldVALUE);
        KEY_DOWN = KEY_VALUE & (KEY_VALUE ^ KEY_OldVALUE);
        KEY_OldVALUE = KEY_VALUE;

        if(KEY_DOWN)
            KEY_COUNT = 0;

        if(KEY_COUNT < 10)
        {
            switch(KEY_UP)
            {
            case 1:
            {
                Data.KEY1_DOWN_Flag = 1;
                break;
            }
            case 2:
            {
                if(!Data.MENU_Flag)	//����ҳ��ʧЧ
                {
                    break;
                }
                Data.KEY2_DOWN_Flag = 1;
                break;
            }
            case 3:
            {

                if(!Data.MENU_Flag)	//����ҳ�滥��Ƶ��
                {
                    uint32_t temp;
                    temp = TIM16->ARR;
                    TIM16->ARR = TIM17->ARR;
                    TIM17->ARR = temp;
                    Data.SWITCH_Flag = 1;
                    break;
                }
                else	//����ҳ��
                {
                    Data.KEY3_DOWN_Flag = 1;
                    break;
                }
            }
            case 4:
            {
                Data.KEY4_DOWN_Flag = 1;
                break;
            }
            default:
                break;
            }
        }
    }
}

//��־λִ��
void Flag_Run_fuction(void)
{
    if(Data.KEY1_DOWN_Flag)	//����1�л�ҳ��
    {
        Data.KEY1_DOWN_Flag = 0;
        switch(Data.MENU_Flag)
        {
        case 0:
        {
            Data.MENU_Flag = 1;
            LED_DIS(0x02, RESET);
            LCD_Dis2();
            break;
        }
        case 1:
        {
            Data.MENU_Flag = 0;
            LED_DIS(0x02, SET);
            LCD_Dis1();
            break;
        }
        }
    }
    if(Data.KEY2_DOWN_Flag)	//����2
    {
        Data.KEY2_DOWN_Flag = 0;
        pwm_duty.PA6_duty += 1000;	//ռ�ձȼ�10%
        if(pwm_duty.PA6_duty > 9000)
        {
            pwm_duty.PA6_duty = 1000;
        }
        TIM16->CCR1 = pwm_duty.PA6_duty;
        sprintf((char *)LCD_Line_BUFF[C1_State], "     PA6:%d%%        ", pwm_duty.PA6_duty / 100);
        LCD_State |= (0x01 << C1_State);
    }
    if(Data.KEY3_DOWN_Flag)	//����3
    {
        Data.KEY3_DOWN_Flag = 0;
        pwm_duty.PA7_duty += 500;	//ռ�ձȼ�10%
        if(pwm_duty.PA7_duty >= 5000)
        {
            pwm_duty.PA7_duty = 500;
        }
        TIM17->CCR1 = pwm_duty.PA7_duty;
        sprintf((char *)LCD_Line_BUFF[C2_State], "     PA7:%d%%        ", pwm_duty.PA7_duty / 50);
        LCD_State |= (0x01 << C2_State);
    }
    if(Data.KEY4_DOWN_Flag)	//����4�л��ֶ��Զ�
    {
        Data.KEY4_DOWN_Flag = 0;
        Data.MODE_Flag = !Data.MODE_Flag;	//ȡ�����Զ�--���� �ֶ�--Ϩ��
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }
}

void PWM_OUT(void)
{
    if(1 == Data.MODE_Flag)	//�ֶ�
    {
        if(Data.SWITCH_Flag)	//����Ƶ��
        {
            TIM16->CCR1 = pwm_duty.PA7_duty;
            TIM17->CCR1 = pwm_duty.PA6_duty;
        }
        else
        {
            TIM16->CCR1 = pwm_duty.PA6_duty;
            TIM17->CCR1 = pwm_duty.PA7_duty;
        }


    }
    if(0 == Data.MODE_Flag)	//�Զ�	ռ�ձ� = ADC��ѹֵ*3.3V
    {
        if(Data.SWITCH_Flag)	//����л���
        {
            if((pwm_duty.ADC_value > 0) && (pwm_duty.ADC_value < 3.3f))
            {
                TIM16->CCR1 = pwm_duty.auto_duty2;
                TIM17->CCR1 = pwm_duty.auto_duty1;
            }
            else if(pwm_duty.ADC_value >= 3.3f)
            {
                TIM16->CCR1 = 5000;	//����ߵ�ƽ
                TIM17->CCR1 = 10000;	//����ߵ�ƽ
            }
            else if(pwm_duty.ADC_value <= 0)
            {
                TIM16->CCR1 = 0;	//����͵�ƽ
                TIM17->CCR1 = 0;	//����͵�ƽ
            }
        }
        else
        {
            if((pwm_duty.ADC_value > 0) && (pwm_duty.ADC_value < 3.3f))
            {
                TIM16->CCR1 = pwm_duty.auto_duty1;
                TIM17->CCR1 = pwm_duty.auto_duty2;
            }
            else if(pwm_duty.ADC_value >= 3.3f)
            {
                TIM16->CCR1 = 10000;	//����ߵ�ƽ
                TIM17->CCR1 = 5000;	//����ߵ�ƽ
            }
            else if(pwm_duty.ADC_value <= 0)
            {
                TIM16->CCR1 = 0;	//����͵�ƽ
                TIM17->CCR1 = 0;	//����͵�ƽ
            }
        }


    }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t time_100ms;
    static uint16_t time_150ms;
    if(htim == &htim6)
    {
        sys_time++;
        time_100ms++;
        time_150ms++;
        if(task_timer.KEY_TIMER_COUNT)
            task_timer.KEY_TIMER_COUNT--;
        if(task_timer.ADC_TIMER_COUNT)
            task_timer.ADC_TIMER_COUNT--;
        if(100 == time_100ms)
        {
            KEY_COUNT++;
        }
        if(150 == time_150ms)	//150msɨ��һ��
        {
            time_150ms = 0;
            PWM_OUT();
        }
    }

}


//��ʼ��
void TASK_Init(void)
{
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    LED_DIS(0xFF, RESET);
    HAL_TIM_Base_Start_IT(&htim6);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);	//ADCУ׼
    HAL_ADC_Start_DMA(&hadc2, (uint32_t *)ADC_BUFF, 10);	//��ʼת��
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
    LED_DIS(0x03, SET);	//Ĭ�����Զ�ģʽLED1����
    LCD_Dis1();
}

void LED_DIS(uint8_t num, uint8_t swch)
{
    if(SET == swch)
    {
        HAL_GPIO_WritePin(GPIOC, (uint16_t)num << 8, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, (uint16_t)num << 8, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

