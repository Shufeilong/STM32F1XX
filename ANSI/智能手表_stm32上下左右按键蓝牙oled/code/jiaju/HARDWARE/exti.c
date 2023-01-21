#include "exti.h"
#include "sys.h"
#include "key.h"
void Exti_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//必须先初始化时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_0;//PA0
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruct);
    
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;//PE3  PE4   PE2
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);//PA0  ---》EXTI0
	
	EXTI_InitStruct.EXTI_Line    =  EXTI_Line0;//PA0  ---》EXTI0
	EXTI_InitStruct.EXTI_LineCmd =  ENABLE;//使能
	EXTI_InitStruct.EXTI_Mode    =  EXTI_Mode_Interrupt;//中断
	EXTI_InitStruct.EXTI_Trigger =  EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//抢占优先级   0-3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0x3;//响应优先级           0-3
	NVIC_Init(&NVIC_InitStruct); 	
}

//中断服务函数     在startup_stm32f40_41xxx.s 里面找
void EXTI0_IRQHandler(void)
{
	//判断确实进中断
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		//delay_xd(5);
		//立马清楚中断线
		EXTI_ClearITPendingBit(EXTI_Line0);
		PFout(9)=!PFout(9);
	
	}
}
