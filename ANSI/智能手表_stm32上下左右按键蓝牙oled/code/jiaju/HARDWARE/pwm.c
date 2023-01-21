#include "pwm.h"

void Pwm_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef       TIM_OCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);//42MHz
	
	GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_9 | GPIO_Pin_8;//PF9   PF10
	GPIO_InitStruct.GPIO_Mode =    GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd =    GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//����   50MHz
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//��ʱ����GPIO����ʲô�����ָ����
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); 
	
	TIM_TimeBaseInitStruct.TIM_Period =  100-1;//��װ��ֵ  65535   �Ҹ�5000����500ms  
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//Ԥ��Ƶ�� 1-65535  84MHz/8400=10000Hz--->1s
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ��
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;//ģʽ1
	TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_Low;//�͵�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//ʹ��
	TIM_OC1Init(TIM14, &TIM_OCInitStruct);
	//ʹ��Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
    //ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ		
	TIM_ARRPreloadConfig(TIM14,ENABLE);
	
	TIM_Cmd(TIM14,ENABLE);//ʹ�ܶ�ʱ��
}

