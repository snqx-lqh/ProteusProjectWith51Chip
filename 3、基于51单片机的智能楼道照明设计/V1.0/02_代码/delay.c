#include "delay.h"

void Delay1ms(void)	//@12.000MHz
{
	unsigned char data i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}

void delay_ms(uint16_t xms)
{
	while(xms--)
	{
		Delay1ms();
	}
}