#include "pcf8591.h"

/********************User modification area begin********************/
//ֻ��Ҫ���Լ���IIC��������滻��ȥ����
#include "soft_i2c.h"
#include "i2c.h"
/**
  * @brief   pcf8591��д��һ���ֽ�
  * @param   addr �豸��ַ   reg �Ĵ�����ַ   write_data д�������
  * @retval  0 �ɹ� 1 ʧ��
 **/
int pcf8591_write_one_byte(uint8_t addr,uint8_t reg,uint8_t write_data)
{
	IIC_Write_Dev_One_Byte(addr,reg,write_data);
	return 0;
}

/**
  * @brief   pcf8591�Ķ�ȡһ���ֽ�����
  * @param   addr �豸��ַ   reg �Ĵ�����ַ   read_data ��ȡ������
  * @retval  0 �ɹ� 1 ʧ��
 **/
int pcf8591_read_one_byte(uint8_t addr,uint8_t reg,uint8_t *read_data)
{
	*read_data = IIC_Read_Dev_One_Byte(addr,reg);
	return 0;
}
/*********************User modification area end**************/

#define pcfAddress 0x48

/**
  * @brief   pcf8591��DAC���������ѹ��0-255�ֱ��Ӧ0-�ṩ�ĵ�ѹ
  * @param   value��0-255 
  * @retval   
 **/
void pcf_analog_write (int value)
{
	pcf8591_write_one_byte(pcfAddress,0x40,value);
}

/**
  * @brief   pcf8591��ADC���Ŷ���ѹ
  * @param   pinReg������0-3
  * @retval  0 �ɹ� 1 ʧ��
 **/
uint8_t pcf_analog_read (int pinReg)
{
	uint8_t adcx ; 

	pcf8591_read_one_byte(pcfAddress, 0x40|pinReg, &adcx);

	return adcx ;
}
