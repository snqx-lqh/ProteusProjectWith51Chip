#ifndef _DHT11_H
#define _DHT11_H

#include "main.h"

sbit DHT11_PIN = P1^7; 

uint8_t DHT11_Get_Data(uint8_t *hum,uint8_t *temp);



#endif

