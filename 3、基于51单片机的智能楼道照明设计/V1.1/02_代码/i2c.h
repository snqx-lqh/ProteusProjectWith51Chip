#ifndef _I2C_H
#define _I2C_H

#include "main.h"

sbit SCL = P1^0;			//����SCLʱ���߶˿�
sbit SDA = P1^1;			//����SDA�����߶˿�

//IIC��ʼ��
void IIC_init(void);
//IICдһ���ֽ����ݵ��豸ָ���Ĵ���
void IIC_Write_Dev_One_Byte(uint8_t addr,uint8_t reg,uint8_t write_data);
//IIC��һ���ֽ����ݵ��豸ָ���Ĵ���
uint8_t IIC_Read_Dev_One_Byte(uint8_t addr,uint8_t reg);

#endif
