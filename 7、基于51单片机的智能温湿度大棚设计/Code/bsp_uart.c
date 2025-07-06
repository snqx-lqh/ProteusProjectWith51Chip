#include "bsp_uart.h"

/**
  * @brief   9600bps@11.0592MHz
  * @param    
  * @retval  void
 **/
void Uart1_Init(void)	//9600bps@11.0592MHz
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


/**
  * @brief   发送指定长度的uint8_t的类型数据
  * @param   str 发送的数据  len 发送的数据长度
  * @retval  void
 **/
void UartSendBuffLen(uint8_t *str,uint8_t len)
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

/**
  * @brief   为了使用printf重定向 
  * @param   c 发送的字符
  * @retval  void
 **/
char putchar(char c)
{
	SBUF = c;
	while(!TI);
	TI = 0;
	return c;
}

//void UartRoutine(void) interrupt 4
//{
//	uint8_t res;	
//	if(RI)
//	{
//		res = SBUF;
//		RI = 0;
//	}	
//}


