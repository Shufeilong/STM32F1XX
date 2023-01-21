#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f4xx.h"

void delay_us(unsigned int nus);
void delay_ms(unsigned int nms);
void delay_s(unsigned int ns);
#endif
