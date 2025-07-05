#include "key.h"
#include "delay.h"

uint8_t KeyScan()
{	 
	static uint8_t key_up=1;//按键按松开标志
	static uint8_t delay_count = 0;
	if(key_up == 1 && (KEY0==0||KEY1==0))
	{
		delay_count ++;
		if(delay_count > 4)
		{
			delay_count = 0;
			key_up=0;
			if(KEY0==0)return KEY0_PRES;
			else if(KEY1==0)return KEY1_PRES;
		}
	}else if(KEY0==1&&KEY1==1)//按键全部抬起
	{
		key_up=1; 
	}	    
 	return 0;// 无按键按下
}
