#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//必须先初始化时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_8;//PF9   PF10
	GPIO_InitStruct.GPIO_Mode =    GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd =    GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOF,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_13 | GPIO_Pin_14;//PE13  PE14
	GPIO_InitStruct.GPIO_Mode =    GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd =    GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//在开灯之前，先把灯关了
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_13 | GPIO_Pin_14);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
}
