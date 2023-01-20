#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f4xx.h"
void delay_xd(unsigned int i);
void KEY_Init(void);
int Key_Scan(void);
char Button_check_all(void);
#endif
