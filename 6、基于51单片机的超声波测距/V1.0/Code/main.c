#include "reg52.h"

#include "stdio.h"
#include "hc_sr04.h"
#include "LCD1602.h"
#include "stdio.h"

#define u8 unsigned char
#define u16 unsigned int
			   
void delayms(unsigned int ms)
{
	unsigned char i=100,j;
	for(;ms;ms--)
	{
		while(--i)
		{
			j=10;
			while(--j);
		}
	}
}

void main()
{
	float dist_value = 0;
	u8 lcdShowBuff[16] = {0};
	
	HCSR04_Init();        // HCSR04的引脚初始化
	HCSR04_Timer0Init();  // HCSR04的定时器计数初始化
	LcdInit();            // LCD显示初始化
	 
	while(1)
	{
		HCSR04_trigSendOnePulse();
		dist_value = DistCount();
		 
		sprintf((char*)lcdShowBuff,"distanc : %f    ",dist_value);
		LcdShowStr(0,0,lcdShowBuff);
		
		delayms(100);
	}
}




