#ifndef _LCD1602_H
#define _LCD1602_H

#include "main.h"

#define LCD1602_DATAPINS  P0      
sbit LCD1602_RS = P2^6;
sbit LCD1602_RW = P2^5;
sbit LCD1602_E  = P2^7;	 

void Lcd1602_Delay1ms(uint16_t c);   
void LcdShowStr(uint8_t x,uint8_t y,uint8_t *str);     //显示字符串
void LcdInit();                                                          //1602初始化

#endif

