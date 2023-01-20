#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f4xx.h"//#include "stm32f10x.h"

void Dht11_Init(void);
void change_mode(GPIOMode_TypeDef mode);
int start_dht11(void);
void get_8bit(void);
int get_dht11_data(void);
void show_Dht(void);
#endif
