#include "allhead.h"

//��ǰ��ַ��ȡ �������洢���յ�����
void eeprom_read_current(uint8_t *dat)
{
	I2CStart();
	I2CSendByte(0xA1);	//����������ַ(������)
	I2CWaitAck();
	*dat = I2CReceiveByte();	//��ȡ����
	I2CSendNotAck();
	I2CStop();
}


//�����ַ��ȡ ��������Ҫ��ȡ�ĵ�ַ
uint8_t eeprom_read_random(uint8_t addr)
{
	uint8_t temp;	//�������ݵ���ʱ����
	
	I2CStart();
	I2CSendByte(0xA0);	//����������ַ(д����)
	I2CWaitAck();
	
	I2CSendByte(addr);	//д����Ҫ��ȡ�������ڵĵ�ַ
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0xA1);	//����������ַ(������)
	I2CWaitAck();
	
	temp = I2CReceiveByte();	//��ȡ����
	I2CSendNotAck();
	I2CStop();
	
	return temp;	//��������
}

//˳���ַ��ȡ //����1�����ȡ�����ݵ�ַ ����2����Ҫ�������ݵ����� ����3��Ҫ�����ٸ�����(һ����ַһ������)
void eeprom_read_sequential(uint8_t addr,uint8_t *dat,uint8_t len)
{
	I2CStart();
	I2CSendByte(0xA0);	//����������ַ(д����)
	I2CWaitAck();
	
	I2CSendByte(addr);	//д����Ҫ��ȡ�������ڵĵ�ַ
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0xA1);
	I2CWaitAck();
	
	while(len--)
	{
		*dat++ = I2CReceiveByte();	//��ȡ���ݵ�����
		
		if(len)
		{
			I2CSendAck();
		}
		else
		{
			I2CSendNotAck();
		}
	}
	I2CStop();
}

//�ֽ�д�� ����1����Ҫд������  ����2����Ҫд��ĵ�ַ
void eeprom_write_byte(uint8_t dat,uint8_t addr)
{
	I2CStart();
	I2CSendByte(0xA0);	//����������ַ(д����)
	I2CWaitAck();
	
	I2CSendByte(addr);	//����д��ĵ�ַ
	I2CWaitAck();
	I2CSendByte(dat);	//��������
	I2CWaitAck();
	I2CStop();
}

//ҳд�� ����1����Ҫд��ĵ�ַ ����2����Ҫд�������(����) ����3�����ݵĳ���
void eeprom_write_page(uint8_t addr,uint8_t *dat,uint8_t len)
{
	uint8_t temp = addr;
	
	I2CStart();
	I2CSendByte(0xA0);	//����������ַ(д����)
	I2CWaitAck();
	
	I2CSendByte(addr);	//����д��ĵ�ַ
	I2CWaitAck();
	while(len--)
	{
		I2CSendByte(*dat++);	//��������
		I2CWaitAck();
		temp ++;                    //E2PROM��ַ����
		if((temp & 0x07) == 0)	//����ַ�Ƿ񵽴�ҳ�߽磬24C02ÿҳ8�ֽ�
		{
			break;	//����ҳ�߽�ʱ������ѭ������������д����
		}
					
	}
	I2CStop();
}

//д��uint16���� ����1������Ҫд��ĵ�ַ ����2����ֵ0~65535
void eeprom_write_u16(uint8_t addr,uint16_t dat)
{
    uint8_t Hn,Ln;	//��λ����λ
    Hn = (dat>>8)&0xFF;	//��ȡ��λ
    Ln = dat&0xFF;	//��ȡ��8λ
    eeprom_write_byte(Ln,addr);	//���͵�λ
    HAL_Delay(10);
    eeprom_write_byte(Hn,addr+1);	//���͸�λ
    HAL_Delay(10);
}

//��ȡuint16���� ���������ݵĵ�ַ
uint16_t eeprom_read_u16(uint8_t addr)
{
    uint16_t tmp;
    tmp = (uint16_t)eeprom_read_random(addr);	
    HAL_Delay(10);
    tmp |= (uint16_t)(eeprom_read_random(addr+1)<<8);	//�ٶ�ȡ��λ��|���Ǻϲ�Ҳ������+
    HAL_Delay(10);

    return tmp;
}

//MCP4017д����
void mcp_write_byte(uint8_t value)
{
	I2CStart();
	I2CSendByte(0x5E);	//д����
	I2CWaitAck();
	
	I2CSendByte(value);	//��������
	I2CWaitAck();
	I2CStop();
}

//MCP4017������
uint8_t mcp_read_byte(void)
{
	uint8_t temp;
	
	I2CStart();
	I2CSendByte(0x5F);	//������
	I2CWaitAck();
	
	temp = I2CReceiveByte();	//��ȡ����
	I2CSendNotAck();
	I2CStop();
	
	return temp;	
}


