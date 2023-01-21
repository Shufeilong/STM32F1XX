#include "tim.h"
#include "sys.h"
#include "OLED_I2C.h"
#include "show.h"
#include "uart.h"
extern char Key;
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

void show_timer(){
	int i;
	int minu=0;
	int seco=0;
	int mill=0;
	char str[20];
	char x=24;
	int num_time=0;  //��ʼ��־λ
	OLED_CLS();
	while(1){
		show_Time(87,0,Time_time);
		Button_check_all();  //�жϰ�ť
		
		sprintf(str,"%02d",minu);
		OLED_ShowStr(x,3,str,2);
		OLED_ShowStr(x+16,3,":",2);
		sprintf(str,"%02d",seco);
		OLED_ShowStr(x+24,3,str,2);
		OLED_ShowStr(x+40,3,":",2);
		sprintf(str,"%02d",mill);
		OLED_ShowStr(x+48,3,str,2);
		PFout(9)=!PFout(9);
		if(num_time){
			delay_ms(5);
			mill+=13;
			if(mill>=100){
				seco+=1;
				mill=0;
				mill=0;
			}
			if(seco>=60){
				minu+=1;
				seco-=60;
			}
		}
		
		
		Usart_text();
		//delay_s(1);
		if(Key==4)
			break;
		else if(Key==1)
			num_time=1;
		else if(Key==2)
			num_time=0;
		else if(Key==3){
			num_time=0;
			minu=0;
			seco=0;
			mill=0;
		}
		//OLED_ShowStr(10,2,str,2);
	
	}
	OLED_CLS();
}

//�жϷ�����     ��startup_stm32f40_41xxx.s ������
void TIM4_IRQHandler(void)
{
	static char flag;
	//�ж�ȷʵ���ж�
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		//��������ж���
		
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
		if(RTC_TimeStruct.RTC_Minutes==0&&flag==0)
		{
			PFout(8)=1;
			//delay_ms(500);
			flag=1;
		}
		if(RTC_TimeStruct.RTC_Minutes==1)
		{
			flag=0;
		}
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}
