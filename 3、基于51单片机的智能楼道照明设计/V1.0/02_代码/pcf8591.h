#ifndef _PCF8591_H
#define _PCF8591_H

#include "main.h"

void    pcf_analog_write (int value);
uint8_t pcf_analog_read (int pinReg);

#endif
