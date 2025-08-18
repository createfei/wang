#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

void USART_BT_Init(void);
void USART_BT_SendChar(uint8_t ch);

#endif