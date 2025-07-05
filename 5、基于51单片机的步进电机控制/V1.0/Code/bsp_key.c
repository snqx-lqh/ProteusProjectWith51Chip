#include "bsp_key.h"

void Delay10ms()		 
{
	unsigned char data i, j;

	i = 18;
	j = 235;
	do
	{
		while (--j);
	} while (--i);
}

void KeyInit()
{
	key1 = 1;
	key2 = 1;
	key3 = 1;
	key4 = 1;
}

/**
 * @brief ����ɨ�躯��
 * 
 * @param mode ģʽΪ1��������ɨ�裬Ϊ0���ǵ���
 */
u8 KeyScan(u8 mode)
{
    static int    keyState = 0;
    if(mode == 1) keyState=0;
    if (keyState == 0 && (key1 == 0||key2 == 0||key3 == 0||key4 == 0))
    {    
		Delay10ms();
		keyState = 1;
		if      (key1 == 0) return 1;
		else if (key2 == 0) return 2;
		else if (key3 == 0) return 3;
		else if (key4 == 0) return 4;
    }else if (key1 == 1 && key2 == 1 && key3 == 1 && key4 == 1)
    {
        keyState = 0;
    }
	return 0;
}