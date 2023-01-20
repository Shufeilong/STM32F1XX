#include "tim.h"
#include "sys.h"
#include "OLED_I2C.h"
#include "show.h"
void Tim_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//��ʼ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//APB1  42MHz  
	
	TIM_TimeBaseInitStruct.TIM_Period =  10000-1;//��װ��ֵ  65535   �Ҹ�5000����500ms  
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//Ԥ��Ƶ�� 1-65535  84MHz/8400=10000Hz--->1s
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ��
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;//TIM4
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//��ռ���ȼ�   0-3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0x3;//��Ӧ���ȼ�           0-3
	NVIC_Init(&NVIC_InitStruct); 
	//��ʱ��4�ж�ʹ��
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
	//ʹ�ܶ�ʱ��4.��ʼ����
	TIM_Cmd(TIM4,ENABLE);
}
	RTC_TimeTypeDef RTC_TimeStruct;

//�жϷ�����     ��startup_stm32f40_41xxx.s ������
void TIM4_IRQHandler(void)
{
	//�ж�ȷʵ���ж�
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		//��������ж���
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
		if(RTC_TimeStruct.RTC_Seconds==0)
		{
			
		}
		
	}
}
