#include "tim.h"
#include "sys.h"
#include "OLED_I2C.h"
#include "show.h"
void Tim_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//初始化时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//APB1  42MHz  
	
	TIM_TimeBaseInitStruct.TIM_Period =  10000-1;//重装载值  65535   我给5000就是500ms  
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//预分频器 1-65535  84MHz/8400=10000Hz--->1s
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频了
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;//TIM4
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//抢占优先级   0-3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0x3;//响应优先级           0-3
	NVIC_Init(&NVIC_InitStruct); 
	//定时器4中断使能
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
	//使能定时器4.开始计数
	TIM_Cmd(TIM4,ENABLE);
}
	RTC_TimeTypeDef RTC_TimeStruct;

//中断服务函数     在startup_stm32f40_41xxx.s 里面找
void TIM4_IRQHandler(void)
{
	//判断确实进中断
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		//立马清楚中断线
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
		if(RTC_TimeStruct.RTC_Seconds==0)
		{
			
		}
		
	}
}
