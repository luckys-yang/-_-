#include "allhead.h"


//ƽ��ֵ�˲�
void ADC_DMA_Test(void)
{
    ADC_2 = 0;
    for(uint8_t i = 0; i < 30; i++)
    {
        ADC_2 += ADC2_Value[i];
    }
    ADC_1 = 0;
    for(uint8_t i = 0; i < 30; i++)
    {
        ADC_1 += ADC1_Value[i];
    }
}

//ADC��λֵ�˲���ADCת��10��ֵ���õ�ð������
float Adc_Proc(uint32_t _buf[])
{
    uint8_t i, j,count;
    float Adc_Val = 0;
    float temp;
    for(i = 0; i < 10 - 1; i++)	//-1����Ϊ���ø��Լ���
    {
		count = 0;
        for(j = 0; j < 10 - 1 - i; j++)	//size-1-i����Ϊÿһ�˾ͻ���һ�����Ƚ�
        {
            if (_buf[j] < _buf[j + 1])	//��������
            {
                temp = _buf[j];
                _buf[j] = _buf[j + 1];
                _buf[j + 1] = temp;
				count = 1;
            }
        }
		if(0 == count)	//���ĳһ��û�н���λ�ã���˵���Ѿ��ź���ֱ���˳�ѭ��
		{
			break;
		}
    }
	//ȥ����С�����ֵȻ��ȡƽ��ֵ
    for(i = 1; i < 10 - 1; i++)
	{
        Adc_Val += _buf[i];		
	}

    return (Adc_Val / 8) / 4096 * 3.3f;
}


//��ȡADC��ֵ����
double getADC(ADC_HandleTypeDef *hadc)
{
    uint32_t adc;
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, 500); //�ȴ��ɼ���������ʱ��500ms
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(hadc), HAL_ADC_STATE_REG_EOC))//��ȡADC��ɱ�־λ
    {
        adc = HAL_ADC_GetValue(hadc);
        return adc * 3.3 / 4096;
    }

    return 0;	//���󷵻�0
}

//����DACֵ
void set_DAC_value(DAC_HandleTypeDef *hdac,uint32_t Channel,float Vol)
{
	uint16_t temp;
	temp = Vol*4096/3.3f;
	HAL_DAC_SetValue(hdac,Channel,DAC_ALIGN_12B_R,temp);	//����DAC���ֵ
}
