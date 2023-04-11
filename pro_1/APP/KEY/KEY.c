#include "allhead.h"

uint8_t KEY_Time_Count;
uint8_t key_value, key_up, key_down;
uint16_t Long_Num = 1;
uint8_t PWM1_Flag = 0;
uint8_t PWM2_Flag = 0;
static uint8_t pwm1_count,pwm2_count;

extern void enter_standby_mode(void);

unsigned char KEY_Scan(void)  //���з���ֵ�İ���ɨ�躯��
{
    unsigned char ucKey_value;	//����һ������ֵ�ı���

    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)	//����B1���¼�ֵ����Ϊ1
        ucKey_value = 1;

    else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)	//����B2���¼�ֵ����Ϊ2
        ucKey_value = 2;

    else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)	//����B3���¼�ֵ����Ϊ3
        ucKey_value = 3;

    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)	//����B4���¼�ֵ����Ϊ4
        ucKey_value = 4;

    else
        ucKey_value = 0;	//�ް������¼�ֵ����Ϊ0

    return ucKey_value;
}
void KEY_Proc(void)
{
    static uint8_t key_old;

    key_value = KEY_Scan();
    key_up = ~key_value & (key_old ^ key_value);

    key_down = key_value & (key_old ^ key_value);
    key_old = key_value;

    if(key_down)	//���а�������ʱ
    {
        KEY_Time_Count = 0; //����ʱ������ ���㿪ʼ��ʱ �˴�ʹ���˻�����ʱ�����ڼ�ʱ
    }

    if(KEY_Time_Count < 10) //�����ʱʱ��С��1s �̰�
    {
        switch(key_up) //�жϰ����Ƿ�̧�� ѡ���ִֵ�ж̰�����Ӧ����
        {
        case 1:
            menu_flag = !menu_flag;
		    LCD_Clear(Black);
            break;
        case 2:
			pwm1_count++;
			pwm2_count++;
			if(pwm1_count > 8)
			{
				pwm1_count = 0;
			}
			if(pwm2_count > 4)
			{
				pwm2_count = 0;
			}
			pwm_oc_setCH1(9000+(9000*(0.1*pwm1_count)),70);
			pwm_oc_setCH2(600+(600*(0.2*pwm2_count)),50);
//            light2468_open(1);
            //��Ӱ�������
            break;

        case 3:
        {
			PWM1_Flag = 1;
//            rtc_time.Minutes = 59;
//            rtc_date.Year = 24;
//            HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
//            HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
            //TXTest();
            //��Ӱ�������
            break;
        }
        case 4:
			PWM2_Flag = 1;
//            Buzzer_test(SET);
            //��Ӱ�������
            break;
        }
    }
    else //���� ��ʱʱ�䳬��1s
    {
        switch(key_value) //�жϰ����Ƿ��� ѡ�񰴼��ļ�ִֵ����Ӧ����
        {
        case 1:
            light1357_open(0);
            break;

        case 2:
            light2468_open(0);
            //��Ӱ�������
            break;

        case 3:
			__HAL_RCC_RTC_ENABLE();
//            Long_Num += 1;
            //��Ӱ�������
            break;

        case 4:
            Buzzer_test(RESET);
            //��Ӱ�������
            break;
        }

    }
}
