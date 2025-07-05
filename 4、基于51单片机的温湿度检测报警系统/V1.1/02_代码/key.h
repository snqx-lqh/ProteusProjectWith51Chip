#ifndef _KEY_H
#define _KEY_H

#include "main.h"

#define KEY0_PRES 1
#define KEY1_PRES 2
#define KEY2_PRES 3
#define KEY3_PRES 4

sbit KEY0 = P3^2;
sbit KEY1 = P3^3;
sbit KEY2 = P3^4;
sbit KEY3 = P3^5;

uint8_t KeyScan();

#endif
