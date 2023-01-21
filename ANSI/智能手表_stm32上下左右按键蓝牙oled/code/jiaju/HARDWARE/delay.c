#include "delay.h"

void delay_us(unsigned int nus)
{
	SysTick->LOAD = (168000000/8/1000000)*nus;//重装载值
	SysTick->CTRL |= 1;//使能CTRL开始计数
	SysTick->VAL = 0;
	while((SysTick->CTRL & (1<<16))==0);
	SysTick->CTRL &= ~1;//失能CTRL开始计数
	SysTick->VAL = 0;
}
//最多能帮你延时 798.9 ms
void delay_ms(unsigned int nms)
{
	SysTick->LOAD = (168000000/8/1000)*nms;//重装载值
	SysTick->CTRL |= 1;//使能CTRL开始计数
	SysTick->VAL = 0;
	while((SysTick->CTRL & (1<<16))==0);
	SysTick->CTRL &= ~1;//失能CTRL开始计数
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
