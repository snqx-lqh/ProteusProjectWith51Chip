#ifndef _LCD1602_H
#define _LCD1602_H

#include <reg52.h>

/**********************************
PIN口定义
**********************************/
#define LCD1602_DATAPINS  P0      //data bus 数据总线
sbit LCD1602_RS = P2^5;
sbit LCD1602_RW = P2^6;
sbit LCD1602_E  = P2^7;	 

/**********************************
函数声明
**********************************/
void Lcd1602_Delay1ms(unsigned int c);   
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);     //显示字符串
void LcdInit();                                                          //1602初始化

#endif

