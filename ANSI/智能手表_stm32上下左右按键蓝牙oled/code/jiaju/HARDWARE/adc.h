#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f4xx.h"

void Adc_Init(void);
uint16_t get_value(void);
void show_adc(void);
#endif
