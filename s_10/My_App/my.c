#include "my.h"

uint32_t LCD_State = 0xFFFF;
uint8_t LCD_Line_BUFF[10][20] =
{
    "                    ",
    "                    ",
    "                    ",	//2		ҳ��1,2����
    "                    ",	//3		ҳ��2 ����1
    "                    ",	//4		ҳ��2 ����2
    "                    ",	//5  	ҳ��1��ǰ��ѹ ҳ��2 ����3
    "                    ",	//6  	ҳ��1��ǰ״̬ ҳ��2 ����4
    "                    ",
    "                    ",
    "                    ",
};

static uint8_t choose_Index = 0;	//����2��������������һ�ΰ���ʱ��λ�ã�Ĭ��0
PARAMETER_TypeDef parameter_Data = {0};
TASK_TypeDef task_timer = {0};
DATA_TypeDef Data = {0};


//ҳд
void EEPROM_Page_Write(uint8_t addr, uint8_t *date, uint8_t len)
{
    I2CStart();
    I2CSendByte(0xA0);	//д����
    I2CWaitAck();

    I2CSendByte(addr);	//д���ַ
    I2CWaitAck();

    while(len--)
    {
        I2CSendByte(*date++);
        I2CWaitAck();

        addr++;
        if((addr & 0x07) == 0)
        {
            break;
        }
    }
    I2CStop();
    HAL_Delay(10);
}



//ҳ��
void EEPROM_Page_Read(uint8_t addr, uint8_t *date, uint8_t len)
{
    I2CStart();
    I2CSendByte(0xA0);	//д��
    I2CWaitAck();

    I2CSendByte(addr);
    I2CWaitAck();
    I2CStart();
    I2CSendByte(0xA1);
    I2CWaitAck();
    while(len--)
    {
        *date++ = I2CReceiveByte();
        if(len)
            I2CSendAck();
        else
            I2CSendNotAck();
    }
    I2CStop();
}

//д�����
void Parameter_Write(PARAMETER_TypeDef *para)
{
    //�洢ʱ����10��������ʽ����
    uint8_t Temp[4] = {0};
    Temp[0] = para->UP_LED;
    Temp[1] = para->DOWN_LED;
    Temp[2] = para->V_UP * 10;
    Temp[3] = para->V_DOWN * 10;

    EEPROM_Page_Write(DATA_ADDR, (uint8_t *)Temp, 4);
}

//��ȡ����
void Parameter_Read(PARAMETER_TypeDef *para)
{
    //ȡ��ʱ����10�ָ�С��
    uint8_t Temp[4] = {0};

    EEPROM_Page_Read(DATA_ADDR, (uint8_t *)Temp, 4);
    para->UP_LED = Temp[0];
    para->DOWN_LED = Temp[1];
    para->V_UP = Temp[2] / (float)10;
    para->V_DOWN = Temp[3] / (float)10;
}

//�������
uint8_t KEY_UP, KEY_DOWN, KEY_VALUE, KEY_TIMER_COUNT;

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
    static uint8_t key_oldValue;
    if(!task_timer.KEY_TIME_COUNT)
    {
        task_timer.KEY_TIME_COUNT = KEY_TIME;
        KEY_VALUE = KEY_Pro();
        KEY_UP = ~KEY_VALUE & (KEY_VALUE ^ key_oldValue);
        KEY_DOWN = KEY_VALUE & (KEY_VALUE ^ key_oldValue);
        key_oldValue = KEY_VALUE;

        if(KEY_DOWN)
        {
            KEY_TIMER_COUNT = 0;
        }

        if(KEY_TIMER_COUNT < 10)
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
                if(0 == Data.MENU_State)	//2,3,4��������ҳ��ʧЧ
                {
                    break;
                }
                Data.KEY2_DOWN_Flag = 1;
                break;
            }
            case 3:
            {
                if((0 == Data.MENU_State) || (0 == Data.High_Line_Flag))	//2,3,4��������ҳ��ʧЧ����Ҫ����2ѡ�в��������Ч
                {
                    break;
                }
                Data.KEY3_DOWN_Flag = 1;
                break;
            }
            case 4:
            {
                if((0 == Data.MENU_State) || (0 == Data.High_Line_Flag))	//2,3,4��������ҳ��ʧЧ����Ҫ����2ѡ�в��������Ч
                {
                    break;
                }
                Data.KEY4_DOWN_Flag = 1;
                break;
            }
            default:
                break;
            }
        }
    }
}


//��־λ����ʵ��
void Run_function(void)
{
    if(Data.KEY1_DOWN_Flag)
    {
        Data.KEY1_DOWN_Flag = 0;
        choose_Index = 0;	//����2�������ָ���ֵ
        Data.MENU_State = !Data.MENU_State;	//ȡ��
        if(Data.MENU_State)	//�л�����������ҳ��
        {
            LCD_Dis2();
        }
        else	//�л�����ҳ�沢�ұ��������EEEPROM
        {
            Parameter_Write(&parameter_Data);	//д��
            LCD_Dis1();
        }
    }
    if(Data.KEY2_DOWN_Flag)
    {
        Data.KEY2_DOWN_Flag = 0;
        switch(choose_Index)
        {
        case 0:	//ѡ�����޵�ѹ��
        {
            Data.High_Line_Flag = 1;	//������һ��ѡ��
            LCD_State |= (0x01 << C1_State);
            choose_Index = 1;
            break;
        }
        case 1:	//ѡ�����޵�ѹ��
        {
            Data.High_Line_Flag = 2;	//�����ڶ���ѡ��
            LCD_State |= (0x01 << C2_State);
            choose_Index = 2;
            break;
        }
        case 2:	//ѡ������LED��
        {
            Data.High_Line_Flag = 3;	//����������ѡ��
            LCD_State |= (0x01 << C3_State);
            choose_Index = 3;
            break;
        }
        case 3:	//ѡ������LED��
        {
            Data.High_Line_Flag = 4;	//�������ĸ�ѡ��
            LCD_State |= (0x01 << C4_State);
            choose_Index = 0;
            break;
        }
        default:
            break;
        }

    }
    if(Data.KEY3_DOWN_Flag)
    {
        Data.KEY3_DOWN_Flag = 0;
        switch(Data.High_Line_Flag)
        {
        case 1:	//���޵�ѹ�ı�
        {
            Add_V(&parameter_Data, SET);
            break;
        }
        case 2:	//���޵�ѹ�ı�
        {
            Add_V(&parameter_Data, RESET);
            break;
        }
        case 3:	//����LED�ı�(��Ҫ�ر�����LED��ѡ����֮ǰ�ĵƻ�����)
        {
            LED_DIS(0xFF, RESET);
            Add_LED(&parameter_Data, SET);
            break;
        }
        case 4:	//����LED�ı�
        {
            LED_DIS(0xFF, RESET);
            Add_LED(&parameter_Data, RESET);
            break;
        }
        default:
            break;
        }
    }
    if(Data.KEY4_DOWN_Flag)
    {
        Data.KEY4_DOWN_Flag = 0;
        switch(Data.High_Line_Flag)
        {
        case 1:	//���޵�ѹ�ı�
        {
            Reduce_V(&parameter_Data, SET);
            break;
        }
        case 2:	//���޵�ѹ�ı�
        {
            Reduce_V(&parameter_Data, RESET);
            break;
        }
        case 3:	//����LED�ı�
        {
            LED_DIS(0xFF, RESET);
            Reduce_LED(&parameter_Data, SET);
            break;
        }
        case 4:	//����LED�ı�
        {
            LED_DIS(0xFF, RESET);
            Reduce_LED(&parameter_Data, RESET);
            break;
        }
        default:
            break;
        }
    }
}

//�ж��Ƿ񳬳����ƺ���
void Judge_function(void)
{
	uint16_t x = 0,y = 0,z = 0;
	x = parameter_Data.Now_V*100;	//����100���ٱȽϣ���Ϊ��ѹֵ��С�������������ʱ���ʺ�С�����������;�ȷ
	y = parameter_Data.V_UP*100;
	z = parameter_Data.V_DOWN*100;
    if(x > y)	//��ǰ��ѹ>����
    {
        Data.UP_LED_Flag = 1;	//����LED��
        Data.DOWN_LED_Flag = 0;	//����LEDϨ��

        if(!Data.MENU_State)	//��ҳ�����ʾ״̬ ����ҳ�治��ʾ
        {
            sprintf((char *)LCD_Line_BUFF[C4_State], "    Status:%s", "Upper");	//״̬
            LCD_State |= (0x01 << C4_State);
        }
    }
    if(x < z)	//��ǰ��ѹ<����
    {
        Data.UP_LED_Flag = 0;	//����LEDϨ��
        Data.DOWN_LED_Flag = 1;	//����LED��
	
        if(!Data.MENU_State)	//��ҳ�����ʾ״̬ ����ҳ�治��ʾ
        {
            sprintf((char *)LCD_Line_BUFF[C4_State], "    Status:%s", "Lower");	//״̬
            LCD_State |= (0x01 << C4_State);
        }
    }
	
    if((x <= y) && (x >= z))	//����<= ��ǰ��ѹ <=����
    {
        Data.UP_LED_Flag = 0;	//����LEDϨ��
        Data.DOWN_LED_Flag = 0;	//����LEDϨ��
        LED_DIS(0xFF, RESET);
        if(!Data.MENU_State)	//��ҳ�����ʾ״̬ ����ҳ�治��ʾ
        {
            sprintf((char *)LCD_Line_BUFF[C4_State], "    Status:%s", "Normal");	//״̬
            LCD_State |= (0x01 << C4_State);
        }
    }
}

//���ӵ�ѹ����
//����1����ѹ���ݴ洢�ṹ��
//����2��SET--���� RESET--����
void Add_V(PARAMETER_TypeDef *para, uint8_t dir)
{
    if(SET == dir)
    {
        para->V_UP += 0.3f;
        if(para->V_UP >= 3.3f)
        {
            para->V_UP = 3.3;
        }
        sprintf((char *)LCD_Line_BUFF[C1_State], "Max Volt:%.1fV", parameter_Data.V_UP);	//���޵�ѹ
        LCD_State |= (0x01 << C1_State);	//ˢ��

    }
    else
    {
        para->V_DOWN += 0.3f;
        if(para->V_DOWN >= 3.3f)
        {
            para->V_DOWN = 3.3;
        }
		if((para->V_DOWN) >= (para->V_UP))	//������޴��ڵ�����������������ǣ����ڵ����ֵ
		{
			para->V_DOWN = para->V_UP;
		}		
        sprintf((char *)LCD_Line_BUFF[C2_State], "Min Volt:%.1fV", parameter_Data.V_DOWN);	//���޵�ѹ
        LCD_State |= (0x01 << C2_State);	//ˢ��
    }
}

//����LED����
//����1��LED���ݴ洢�ṹ��
//����2��SET--����LED RESET--����LED
void Add_LED(PARAMETER_TypeDef *para, uint8_t dir)
{
    if(SET == dir)
    {
        para->UP_LED += 1;
        if(para->UP_LED >= 8)
        {
            para->UP_LED = 8;
        }
        if((para->UP_LED) == (para->DOWN_LED))	//�����Ⱦ��������LED
        {
            if(para->UP_LED >= 8)
            {
                para->UP_LED = 1;
            }
            else
            {
                para->UP_LED += 1;
            }
        }
        sprintf((char *)LCD_Line_BUFF[C3_State], "Upper:LD%d", parameter_Data.UP_LED);	//���޵�ѹ
        LCD_State |= (0x01 << C3_State);	//ˢ��

    }
    else
    {
        para->DOWN_LED += 1;
        if(para->DOWN_LED >= 8)
        {
            para->DOWN_LED = 8;
        }
        if((para->DOWN_LED) == (para->UP_LED))	//�����Ⱦ��������LED
        {
            if(para->DOWN_LED >= 8)
            {
                para->DOWN_LED = 1;
            }
            else
            {
                para->DOWN_LED += 1;
            }
        }
        sprintf((char *)LCD_Line_BUFF[C4_State], "Lower:LD%d", parameter_Data.DOWN_LED);	//���޵�ѹ
        LCD_State |= (0x01 << C4_State);	//ˢ��
    }
}

//���ٵ�ѹ����
//����1����ѹ���ݴ洢�ṹ��
//����2��SET--���� RESET--����
void Reduce_V(PARAMETER_TypeDef *para, uint8_t dir)
{
    if(SET == dir)
    {
        para->V_UP -= 0.3f;
        if(para->V_UP <= 0)
        {
            para->V_UP = 0;
        }
		if((para->V_UP) <= (para->V_DOWN))	//�������С�ڵ�����������������ǣ����ڵ����ֵ(��Ϊ����������С�����޵�)
		{
			para->V_UP = para->V_DOWN;
		}		
        sprintf((char *)LCD_Line_BUFF[C1_State], "Max Volt:%.1fV", parameter_Data.V_UP);	//���޵�ѹ
        LCD_State |= (0x01 << C1_State);	//ˢ��

    }
    else
    {
        para->V_DOWN -= 0.3f;
        if(para->V_DOWN <= 0)
        {
            para->V_DOWN = 0;
        }
        sprintf((char *)LCD_Line_BUFF[C2_State], "Min Volt:%.1fV", parameter_Data.V_DOWN);	//���޵�ѹ
        LCD_State |= (0x01 << C2_State);	//ˢ��
    }
}

//����LED����
//����1��LED���ݴ洢�ṹ��
//����2��SET--����LED RESET--����LED
void Reduce_LED(PARAMETER_TypeDef *para, uint8_t dir)
{
    if(SET == dir)
    {
        para->UP_LED -= 1;
        if(para->UP_LED <= 1)
        {
            para->UP_LED = 1;
        }
        if((para->UP_LED) == (para->DOWN_LED))	//�����Ⱦ��������LED
        {
            if(para->UP_LED <= 1)
            {
                para->UP_LED = 8;
            }
            else
            {
                para->UP_LED -= 1;
            }
        }
        sprintf((char *)LCD_Line_BUFF[C3_State], "Upper:LD%d", parameter_Data.UP_LED);	//���޵�ѹ
        LCD_State |= (0x01 << C3_State);	//ˢ��

    }
    else
    {
        para->DOWN_LED -= 1;
        if(para->DOWN_LED <= 1)
        {
            para->DOWN_LED = 1;
        }
        if((para->DOWN_LED) == (para->UP_LED))	//�����Ⱦ��������LED
        {
            if(para->DOWN_LED <= 1)
            {
                para->DOWN_LED = 8;
            }
            else
            {
                para->DOWN_LED -= 1;
            }
        }
        sprintf((char *)LCD_Line_BUFF[C4_State], "Lower:LD%d", parameter_Data.DOWN_LED);	//���޵�ѹ
        LCD_State |= (0x01 << C4_State);	//ˢ��
    }
}



//R37��ѹ��ȡ
void GetAdc(void)
{
    uint32_t adc;
    if(!task_timer.ADC_TIME_COUNT)
    {
        task_timer.ADC_TIME_COUNT = ADC_TIME;
        HAL_ADC_Start(&hadc2);
        HAL_ADC_PollForConversion(&hadc2, 300);	//300ms��ʱ
        if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2), HAL_ADC_STATE_REG_EOC))	//��ȡ���
        {
            adc = HAL_ADC_GetValue(&hadc2);
            parameter_Data.Now_V = adc * 3.3 / 4096;
            if(parameter_Data.Now_V > (double)3.29)
                parameter_Data.Now_V = 3.3;
        }
    }
}

//LED��˸
void LED_Flicker(void)
{
    if(Data.LED_SCAN_Flag)
    {
        Data.LED_SCAN_Flag = 0;
        if(Data.UP_LED_Flag)
        {
            switch(parameter_Data.UP_LED)
            {
            case 1:
                LED_Togg(0x01);
                break;
            case 2:
                LED_Togg(0x02);
                break;
            case 3:
                LED_Togg(0x04);
                break;
            case 4:
                LED_Togg(0x08);
                break;
            case 5:
                LED_Togg(0x10);
                break;
            case 6:
                LED_Togg(0x20);
                break;
            case 7:
                LED_Togg(0x40);
                break;
            case 8:
                LED_Togg(0x80);
                break;
            default:
                break;
            }
        }
        if(!Data.UP_LED_Flag)	//˳���жϱ�־λ����0ʱֱ��Ϩ����Ϊ��ʱ��ת��ոպ��ǵ͵�ƽ���ָı���ָʾ�ƾͻᵼ�³���
        {
            switch(parameter_Data.UP_LED)
            {
            case 1:
                LED_DIS(0x01,RESET);
                break;
            case 2:
                LED_DIS(0x02,RESET);
                break;
            case 3:
                LED_DIS(0x04,RESET);
                break;
            case 4:
                LED_DIS(0x08,RESET);
                break;
            case 5:
                LED_DIS(0x10,RESET);
                break;
            case 6:
                LED_DIS(0x20,RESET);
                break;
            case 7:
                LED_DIS(0x40,RESET);
                break;
            case 8:
                LED_DIS(0x80,RESET);
                break;
            default:
                break;
            }
        }		
        if(Data.DOWN_LED_Flag)
        {
            switch(parameter_Data.DOWN_LED)
            {
            case 1:
                LED_Togg(0x01);
                break;
            case 2:
                LED_Togg(0x02);
                break;
            case 3:
                LED_Togg(0x04);
                break;
            case 4:
                LED_Togg(0x08);
                break;
            case 5:
                LED_Togg(0x10);
                break;
            case 6:
                LED_Togg(0x20);
                break;
            case 7:
                LED_Togg(0x40);
                break;
            case 8:
                LED_Togg(0x80);
                break;
            default:
                break;
            }
        }
        if(!Data.DOWN_LED_Flag)
        {
            switch(parameter_Data.DOWN_LED)
            {
            case 1:
                LED_DIS(0x01,RESET);
                break;
            case 2:
                LED_DIS(0x02,RESET);
                break;
            case 3:
                LED_DIS(0x04,RESET);
                break;
            case 4:
                LED_DIS(0x08,RESET);
                break;
            case 5:
                LED_DIS(0x10,RESET);
                break;
            case 6:
                LED_DIS(0x20,RESET);
                break;
            case 7:
                LED_DIS(0x40,RESET);
                break;
            case 8:
                LED_DIS(0x80,RESET);
                break;
            default:
                break;
            }
        }			
    }
}

//LED
void LED_DIS(uint8_t num, uint8_t swch)
{
    if(SET == swch)
    {
        HAL_GPIO_WritePin(GPIOC, (uint16_t)num << 8, GPIO_PIN_RESET);	//����
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, (uint16_t)num << 8, GPIO_PIN_SET);	//Ϩ��
    }
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

//LED��ת
void LED_Togg(uint8_t num)
{
    HAL_GPIO_TogglePin(GPIOC, (uint16_t)num << 8);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

//ҳ��1��ʾ
void LCD_Dis1(void)
{
    Data.High_Line_Flag = 0;
    sprintf((char *)LCD_Line_BUFF[BT_State], "        Main");	//����
    sprintf((char *)LCD_Line_BUFF[C1_State], "                    ");
    sprintf((char *)LCD_Line_BUFF[C2_State], "                    ");
    sprintf((char *)LCD_Line_BUFF[C3_State], "    Volt:%.2fV", parameter_Data.Now_V);	//��ѹ


    LCD_State |= 0xFF;
}

//ҳ��2��ʾ
void LCD_Dis2(void)
{
    sprintf((char *)LCD_Line_BUFF[BT_State], "      Setting");	//����
    sprintf((char *)LCD_Line_BUFF[C1_State], "Max Volt:%.1fV", parameter_Data.V_UP);	//���޵�ѹ
    sprintf((char *)LCD_Line_BUFF[C2_State], "Min Volt:%.1fV", parameter_Data.V_DOWN);	//���޵�ѹ
    sprintf((char *)LCD_Line_BUFF[C3_State], "Upper:LD%d           ", parameter_Data.UP_LED);	//���޵�
    sprintf((char *)LCD_Line_BUFF[C4_State], "Lower:LD%d           ", parameter_Data.DOWN_LED);	//���޵�

    LCD_State |= 0xFF;
}


//LCD ��ʾ
void LCD_function(void)
{
    uint8_t i;

    for(i = 0; i < 10; i++)
    {
        if(LCD_State & (0x01 << i))
        {
            Arr_KG(LCD_Line_BUFF, i);	//����ո�
            LCD_State &= (~(1 << i));
            switch(Data.High_Line_Flag)
            {
            case 1:
            {
                LCD_SetBackColor(Blue);
                LCD_DisplayStringLine(Line3, LCD_Line_BUFF[3]);
                LCD_SetBackColor(Black);
                LCD_DisplayStringLine(Line4, LCD_Line_BUFF[4]);
                LCD_DisplayStringLine(Line5, LCD_Line_BUFF[5]);
                LCD_DisplayStringLine(Line6, LCD_Line_BUFF[6]);
                break;
            }
            case 2:
            {
                LCD_SetBackColor(Blue);
                LCD_DisplayStringLine(Line4, LCD_Line_BUFF[4]);
                LCD_SetBackColor(Black);
                LCD_DisplayStringLine(Line3, LCD_Line_BUFF[3]);
                LCD_DisplayStringLine(Line5, LCD_Line_BUFF[5]);
                LCD_DisplayStringLine(Line6, LCD_Line_BUFF[6]);
                break;
            }
            case 3:
            {
                LCD_SetBackColor(Blue);
                LCD_DisplayStringLine(Line5, LCD_Line_BUFF[5]);
                LCD_SetBackColor(Black);
                LCD_DisplayStringLine(Line4, LCD_Line_BUFF[4]);
                LCD_DisplayStringLine(Line3, LCD_Line_BUFF[3]);
                LCD_DisplayStringLine(Line6, LCD_Line_BUFF[6]);
                break;
            }
            case 4:
            {
                LCD_SetBackColor(Blue);
                LCD_DisplayStringLine(Line6, LCD_Line_BUFF[6]);
                LCD_SetBackColor(Black);
                LCD_DisplayStringLine(Line5, LCD_Line_BUFF[5]);
                LCD_DisplayStringLine(Line4, LCD_Line_BUFF[4]);
                LCD_DisplayStringLine(Line3, LCD_Line_BUFF[3]);
                break;
            }
            default:
            {
                LCD_SetBackColor(Black);
                LCD_DisplayStringLine(i * 24, LCD_Line_BUFF[i]);
                break;
            }
            }
        }
    }
    if(!Data.One_Flag)	//�״��ϵ�ˢ��һ��
    {
        Data.One_Flag = 1;
        LCD_Dis1();	//��ʾҳ��1
    }
    if(!Data.MENU_State)	//��ҳ�����ʾ��ѹ��״̬ ����ҳ�治��ʾ
    {
        sprintf((char *)LCD_Line_BUFF[C3_State], "    Volt:%.2fV", parameter_Data.Now_V);	//��ѹ
        sprintf((char *)LCD_Line_BUFF[C4_State], "    Status:");	//״̬
        LCD_State |= (0x01 << C3_State);
        LCD_State |= (0x01 << C4_State);
    }
}

//����ո�
void Arr_KG(uint8_t arr[][20], uint8_t NUM)
{
    for(uint8_t j = 0; j < 20; j++)
    {
        if(arr[NUM][j] == '\0')
            arr[NUM][j] = ' ';
    }
}




//��ʼ��
void TASK_Init(void)
{
    LCD_Init();
    I2CInit();
    LED_DIS(0xFF, RESET);
    HAL_TIM_Base_Start_IT(&htim6);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);	//ADCУ��
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    Data.MENU_State = 0;	//Ĭ����0
    Parameter_Read(&parameter_Data);	//�ϵ��ȡ����
# if 0
    parameter_Data.Now_V = 2.04;
    parameter_Data.V_UP  = 2.1;
    parameter_Data.V_DOWN = 0.4;
    parameter_Data.UP_LED = 1;
    parameter_Data.DOWN_LED = 3;
    Parameter_Write(&parameter_Data);
#endif
}



//��ʱ���ص�
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint16_t Key_timer_count;
    static uint16_t LED_count;
    if(htim == &htim6)
    {
        Key_timer_count++;
        if(task_timer.KEY_TIME_COUNT)
            task_timer.KEY_TIME_COUNT--;
        if(task_timer.ADC_TIME_COUNT)
            task_timer.ADC_TIME_COUNT--;

        if(100 == Key_timer_count)
        {
            Key_timer_count = 0;
            KEY_TIMER_COUNT++;
        }
        if(Data.UP_LED_Flag || Data.DOWN_LED_Flag)
        {
            LED_count++;
            if(200 == LED_count)	//200ms
            {
                LED_count = 0;
                Data.LED_SCAN_Flag = 1;
            }
        }
    }
}
