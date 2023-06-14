#include "allhead.h"

/*====================PWM���ģʽ������=======================*/
//ʹ�ö�ʱ��8ͨ��1,2
uint16_t pa6_duty = 100;	//PA6ͨ��1��ʼռ�ձ�10%
uint16_t pa7_duty = 500;	//PA7ͨ��1��ʼռ�ձ�50%
uint16_t pa6_fre = 1000;	//��ʱ����ʼARRֵ

//PA6����ռ�ձȺ��� ������ռ�ձ�ֵ��100~900
void pA6_SetDuty(uint16_t d)
{
	pa6_duty+=d;
	
	if(pa6_duty > 900)
	{
		pa6_duty = 100;
	}
	__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,pa6_duty);	//����ռ�ձ�
}

//PA7����ռ�ձȺ��� ������ռ�ձ�ֵ��100~900
void pA7_SetDuty(uint16_t d)
{
	pa7_duty+=d;
	
	if(pa7_duty > 900)
	{
		pa7_duty = 100;
	}
	__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,pa7_duty);	//����ռ�ձ�
}

//��ʱ��3����Ƶ�ʺ��� ������ÿ�����Ӷ��� ��λ %
void p_SetFre(uint16_t f) 
{
	uint16_t p_arr;
	uint32_t p_fre = 80000000/(31 + 1)/(htim8.Instance->ARR + 1);	//����Ƶ��
	float p_temp1 = (float)f/100;	//�ٷ���תС����ʽ����������ת������0��
	float temp2 = p_fre*p_temp1;	//�����Ҫ�Ӷ���
	p_fre += temp2;	//��ԭ�л��������
	p_arr = (80000000/(31 + 1))/p_fre;	//�����Ҫ��ARR
	
	if(p_fre > (2500+(2500*0.9)))	//������ֵ��0.9����ص���ֵƵ�ʣ���ARR�ص���ֵ����
	{
		p_arr = 1000;
	}
	__HAL_TIM_SetAutoreload(&htim8,p_arr - 1);	//����ARR
}




/*====================PWM����Ƚ�ģʽ������=======================*/

uint32_t ch1_pulse_val,ch2_pulse_val;	//ͨ��1��CRRxֵ   ͨ��2��CRRxֵ
uint32_t ch1_duty_val,ch2_duty_val;	//ͨ��1��Ч��ƽռ�ձ�����ֵ   ͨ��2��Ч��ƽռ�ձ�����ֵ
uint32_t ch1_high,ch2_high;
uint32_t ch1_low,ch2_low;

//����ͨ��1Ƶ�ʣ�ռ�ձ� ����1��Ƶ��Hz  ����2��ռ�ձ�%
void pwm_oc_setCH1(uint32_t freq,uint8_t duty)
{
	ch1_pulse_val = 1000000 / freq;	//���Ƶ�������CRRxֵ
	ch1_duty_val = ch1_pulse_val * duty / 100;	//�����Ч��ƽ����ֵ
	htim4.Instance->CCR1 = ch1_pulse_val;
}

//����ͨ��2Ƶ�ʣ�ռ�ձ� ����1��Ƶ��Hz  ����2��ռ�ձ�%
void pwm_oc_setCH2(uint32_t freq,uint8_t duty)
{
	ch2_pulse_val = 1000000 / freq;	//�����Ƶ�������CRRxֵ
	ch2_duty_val = ch2_pulse_val * duty / 100;	//�����Ч��ƽ����ֵ
	htim4.Instance->CCR2 = ch2_pulse_val;
}


//PWM�Ƚ�ģʽ�ص�����
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t pwm1_flag = 1;	//���巭ת��־λ
	static uint8_t pwm2_flag = 1;	//���巭ת��־λ
	
	if(htim == &htim4)	//����Ƕ�ʱ��4
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)	//�����ͨ��1
		{
			if(pwm1_flag)
			{
				htim4.Instance->CCR1 += ch1_duty_val;	//д����Ч��ƽ�����CRRxֵ
			}
			else
			{
				htim4.Instance->CCR1 += ch1_pulse_val - ch1_duty_val;	//д����Ч��ƽ�����CRRxֵ(��Ƶ��ֵ-�ߵ�ƽֵ=�͵�ƽֵ)
			}
			pwm1_flag = !pwm1_flag;	//ȡ��
		}
		
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)	//�����ͨ��2
		{
			if(pwm2_flag)
			{
				htim4.Instance->CCR2 += ch2_duty_val;	//д����Ч��ƽ�����CRRxֵ	
			}
			else
			{
				htim4.Instance->CCR2 += ch2_pulse_val - ch2_duty_val;	//д����Ч��ƽ�����CRRxֵ			
			}	
			pwm2_flag = !pwm2_flag;	//ȡ��
		}
	}
}




/*====================PWM���������=======================*/


//�����жϻص�����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	
	if(htim==(&htim2))
	{
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if(1 == TIM_IC_flag)
			{
				TIM_IC_flag++;
				TIM_IC_2.TIM_ONE = TIM2->CCR2;	//��¼��һ��������
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_FALLING);	//�½��ز���
			}
			else if(2 == TIM_IC_flag)
			{
				TIM_IC_flag++;
				TIM_IC_2.TIM_TWO = TIM2->CCR2;	//��¼��һ���½���
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_RISING);	//�����ز���			
			}
			else if(3 == TIM_IC_flag)
			{
				TIM_IC_flag++;
				TIM_IC_2.TIM_THREE = TIM2->CCR2;	//��¼�ڶ���������
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_FALLING);	//�½��ز���
			}
			else if(4 == TIM_IC_flag)
			{
				TIM_IC_flag++;
				TIM_IC_2.TIM_FOUR = TIM2->CCR2;	//��¼�ڶ����½���
				HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_2);	//ֹͣ����
				__HAL_TIM_SetCounter(&htim2, 0);	//����ֵ��0				
			}
		}
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
		{
			LED_Dis1();
		}
	}
	
	if(htim==(&htim3))
	{
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if(1 == TIM_IC_flag)
			{
				TIM_IC_flag++;
				TIM_IC_3.TIM_ONE = TIM3->CCR1;	//��¼��һ��������
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);	//�½��ز���
			}
			else if(2 == TIM_IC_flag)
			{
				TIM_IC_flag++;
				TIM_IC_3.TIM_TWO = TIM3->CCR1;	//��¼��һ���½���
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);	//�����ز���			
			}
			else if(3 == TIM_IC_flag)
			{
				TIM_IC_flag++;
				TIM_IC_3.TIM_THREE = TIM3->CCR1;	//��¼�ڶ���������
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);	//�½��ز���
			}
			else if(4 == TIM_IC_flag)
			{
				TIM_IC_flag++;
				TIM_IC_3.TIM_FOUR = TIM3->CCR1;	//��¼�ڶ����½���
				HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1);	//ֹͣ����
				__HAL_TIM_SetCounter(&htim3, 0);	//����ֵ��0				
			}
		}
	}	
}
