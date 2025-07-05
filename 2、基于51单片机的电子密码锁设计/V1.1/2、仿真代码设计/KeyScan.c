#include "KeyScan.h"

/**
  * @name    KeyScan
  * @brief   ������⺯��
  * @param   : [����/��] 
  * @retval  ����ֵ
  */
u8 KeyScan(void)
{
    static u8 count = 0;
	u8 KeyState = 0; //1 Ϊ���� 0 Ϊ̧��
	u8 KeyValue = 0;
	
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f && KeyState == 0)
	{
        count ++;
		if(GPIO_KEY!=0x0f && count > 20 && KeyState == 0)
		{
            count = 0;
			KeyState = 1;
			//��
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=1;break;
				case(0X0b):	KeyValue=2;break;
				case(0X0d): KeyValue=3;break;
				case(0X0e):	KeyValue=4;break;
			}
			//��
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
			return KeyValue;
		}
	}else if(GPIO_KEY==0x0F && KeyState == 1)
    {
        KeyState = 0;
        KeyValue = 0;
    }
	return 0;
}
