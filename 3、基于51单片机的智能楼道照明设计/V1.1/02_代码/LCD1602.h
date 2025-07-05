#ifndef _LCD1602_H
#define _LCD1602_H

#include <reg52.h>

#ifndef uint8_t
#define uint8_t  unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned int
#endif

/**********************************
PIN口定义
**********************************/
#define LCD1602_DATAPINS  P0      //data bus 数据总线
sbit LCD1602_RS = P2^6;
sbit LCD1602_RW = P2^5;
sbit LCD1602_E  = P2^7;	 

/**********************************
函数声明
**********************************/
void Lcd1602_Delay1ms(uint16_t c);   
void LcdShowStr(uint8_t x,uint8_t y,uint8_t *str);     //显示字符串
void LcdShowNumInt(uint8_t x,uint8_t y,uint16_t num,uint8_t len,uint8_t mode);
void LcdShowNumFloat(uint8_t x,uint8_t y,float num,uint8_t intLen,uint8_t floatLen,uint8_t mode);
void LcdInit();                         //1602初始化

#endif

