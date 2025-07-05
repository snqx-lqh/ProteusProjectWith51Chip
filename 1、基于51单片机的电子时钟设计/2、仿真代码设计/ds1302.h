#ifndef _DS1302_H_
#define _DS1302_H_

#include <reg52.h>
#include <intrins.h>
#include "main.h"

sbit IO =P2^5;
sbit SCK=P2^4;
sbit RST=P2^3;

void   ds1302write(u8 addre,u8 dat);
u8     ds1302read(u8 addre);
void   ds1302init();
void   ds1302readtime();
void   ds1302writetime();
extern u8 time[7];
extern u8 second,minute,hour,day,month,week,year;
extern u8 secondTemp,minuteTemp,hourTemp,dayTemp,monthTemp,weekTemp,yearTemp;
#endif