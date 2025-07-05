#include "i2c.h"
#include <intrins.h>

void Delay()
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

//IIC��ʼ������
void IIC_init()
{
	SCL = 1;
	Delay();
	SDA = 1;
	Delay();
}
 
//IIC��ʼ�źź���
void IIC_start()
{
	//��SCL�ź�Ϊ�ߵ�ƽ�ڼ䣬SDA�����½���ʱ��������ʼ�ź�
	SDA = 1;
	Delay();
	SCL = 1;
	Delay();
	SDA = 0;
	Delay();
}
 
//IICӦ���źź���
void IIC_respons()
{
	//��SCL�ߵ�ƽ�ڼ䣬��SDA�����ͣ�����Ӧ���źţ��������ݴ���ɹ�
	uint8_t i = 0;
	SCL = 1;
	Delay();
	while(SDA == 1 && (i < 255))
	{
		i ++;
	}
	SCL = 0;
	Delay();
}
 
//����ֹͣ�źź���
void IIC_stop()
{
	//��SCL�ڸߵ�ƽʱ��SDA����һ���������ź�Ϊֹͣ�ź�
	SDA = 0;
	Delay();
	SCL = 1;
	Delay();
	SDA = 1;
	Delay();
}
 
//дһ���ֽں���
void IIC_writebyte(uint8_t date)
{
	uint8_t i,temp;
	temp = date;
	for(i = 0;i < 8;i++)
	{
		temp = temp << 1;	//�����һλ�Զ����浽�Ĵ���CY��
		SCL = 0;
		Delay();
		SDA = CY;
		Delay();
		SCL = 1;
		Delay();
	}
	SCL = 0;
	Delay();
	SCL = 1;
	Delay();
}
 
//IIC��һ���ֽں���
uint8_t IIC_readbyte()
{
	uint8_t i,Data;
	SCL = 0;
	Delay();
	SDA = 1;
	for(i = 0;i < 8;i++)
	{
		SCL = 1;
		Delay();
		SDA = 1;
		Data = (Data << 1) | SDA;
		SCL = 0;
		Delay();
	}
	Delay();
	return Data;
}
 
 
void IIC_Write_Dev_One_Byte(uint8_t addr,uint8_t reg,uint8_t write_data)
{
	IIC_start();
	IIC_writebyte((addr<<1)|0);
	IIC_respons();
	IIC_writebyte(reg);
	IIC_respons();
	IIC_writebyte(write_data);
	IIC_respons();
	IIC_stop();
}
 
 
uint8_t IIC_Read_Dev_One_Byte(uint8_t addr,uint8_t reg)
{
	uint8_t date;
	IIC_start();
	IIC_writebyte((addr<<1)|0);		//AT24C02�ĵ�ַΪ0xa0ʱ��д��
	IIC_respons();
	IIC_writebyte(reg);
	IIC_respons();
	IIC_start();
	IIC_writebyte((addr<<1)|1);		//AT24C02�ĵ�ַΪ0xa1ʱ�Ƕ�
	IIC_respons();
	date = IIC_readbyte();	//�����ݶ�ȡ��date��
	IIC_stop();							//ֹͣ�ź�
	return date;
}