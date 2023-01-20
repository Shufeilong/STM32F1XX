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

void show_timer(){
	int i;
	int minu=0;
	int seco=0;
	int mill=0;
	char str[20];
	char x=24;
	int num_time=0;  //开始标志位
	OLED_CLS();
	while(1){
		show_Time(87,0,Time_time);
		Button_check_all();  //判断按钮
		
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

//中断服务函数     在startup_stm32f40_41xxx.s 里面找
void TIM4_IRQHandler(void)
{
	static char flag;
	//判断确实进中断
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		//立马清楚中断线
		
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
