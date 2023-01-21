#include "delay.h"

void delay_us(unsigned int nus)
{
	SysTick->LOAD = (168000000/8/1000000)*nus;//��װ��ֵ
	SysTick->CTRL |= 1;//ʹ��CTRL��ʼ����
	SysTick->VAL = 0;
	while((SysTick->CTRL & (1<<16))==0);
	SysTick->CTRL &= ~1;//ʧ��CTRL��ʼ����
	SysTick->VAL = 0;
}
//����ܰ�����ʱ 798.9 ms
void delay_ms(unsigned int nms)
{
	SysTick->LOAD = (168000000/8/1000)*nms;//��װ��ֵ
	SysTick->CTRL |= 1;//ʹ��CTRL��ʼ����
	SysTick->VAL = 0;
	while((SysTick->CTRL & (1<<16))==0);
	SysTick->CTRL &= ~1;//ʧ��CTRL��ʼ����
	SysTick->VAL = 0;
}

void delay_s(unsigned int ns)
{
	while(ns--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}
