#ifndef __RTC_H__
#define __RTC_H__

#include "stm32f4xx.h"
void RTC_set(void);
void Rtc_Init(void);
void RTC_text(void);
int num_get(void);
void ALarm_set(void);
void flash_font(char x,char y,char str[],char Compulsory);
void Time_judge(u8 *Year,char *mouth,char *day);
void Date_judge(unsigned char *hour,unsigned char *minu,unsigned char *seco);
#endif
