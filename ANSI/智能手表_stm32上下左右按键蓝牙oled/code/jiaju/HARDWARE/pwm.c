#include "pwm.h"

void Pwm_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef       TIM_OCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);//42MHz
	
	GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_9 | GPIO_Pin_8;//PF9   PF10
	GPIO_InitStruct.GPIO_Mode =    GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd =    GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//此时不做GPIO，做什么事情给指定好
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); 
	
	TIM_TimeBaseInitStruct.TIM_Period =  100-1;//重装载值  65535   我给5000就是500ms  
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//预分频器 1-65535  84MHz/8400=10000Hz--->1s
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频了
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;//模式1
	TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_Low;//低电平有效
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//使能
	TIM_OC1Init(TIM14, &TIM_OCInitStruct);
	//使能预装载寄存器
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
    //使能自动重装载的预装载寄存器允许位		
	TIM_ARRPreloadConfig(TIM14,ENABLE);
	
	TIM_Cmd(TIM14,ENABLE);//使能定时器
}

