#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "main.h"

void Uart1_Init(void);
void UartSendBuffLen(uint8_t *str,uint8_t len);

#endif
