#include "allhead.h"

//#include "fonts.h"	//����Ҫ�������ͷ�ļ����򱨴�

uint8_t arr1[20] = "HELLO World         ";	//��һ����ʾ���ַ���
uint8_t pos[5] = {1, 4, 7, 8, 9};	//��Ҫ�������������������ܳ��������ﶨ��ģ������Ҫ��Ҫ�ں�������Ӽ���Ԫ�ؼ���(������ֻ�ܶ಻����)

void MY_LCD_Dis(void)
{
    LCD_DisplayStringLine(Line0, arr1);	//��1����ʾ�ִ���
    hight_string(arr1, Line1, pos, 5, White, Red);
    LCD_DisplayChar(Line2, 320 - (16 * 1), '@');	//�ڵ�2�е�2����ʾ�ַ�@
}

//ĳ����ʾ�ִ����Ҹ�������ַ�
/*
����1��Ҫ��ʾ���ַ���
����2������һ��(1~20)
����3���ڸ����ļ���(320~1)
����4����Ҫ����������
����5��Ĭ�ϵı�����ɫ
����6����Ҫ��������ɫ
*/
void hight_string(uint8_t *str, uint8_t Line, const uint8_t *pos, uint8_t pos_len, uint16_t color, uint16_t hight_color)
{
    uint8_t i = 0, k;
    //���Ч�ʵ�����forѭ��ִ�У�len x 19��
#if 0
    for(i = 0; i < pos_len; i++)
    {
        for(j = 0; j < 19; j++)
        {
            if(j != pos[i])
            {
                LCD_DisplayChar(Line, 320 - (16 * j), str[j]);	//���������Ҫ�������ַ���Ĭ����ʾ
            }

        }
    }
    for(k = 0; k < pos_len; k++)
    {
        LCD_SetBackColor(hight_color);	//��Ҫ��������ɫ
        LCD_DisplayChar(Line, 320 - (16 * pos[k]), str[pos[k]]);	//Ҫ�������ַ�
    }
    LCD_SetBackColor(color);	//����Ҫ����ʱ����ʾ��ɫ
#endif

    //�������Ч�ʸߵ㲻�����ʱ������ԭ������Ǽ���forѭ���Ĵ������ִ��Ч�ʣ�forѭ��ֻ��ִ��19�ε�����Ҫ�ֶ�������Ҫ����������
#if 1
    for(i = 0; i < 19; i++)
    {
        if((i != pos[0]) && (i != pos[1]) && (i != pos[2]) && (i != pos[3]) && (i != pos[4]))
        {
            LCD_DisplayChar(Line, 320 - (16 * i), str[i]);	//���������Ҫ�������ַ���Ĭ����ʾ
        }
    }
    for(k = 0; k < pos_len; k++)
    {
        LCD_SetBackColor(hight_color);	//��Ҫ��������ɫ
        LCD_DisplayChar(Line, 320 - (16 * pos[k]), str[pos[k]]);	//Ҫ�������ַ�
    }
    LCD_SetBackColor(color);	//����Ҫ����ʱ����ʾ��ɫ
#endif
}
//����ĳ����������ɫ�����������һ��ֻ�ǰ� "LCD_SetBackColor"���"LCD_SetTextColor"

//��ʾ����
void MY_LCD_DisNum(uint32_t num)
{

    uint8_t DisNum_arr[20] = "";	//Ҫ��ʼ��Ϊ0��Ȼ����ո������ܲ����ɾ�
    uint8_t TestArr[20];
    uint8_t ADC1_Num[20] = "";	//ADC1ͨ��11
    uint8_t ADC2_Num[20] = "";	//ADC2
    uint8_t DAC_Num[20] = "";	//DACͨ��1
    uint8_t ADC5_Num[20] = "";	//ADC1ͨ��5
	uint8_t RTC_Timer[20] = "";	//RTCʱ��
	uint8_t RTC_Date[20] = "";	//RTC����
    uint8_t DisNum_pos[20] = {13, 14, 15, 16, 17};	//��Ҫ����������
    uint32_t dis_num = num;
	
	MY_LCD_Dis();
    sprintf((char *)DisNum_arr, "TIM_NVIC_num:%d", dis_num); //��Ҫ��%d�����һ���ո��������Ӱ
    MY_LCD_Arr_Completion(DisNum_arr);	//����ո����������Ӱ
    hight_string(DisNum_arr, Line3, DisNum_pos, 5, Black, Yellow);

	sprintf((char *)RTC_Timer, "time:%02d:%02d:%02d",rtc_time.Hours,rtc_time.Minutes,rtc_time.Seconds);
    MY_LCD_Arr_Completion((uint8_t *)RTC_Timer);
    LCD_DisplayStringLine(Line8, (uint8_t *)RTC_Timer);
	
    sprintf((char *)RTC_Date, "date:%02d-%02d-%02d week:%d",rtc_date.Year,rtc_date.Month,rtc_date.Date,rtc_date.WeekDay);
    MY_LCD_Arr_Completion((uint8_t *)RTC_Date);
    LCD_DisplayStringLine(Line9, (uint8_t *)RTC_Date);	
	

    if(RX_flag)
    {
        RX_flag = 0;
        LCD_ClearLine(Line4);
        LCD_ClearLine(Line5);

        sprintf((char *)TestArr, "%d %d", d1.x, d1.y);
        LCD_DisplayStringLine(Line4, (uint8_t *)d1.c1);
        LCD_DisplayStringLine(Line5, (uint8_t *)TestArr);
        memset(TestArr, 0, sizeof(TestArr));
    }
    if(ADC_flag)
    {
        ADC_flag = 0;
        sprintf((char *)ADC1_Num, "adc1-11:%.2f", (double)ADC1_Value[0] / 4096 * 3.3f);
        MY_LCD_Arr_Completion((uint8_t *)ADC1_Num);
        LCD_DisplayStringLine(Line5, (uint8_t *)ADC1_Num);

        sprintf((char *)ADC2_Num, "adc2-15:%.2f", Adc_Proc(ADC2_Value));
        MY_LCD_Arr_Completion((uint8_t *)ADC2_Num);
        LCD_DisplayStringLine(Line6, (uint8_t *)ADC2_Num);

        sprintf((char *)DAC_Num, "DAC_value:%d", HAL_DAC_GetValue(&hdac1, DAC_CHANNEL_1));
        MY_LCD_Arr_Completion((uint8_t *)DAC_Num);
        LCD_DisplayStringLine(Line7, (uint8_t *)DAC_Num);
#if 0
        sprintf((char *)ADC5_Num, "adc1-5:%.2f", (double)ADC1_Value[1] / 4096 * 3.3f);
        MY_LCD_Arr_Completion((uint8_t *)ADC5_Num);
        LCD_DisplayStringLine(Line8, (uint8_t *)ADC5_Num);
#endif	
		
		
    }
}

//���鲹��ո�
void MY_LCD_Arr_Completion(uint8_t *ptr)
{
    uint8_t i = 0;
    while(i < 20)
    {
        if(0 == ptr[i])
        {
            ptr[i] = ' ';
        }
        i++;
    }
}

//����2��ʾpwm���
uint8_t uart1_scan[20] = "UART:               ";
void pwm_display(void)
{
    uint8_t pwm_A6_1[20] = "";
    uint8_t pwm_A7_1[20] = "";
    uint8_t pwm_A6_fre[20] = "";
    uint8_t pwm_oc_scan[20] = "";
    uint8_t pwm_ic_scan[20] = "";
	
    float temp1 = ((float)htim3.Instance->CCR1) / (htim3.Instance->ARR + 1);	// ͨ��1���Ƚ�ֵ/ARR+1��ע������Ϊ�˱���С����
    float temp2 = ((float)htim3.Instance->CCR2) / (htim3.Instance->ARR + 1);	// ͨ��2���Ƚ�ֵ/ARR+1
    uint32_t tim3_fre = 80000000 / (31 + 1) / (htim3.Instance->ARR + 1);

    //TIM3PWMģʽ
    sprintf((char *)pwm_A6_1, "A6:Dty-%d%% CCR1-%d", (uint16_t)(temp1 * 100), htim3.Instance->CCR1);
    MY_LCD_Arr_Completion(pwm_A6_1);
    LCD_DisplayStringLine(Line0, (uint8_t *)pwm_A6_1);

    sprintf((char *)pwm_A7_1, "A7:Dty-%d%% CCR2-%d", (uint16_t)(temp2 * 100), htim3.Instance->CCR2);
    MY_LCD_Arr_Completion(pwm_A7_1);
    LCD_DisplayStringLine(Line1, (uint8_t *)pwm_A7_1);

    sprintf((char *)pwm_A6_fre, "Time_Fre:%dHz", tim3_fre);
    MY_LCD_Arr_Completion(pwm_A6_fre);
    LCD_DisplayStringLine(Line2, (uint8_t *)pwm_A6_fre);

    //TIM4�Ƚ�ģʽ
    sprintf((char *)pwm_oc_scan, "1F:%dHz D:%.2f%%", 1000000 / ch1_pulse_val, (double)ch1_duty_val / ch1_pulse_val * 100);
    MY_LCD_Arr_Completion(pwm_oc_scan);
    LCD_DisplayStringLine(Line3, (uint8_t *)pwm_oc_scan);
    memset(pwm_oc_scan, 0, sizeof(pwm_oc_scan));


    sprintf((char *)pwm_oc_scan, "2F:%dHz D:%.2f%%", 1000000 / ch2_pulse_val, (double)ch2_duty_val / ch2_pulse_val * 100);
    MY_LCD_Arr_Completion(pwm_oc_scan);
    LCD_DisplayStringLine(Line4, (uint8_t *)pwm_oc_scan);
    memset(pwm_oc_scan, 0, sizeof(pwm_oc_scan));


    pwm_ic_flag = 0;
    sprintf((char *)pwm_ic_scan, "f1:%dHz d1:%.2f", TIM_IC_2.TIM_freq, TIM_IC_2.TIM_zkb);
    MY_LCD_Arr_Completion(pwm_ic_scan);
    LCD_DisplayStringLine(Line5, (uint8_t *)pwm_ic_scan);
    memset(pwm_ic_scan, 0, sizeof(pwm_ic_scan));


    sprintf((char *)pwm_ic_scan, "f2:%dHz d2:%.2f", TIM_IC_3.TIM_freq, TIM_IC_3.TIM_zkb);
    MY_LCD_Arr_Completion(pwm_ic_scan);
    LCD_DisplayStringLine(Line6, (uint8_t *)pwm_ic_scan);
    memset(pwm_ic_scan, 0, sizeof(pwm_ic_scan));
	
	LCD_DisplayStringLine(Line7,(uint8_t *)uart1_scan);
}
