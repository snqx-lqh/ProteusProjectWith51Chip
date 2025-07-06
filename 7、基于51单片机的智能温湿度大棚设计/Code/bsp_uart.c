#include "bsp_uart.h"

/**
  * @brief   9600bps@11.0592MHz
  * @param    
  * @retval  void
 **/
void Uart1_Init(void)	//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x20;		//���ö�ʱ��ģʽ
	TL1 = 0xFD;			//���ö�ʱ��ʼֵ
	TH1 = 0xFD;			//���ö�ʱ����ֵ
	ET1 = 0;			//��ֹ��ʱ���ж�
	TR1 = 1;			//��ʱ��1��ʼ��ʱ
	ES = 1;             //�򿪴����ж�
}


/**
  * @brief   ����ָ�����ȵ�uint8_t����������
  * @param   str ���͵�����  len ���͵����ݳ���
  * @retval  void
 **/
void UartSendBuffLen(uint8_t *str,uint8_t len)
{
	while(len != 0)
	{
		SBUF = *str;
		while(!TI);
		TI = 0;
		str++;
		len --;
	}
}

/**
  * @brief   Ϊ��ʹ��printf�ض��� 
  * @param   c ���͵��ַ�
  * @retval  void
 **/
char putchar(char c)
{
	SBUF = c;
	while(!TI);
	TI = 0;
	return c;
}

//void UartRoutine(void) interrupt 4
//{
//	uint8_t res;	
//	if(RI)
//	{
//		res = SBUF;
//		RI = 0;
//	}	
//}


