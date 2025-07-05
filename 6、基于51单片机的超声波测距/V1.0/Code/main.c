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
	
	HCSR04_Init();        // HCSR04�����ų�ʼ��
	HCSR04_Timer0Init();  // HCSR04�Ķ�ʱ��������ʼ��
	LcdInit();            // LCD��ʾ��ʼ��
	 
	while(1)
	{
		HCSR04_trigSendOnePulse();
		dist_value = DistCount();
		 
		sprintf((char*)lcdShowBuff,"distanc : %f    ",dist_value);
		LcdShowStr(0,0,lcdShowBuff);
		
		delayms(100);
	}
}




