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

//IIC初始化函数
void IIC_init()
{
	SCL = 1;
	Delay();
	SDA = 1;
	Delay();
}
 
//IIC起始信号函数
void IIC_start()
{
	//当SCL信号为高电平期间，SDA出现下降沿时，就是起始信号
	SDA = 1;
	Delay();
	SCL = 1;
	Delay();
	SDA = 0;
	Delay();
}
 
//IIC应答信号函数
void IIC_respons()
{
	//在SCL高电平期间，讲SDA线拉低，产生应答信号，表明数据传输成功
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
 
//定义停止信号函数
void IIC_stop()
{
	//当SCL在高电平时，SDA产生一个上升沿信号为停止信号
	SDA = 0;
	Delay();
	SCL = 1;
	Delay();
	SDA = 1;
	Delay();
}
 
//写一个字节函数
void IIC_writebyte(uint8_t date)
{
	uint8_t i,temp;
	temp = date;
	for(i = 0;i < 8;i++)
	{
		temp = temp << 1;	//溢出的一位自动保存到寄存器CY中
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
 
//IIC读一个字节函数
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
	IIC_writebyte((addr<<1)|0);		//AT24C02的地址为0xa0时是写入
	IIC_respons();
	IIC_writebyte(reg);
	IIC_respons();
	IIC_start();
	IIC_writebyte((addr<<1)|1);		//AT24C02的地址为0xa1时是读
	IIC_respons();
	date = IIC_readbyte();	//将数据读取到date中
	IIC_stop();							//停止信号
	return date;
}