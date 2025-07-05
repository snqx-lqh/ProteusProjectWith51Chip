#ifndef _LCD1602_H
#define _LCD1602_H

#include <reg52.h>

/**********************************
PIN�ڶ���
**********************************/
#define LCD1602_DATAPINS  P0      //data bus ��������
sbit LCD1602_RS = P2^5;
sbit LCD1602_RW = P2^6;
sbit LCD1602_E  = P2^7;	 

/**********************************
��������
**********************************/
void Lcd1602_Delay1ms(unsigned int c);   
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);     //��ʾ�ַ���
void LcdInit();                                                          //1602��ʼ��

#endif

