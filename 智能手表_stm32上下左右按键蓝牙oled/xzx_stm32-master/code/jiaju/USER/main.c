#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "sys.h"
#include "exti.h"
#include "tim.h"
#include "delay.h"
#include "pwm.h"
#include "uart.h"
#include "stdlib.h"
#include "string.h"//memset
#include "stdio.h"//fputc   printf
#include "dht11.h"
#include "infrared.h"
#include "rtc.h"
#include "adc.h"
#include "iwdg.h"
#include "OLED_I2C.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "show.h"
#include "mpu6050.h"

void model_Init(void);
void main_mode(void);
void select_mode(void);
void enter_mode(int mode_num);
void clock_mode(void);
void sleep_mode(void);
void calendar_mode(void);
void model_Init(void);
unsigned int Key;
unsigned char uart_recive_buff[20]={0};
unsigned char rx_buff[20]={0};
int cont=0;
int rx_i=0;
int line,page;

//中断服务函数     在startup_stm32f40_41xxx.s 里面找
	

int main(void)
{
	int key_a;

//	u16 adc_value=0;
//	RTC_TimeTypeDef RTC_TimeStruct;
//  RTC_DateTypeDef RTC_DateStruct;
//	uint32_t Data=0;
//	int temp,humi=0;
	
	
	model_Init();
	open_show(); 
	delay_s(1);
	main_screen();
	//OLED_CLS();
	line=1;
	page=1;
	main_mode();
	while(1)
	{
		//PFout(8)=0;
		//show_Dht();
		//RTC_set();
		//show_adc();
		//show_time();
		select_mode();
		/*
		delay_s(1);
		Usart_text();
		MPU_text();
		PFout(9)=!PFout(9);
		key_a=Button_check_all();
		
		*/
//		printf("key=%d,key_a=%d\n",Key,key_a);
//		key=Key_Scan();
//		if(key==1)
//		{
		//delay_s(1);
//			PFout(9)=!PFout(9);
//			
//		}
	}
	
	return 0;
}

void model_Init(){

	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//滴答时钟  168/8=21MHz
	KEY_Init();
	//Exti_Init();
	Tim_Init();
	//Pwm_Init();
	Dht11_Init();
	//Infrared_Init();
	I2C_Configuration();
	Rtc_Init();
	Adc_Init();
	//Iwdg_Init();
	Usart_Init(9600,3);
	OLED_Init();
	MPU_Set_Rate(1000);
	if(MPU_Init())
		printf("陀螺仪初始化失败\n");
	MPU_Get_Temperature();
	

}

void main_mode()
{
	while(!Button_check_all())
	{
		main_screen();
		Usart_text();
	}
	page=1;
	line=1;
	OLED_CLS();
}

void clock_mode()
{
	while(1)
	{
		show_Time(30,3,Time_times);
		OLED_ShowStr(46,7,"ENTER TO SET",1);
		Usart_text();
		if(Button_check_all())
		{
			PFout(8)=0;
			if(Key==3)
			{
				OLED_CLS();
				RTC_set();
				OLED_CLS();
			}
			else
			{
				return;
			}
		}
	}
}

void calendar_mode()
{
	char str[30];
	unsigned char Week_[7][5]={"Mon.","Tue.","Wed.","Thu.","Fri.","Sat.","Sun."};
	char x=17;
	char y=3;
	char i=1;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	u8 Time_num[4];
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	Time_num[0]=RTC_DateStruct.RTC_Year;
	Time_num[1]=RTC_DateStruct.RTC_Month;
	Time_num[2]=RTC_DateStruct.RTC_Date;
	i=RTC_DateStruct.RTC_WeekDay;
	//show_Time(20,3,Time_date);
	//show_Time(56,5,Time_week);
	OLED_CLS();
	while(1)
	{
		x=17;
		y=3;
		show_Time(87,0,Time_time);
		sprintf(str,"20%02d",Time_num[0]);
		OLED_ShowStr(x+2,y,(unsigned char *)str,2);//测试6*8字符
		OLED_ShowStr(x+33,y,"/",2);//测试6*8字符
		sprintf(str,"%02d",Time_num[1]);
		OLED_ShowStr(x+41,y,(unsigned char *)str,2);//测试6*8字符
		OLED_ShowStr(x+57,y,"/",2);//测试6*8字符
		sprintf(str,"%02d",Time_num[2]);
		OLED_ShowStr(x+65,y,(unsigned char *)str,2);//测试6*8字符
		/*
		sprintf(str,"%02d",Time_num[0]);
		OLED_ShowStr(20,3,str,2);
		OLED_ShowStr(x+16,3,":",2);
	*/	
		x=56;
		y=5;
		OLED_ShowStr(x,y,Week_[i-1],2);
		Usart_text();
		Button_check_all();  //判断按钮
		if(Key==1){
			Time_num[2]++;
			i++;
			if(i>=8)
				i=1;
		}
		else if(Key==2){
			Time_num[2]--;
			i-=1;

			if(i<=0)
				i=7;
		}
		else if(Key==4)
			break;
		
		
		Time_judge(&Time_num[0],&Time_num[1],&Time_num[2]);
	}
}


void sleep_mode()
{
	OLED_OFF();
	while((!MPU_text()) && !Button_check_all());
	OLED_ON();
}
void select_mode()
{
	select_screen(page);
	select_arr(line);
	Usart_text();
		if(Button_check_all()==1)
		{
			select_screen(page);
			select_arr(line);
			switch(Key)
			{
				case 1:line--;break;
				case 2:line++;break;
				case 3:enter_mode(3*(page-1)+line);break;
				case 4:main_mode();break;
			}
			if(line==0)
			{
				if(page==1)line=1;
				else
				{
					page--;
					line=3;
				}
			}
			if(line==4)
			{
				page++;
				line=1;
			}
			if(page==4)
			{
				page=3;
			}
			if(page==3)
			{
				if(line>1)line=1;
			}
	  }
}
void enter_mode(int mode_num)
{
	OLED_CLS();
	switch(mode_num)
	{
		case 1:temp();break;				//温湿度模式
		case 2:show_adc();break;								//光强值模式
		case 4:calendar_mode();break;//日历模式
		case 5:sleep_mode();break;	//休眠模式
		case 6:clock_mode();break;	//时钟模式
		case 7:show_timer();break;								//秒表 
	}
	OLED_CLS();
}
