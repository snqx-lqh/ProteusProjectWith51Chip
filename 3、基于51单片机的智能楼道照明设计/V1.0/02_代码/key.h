#ifndef _KEY_H
#define _KEY_H

#include "main.h"

#define KEY0_PRES 1
#define KEY1_PRES 2

sbit KEY0 = P3^2;
sbit KEY1 = P3^3;

uint8_t KeyScan();

#endif
