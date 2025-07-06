#ifndef _KEY_SCAN_H
#define _KEY_SCAN_H

#include "main.h"

sbit KEY1 = P1^0;
sbit KEY2 = P1^1;
sbit KEY3 = P1^2;
sbit KEY4 = P1^3;

#define KEY1_VALUE 1
#define KEY2_VALUE 2
#define KEY3_VALUE 3
#define KEY4_VALUE 4

// 按键初始化
void key_init(void);
// 按键扫描
uint8_t key_scan(uint8_t xms);

#endif

