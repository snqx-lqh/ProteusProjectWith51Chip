#include "pcf8591.h"

/********************User modification area begin********************/
//只需要将自己的IIC处理程序替换进去即可
#include "soft_i2c.h"
#include "i2c.h"
/**
  * @brief   pcf8591的写入一个字节
  * @param   addr 设备地址   reg 寄存器地址   write_data 写入的数据
  * @retval  0 成功 1 失败
 **/
int pcf8591_write_one_byte(uint8_t addr,uint8_t reg,uint8_t write_data)
{
	IIC_Write_Dev_One_Byte(addr,reg,write_data);
	return 0;
}

/**
  * @brief   pcf8591的读取一个字节数据
  * @param   addr 设备地址   reg 寄存器地址   read_data 读取的数据
  * @retval  0 成功 1 失败
 **/
int pcf8591_read_one_byte(uint8_t addr,uint8_t reg,uint8_t *read_data)
{
	*read_data = IIC_Read_Dev_One_Byte(addr,reg);
	return 0;
}
/*********************User modification area end**************/

#define pcfAddress 0x48

/**
  * @brief   pcf8591的DAC引脚输出电压，0-255分别对应0-提供的电压
  * @param   value：0-255 
  * @retval   
 **/
void pcf_analog_write (int value)
{
	pcf8591_write_one_byte(pcfAddress,0x40,value);
}

/**
  * @brief   pcf8591的ADC引脚读电压
  * @param   pinReg：引脚0-3
  * @retval  0 成功 1 失败
 **/
uint8_t pcf_analog_read (int pinReg)
{
	uint8_t adcx ; 

	pcf8591_read_one_byte(pcfAddress, 0x40|pinReg, &adcx);

	return adcx ;
}
