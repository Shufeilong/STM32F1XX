#include "iwdg.h"

void Iwdg_Init(void)
{
	//去除写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//往里面写入0x5555
	
	//配置预分频器   原来使用的时钟是LSI 32KHz
	IWDG_SetPrescaler(IWDG_Prescaler_4);//32KHz/4=8KHz  8000--->1s 8--->1ms
	
	
	IWDG_SetReload(80);//喂狗时间 10ms   这个值是在0 - 4096
	
	IWDG_Enable();//启动独立看门狗      往里面写入0xCCCC
	
	IWDG_ReloadCounter();//使能重装载计数器  往里面写入0xAAAA
}
