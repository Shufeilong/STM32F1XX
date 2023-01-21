#include "exti.h"
#include "sys.h"
#include "key.h"
void Exti_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//�����ȳ�ʼ��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_0;//PA0
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//����   50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruct);
    
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;//PE3  PE4   PE2
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//����   50MHz
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);//PA0  ---��EXTI0
	
	EXTI_InitStruct.EXTI_Line    =  EXTI_Line0;//PA0  ---��EXTI0
	EXTI_InitStruct.EXTI_LineCmd =  ENABLE;//ʹ��
	EXTI_InitStruct.EXTI_Mode    =  EXTI_Mode_Interrupt;//�ж�
	EXTI_InitStruct.EXTI_Trigger =  EXTI_Trigger_Falling;//�½��ش���
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//��ռ���ȼ�   0-3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0x3;//��Ӧ���ȼ�           0-3
	NVIC_Init(&NVIC_InitStruct); 	
}

//�жϷ�����     ��startup_stm32f40_41xxx.s ������
void EXTI0_IRQHandler(void)
{
	//�ж�ȷʵ���ж�
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		//delay_xd(5);
		//��������ж���
		EXTI_ClearITPendingBit(EXTI_Line0);
		PFout(9)=!PFout(9);
	
	}
}
