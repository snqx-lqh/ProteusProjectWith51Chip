#include "delay.h"

 
// 不是很准的延时
void delay_ms(uint16_t xms)
{
   uint16_t i,j;
   for(i=xms;i>0;i--)
      for(j=110;j>0;j--);
}

