#include "LCD1602.h"

/**
  * @name    Lcd1602_Delay1ms
  * @brief   ��ʱ��������ʱ1ms
  * @param   : [����/��] 
  * @retval  ����ֵ
  */
void Lcd1602_Delay1ms(unsigned int c)   //��� 0us
{
    unsigned char a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
    	
}
/**
  * @name    LcdWriteCom
  * @brief   ��LCDд��һ���ֽڵ�����
  * @param   com
  * @retval  ����ֵ
  */
void LcdWriteCom(unsigned char com)	  //д������
{
	Lcd1602_Delay1ms(1);
	LCD1602_E = 0;     //ʹ��
	LCD1602_RS = 0;	   //ѡ��������
	LCD1602_RW = 0;	   //ѡ��д��
	
	LCD1602_DATAPINS = com;     //��������
//	Lcd1602_Delay1ms(1);		//�ȴ������ȶ�

	LCD1602_E = 1;	          //д��ʱ��
//	Lcd1602_Delay1ms(5);	  //����ʱ��
	LCD1602_E = 0;
}
/**
  * @name    LcdWriteData
  * @brief   ��LCDд��һ���ֽڵ�����
  * @param   dat
  * @retval  ����ֵ
  */
void LcdWriteData(unsigned char dat)			//д������
{
	Lcd1602_Delay1ms(1);
	LCD1602_E = 0;	//ʹ������
	LCD1602_RS = 1;	//ѡ����������
	LCD1602_RW = 0;	//ѡ��д��

	LCD1602_DATAPINS = dat; //д������
//	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //д��ʱ��
//	Lcd1602_Delay1ms(5);   //����ʱ��
	LCD1602_E = 0;
}	
/**
  * @name    LcdInit
  * @brief   ��ʼ��LCD��
  * @param   ��
  * @retval  ����ֵ
  */
void LcdInit()						  //LCD��ʼ���ӳ���
{
 	LcdWriteCom(0x38);  //����ʾ
	LcdWriteCom(0x0c);  //����ʾ����ʾ���
	LcdWriteCom(0x06);  //дһ��ָ���1
	LcdWriteCom(0x01);  //����
	LcdWriteCom(0x80);  //��������ָ�����
}	
/**
  * @name    LcdSetCursor
  * @brief   ������ʾ RAM ��ʼ��ַ���༴���λ�ã�(x,y)-��Ӧ��Ļ�ϵ��ַ�����
  * @param   x y
  * @retval  ����ֵ
  */
void LcdSetCursor(unsigned char x, unsigned char y)
{
	unsigned char addr;

	if (y == 0) //���������Ļ���������ʾ RAM �ĵ�ַ
		addr = 0x00 + x; //��һ���ַ���ַ�� 0x00 ��ʼ
	else
		addr = 0x40 + x; //�ڶ����ַ���ַ�� 0x40 ��ʼ
	LcdWriteCom(addr | 0x80); //���� RAM ��ַ
}
/**
  * @name    LcdShowStr
  * @brief   ��Һ������ʾ�ַ�����(x,y)-��Ӧ��Ļ�ϵ���ʼ���꣬str-�ַ���ָ��
  * @param   x y str
  * @retval  ����ֵ
  */
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str)
{
	LcdSetCursor(x, y); //������ʼ��ַ
	while (*str != '\0') //����д���ַ������ݣ�ֱ����⵽������
	{
		LcdWriteData(*str++); //��ȡ str ָ������ݣ�Ȼ�� str �Լ� 1
	}
}
