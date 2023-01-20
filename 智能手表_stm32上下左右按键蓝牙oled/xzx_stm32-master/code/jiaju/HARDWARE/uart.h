#ifndef __UART_H__
#define __UART_H__

#include "stm32f4xx.h"

void Usart_Init(unsigned band,char U_num);
void Usart_text(void);
char Uart_control_text(void);
#endif
