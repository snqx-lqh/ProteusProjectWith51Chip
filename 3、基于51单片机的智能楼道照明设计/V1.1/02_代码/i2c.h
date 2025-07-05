#ifndef _I2C_H
#define _I2C_H

#include "main.h"

sbit SCL = P1^0;			//定义SCL时钟线端口
sbit SDA = P1^1;			//定义SDA数据线端口

//IIC初始化
void IIC_init(void);
//IIC写一个字节数据到设备指定寄存器
void IIC_Write_Dev_One_Byte(uint8_t addr,uint8_t reg,uint8_t write_data);
//IIC读一个字节数据到设备指定寄存器
uint8_t IIC_Read_Dev_One_Byte(uint8_t addr,uint8_t reg);

#endif
