#ifndef __SHOW_H__
#define __SHOW_H__

#include "stm32f4xx.h"
#include "sys.h"
#include "rtc.h"

#define Time_time  0	//��ʾ�������ʱ��
#define Time_times 1	//��ʾ�����ʱ��
#define Time_week  2
#define Time_date  3

#define bluetooth 0
#define none 1
#define logo_overwatch 2
#define sunlight 3
#define temperature 4
#define hdmi 5
#define clock 6
#define up_arrow 7
#define down_arrow 8
#define select_arrow 9
#define timer 10
#define calendar 11
#define sleep 12

void show_Time(int x,int y,int Time_Class);	//ʱ����ʾ����
void main_screen();	//����Ļ��ʾ����
void select_mode();	//ѡ����溯��
#endif

