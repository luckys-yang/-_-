#include "allhead.h"

char USART1_RX_TEMP[USART1_RX_LEN] = "\0";	//��Ž���������ʱ����
char USART1_RX_BUF[USART1_RX_LEN];	//��Ž������ݵ���������
char USART1_NewData;	//���½��յ��ֽ�
uint8_t USART1_Count = 0;	//����ֵ
uint8_t RX_flag;


RX_Data d1;



//�ض����ͺ���
void PrintTest(void)
{
	HAL_Delay(1000);
	printf("����\r\n");
	HAL_Delay(1000);
}

//HAL�ⷢ�ͺ���
void TXTest(void)
{
	uint8_t arr[] = "��������\r\n";
	HAL_UART_Transmit(&huart1,(uint8_t*)&arr,strlen((char*)arr),0xFFFF);
}



//�ض���printf����
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,0xFFFF);
    return ch;
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(&huart1 == huart)	//����Ǵ���1
//	{	
//		if('\n' == USART1_NewData)	//��������
//		{
//			USART1_RX_TEMP[USART1_Count-1] = '\0';	//��'\n'ǰ���'\r'ȥ��
//			sscanf(USART1_RX_TEMP,"%3s:%d:%d",d1.c1,&d1.x,&d1.y);
//			strcpy(USART1_RX_BUF,USART1_RX_TEMP);	//��������
//			RX_flag = 1;
//			USART1_Count = 0;
//			memset(USART1_RX_BUF,0,sizeof(USART1_RX_BUF));	//��ʼ��Ϊ0
//			memset(USART1_RX_TEMP,0,sizeof(USART1_RX_TEMP));	//��ʼ��Ϊ0
//		}
//		else
//		{
//			USART1_RX_TEMP[USART1_Count++] = USART1_NewData;			
//		}

//		HAL_UART_Receive_IT(&huart1,(uint8_t*)&USART1_NewData,1);
//	}
//}
