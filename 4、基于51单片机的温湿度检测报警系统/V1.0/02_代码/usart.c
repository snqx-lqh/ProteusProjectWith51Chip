#include "usart.h"

void UartInit(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES = 1;             //打开串口中断
}

void SendBuffLen(uint8_t *str,uint8_t len)
{
	while(len != 0)
	{
		SBUF = *str;
		while(!TI);
		TI = 0;
		str++;
		len --;
	}
}

void UartRoutine(void) interrupt 4
{
	uint8_t res;	
	if(RI)
	{
		res = SBUF;
		RI = 0;
	}	
}