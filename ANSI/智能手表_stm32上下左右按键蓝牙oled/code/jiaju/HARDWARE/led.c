#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//�����ȳ�ʼ��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_8;//PF9   PF10
	GPIO_InitStruct.GPIO_Mode =    GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd =    GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//����   50MHz
	GPIO_Init(GPIOF,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_13 | GPIO_Pin_14;//PE13  PE14
	GPIO_InitStruct.GPIO_Mode =    GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd =    GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//����   50MHz
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//�ڿ���֮ǰ���Ȱѵƹ���
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_13 | GPIO_Pin_14);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
}
