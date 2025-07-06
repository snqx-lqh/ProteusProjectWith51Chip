#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "main.h"

typedef enum{
	Timer0 = 0,
	Timer1
}Timer_t;

void bsp_timer_set_us_IT(Timer_t Timer, uint16_t xus);
void bsp_timer_set_ms_IT(Timer_t Timer, uint16_t xms);

void timer_reload_us(Timer_t Timer, uint16_t xus);
void timer_reload_ms(Timer_t Timer, uint16_t xms);

#endif
