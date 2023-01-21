#include "rtc.h"
#include "sys.h"
#include "show.h"
#include "key.h"
#include "string.h"
#include "delay.h"
#include "OLED_I2C.h"
#include "stdio.h"
#define Data_qwe 0x390
extern char Blue_str[100];
extern char Usart_get;
extern char Key;
void Rtc_Init(void)
{
    RTC_InitTypeDef RTC_InitStruct;
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
   //① 使能PWR(电池)时钟：
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR ,ENABLE);
	
   //②  使能后备寄存器访问:  
    PWR_BackupAccessCmd(ENABLE);
   //③  配置RTC时钟源，使能RTC时钟：32.768KHZ
      RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
      RCC_RTCCLKCmd(ENABLE);
    if(RTC_ReadBackupRegister(RTC_BKP_DR0) != Data_qwe)
    {
        // 如果使用LSE，要打开LSE：
        RCC_LSEConfig(RCC_LSE_ON);
       //④ 初始化RTC(同步/异步分频系数和时钟格式)：把频率变成1hz  
        RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;//24小时制
        RTC_InitStruct.RTC_AsynchPrediv = 0x7F;//异步分频器 7位  0-128
        RTC_InitStruct.RTC_SynchPrediv =  0xFF;//同步分频器 15位 0-32768
        RTC_Init(&RTC_InitStruct);
       //⑤ 设置时间：
        RTC_TimeStruct.RTC_H12 = RTC_H12_AM;//24小时制
        RTC_TimeStruct.RTC_Hours = 17; //10点
        RTC_TimeStruct.RTC_Minutes = 41;//10分
        RTC_TimeStruct.RTC_Seconds = 35;//12秒
        RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
        
        //⑥设置日期：
        RTC_DateStruct.RTC_Year = 21;//xx21年
        RTC_DateStruct.RTC_Month = RTC_Month_March;//3月
        RTC_DateStruct.RTC_Date = 10;//9日
        RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Tuesday;//星期二
        RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
        
        //将一个值写入后备寄存器，这样下次程序运行就不会进入设置RTC时钟
        RTC_WriteBackupRegister(RTC_BKP_DR0,Data_qwe);
    }  
	ALarm_set();
}

void ALarm_set()
{ 
 NVIC_InitTypeDef NVIC_InitStructure;
 EXTI_InitTypeDef   EXTI_InitStructure;
 RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
 RTC_TimeTypeDef RTC_TimeTypeInitStructure;
 
	PWR_BackupAccessCmd(ENABLE);
 RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//关闭闹钟A 
  RTC_TimeTypeInitStructure.RTC_Hours=17;//小时
 RTC_TimeTypeInitStructure.RTC_Minutes=41;//分钟
 RTC_TimeTypeInitStructure.RTC_Seconds=45;//秒
 RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
	
  RCC_LSEConfig(RCC_LSE_ON);
	
 RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=RTC_Weekday_Tuesday;//星期
 RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//按星期闹
 RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;//精确匹配星期，时分秒
 RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
 
 
 RTC_ClearITPendingBit(RTC_IT_ALRA);//清除RTC闹钟A的标志
  EXTI_ClearITPendingBit(EXTI_Line17);//清除LINE17上的中断标志位 
 
 RTC_ITConfig(RTC_IT_ALRA,ENABLE);//开启闹钟A中断
 RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//开启闹钟A 
 
 EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE17
  EXTI_Init(&EXTI_InitStructure);//配置

 NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
  
}


void RTC_set(){
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	int Num_get=0;
	int Key_a=0;
	int Num_now[2]={0};
	char Flash_=0;
	char Flash_num=0;
	char str[30]={0};
	u8 *Time_num[7];
	//if(Usart_get)
	PWR_BackupAccessCmd(ENABLE);
	//memset(Blue_str,'\0',100);
	//等待接收
	RCC_RTCCLKCmd(DISABLE);
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	Time_num[3]=&RTC_TimeStruct.RTC_Hours;
	Time_num[4]=&RTC_TimeStruct.RTC_Minutes;
	Time_num[5]=&RTC_TimeStruct.RTC_Seconds;
	Time_num[0]=&RTC_DateStruct.RTC_Year;
	Time_num[1]=&RTC_DateStruct.RTC_Month;
	Time_num[2]=&RTC_DateStruct.RTC_Date;
	Time_num[6]=&RTC_DateStruct.RTC_WeekDay;
	show_Time(25,5,Time_times);
	show_Time(17,3,Time_date);
	while(1){
		//printf("hell\n");
		
		//show_Time(14,3,Time_date);
		Key_a=Button_check_all();
		if(Key==4)		//退出
			break;
		else if(Key==3 && Key_a==1){//设置键
			Flash_=1;
			if(Flash_){
				Flash_num++;
				if(Num_now[0]!=0)
					flash_font(Num_now[0],Num_now[1],str,1);	
				//Flash_num=2;
						//把字写回来
			}	
			if(Flash_num>=7){
				Flash_num=0;
				Flash_=0;
			}
		}
		else if(Key==2){		//减少时间
			if(!(Key_a%10) || Key_a==1){
				printf("123,%d\n",RTC_TimeStruct.RTC_Hours);
				(*Time_num[Flash_num-1])--;
				Time_judge(&RTC_DateStruct.RTC_Year,&RTC_DateStruct.RTC_Month,&RTC_DateStruct.RTC_Date);
				RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
				RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
				Key_a=0;
			}
		}
		else if(Key==1){		//减少时间
			if((Key_a>1000 && !(Key_a%100)) || Key_a==1){
				(*Time_num[Flash_num-1])++;
				Time_judge(&RTC_DateStruct.RTC_Year,&RTC_DateStruct.RTC_Month,&RTC_DateStruct.RTC_Date);
				Date_judge(&RTC_TimeStruct.RTC_Hours,&RTC_TimeStruct.RTC_Minutes,&RTC_TimeStruct.RTC_Seconds);
				RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
				RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
				Key_a=0;
			}
		}
		//delay_ms(100);
		if(Flash_){
			switch(Flash_num){
				case 1:{
					sprintf(str,"20%02d",*Time_num[Flash_num-1]);
					flash_font(18,3,str,0);	
					Num_now[0]=18;
					Num_now[1]=3;
					break;
				}
				case 2:{
					sprintf(str,"%02d",*Time_num[Flash_num-1]);
					flash_font(58,3,str,0);	
					Num_now[0]=58;
					Num_now[1]=3;
					break;
				}
				case 3:{
					sprintf(str,"%02d",*Time_num[Flash_num-1]);
					flash_font(82,3,str,0);
					Num_now[0]=82;
					Num_now[1]=3;
						break;
				}
				case 4:{
					sprintf(str,"%02d",*Time_num[Flash_num-1]);
					flash_font(25,5,str,0);	
					Num_now[0]=25;
					Num_now[1]=5;
						break;
				}
				case 5:{
					sprintf(str,"%02d",*Time_num[Flash_num-1]);
					flash_font(50,5,str,0);
					Num_now[0]=50;
					Num_now[1]=5;
						break;
				}
				case 6:{
					sprintf(str,"%02d",*Time_num[Flash_num-1]);
					flash_font(74,5,str,0);
					Num_now[0]=74;
					Num_now[1]=5;
						break;
				}
				
			}
			
		}
			
		
	/*	
		
		if(Key){
			Num_get=num_get();
#ifdef __TEXT__
			printf("%d\n",Num_get);
#endif
			if(Num_get>=0 && Num_get<=24){
				RTC_TimeStruct.RTC_Hours=Num_get;
				RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
				memset(Blue_str,'\0',100);
				printf("succes set\n");
			}
			else{
				printf("error set\n");
				memset(Blue_str,'\0',100);
			}
		}
		else if(strstr(Blue_str,"Minu=")){
			//Num_get=(Blue_str[5]-'0')*10+Blue_str[6]-'0';
			Num_get=num_get();
#ifdef __TEXT__
			printf("%d,%c,%c\n",Num_get,Blue_str[5],Blue_str[6]);
			printf("%s\n",Blue_str);
			printf("%d\n",Blue_str[5]);
#endif
			if(Num_get>=0 && Num_get<=59){
				RTC_TimeStruct.RTC_Minutes=Num_get;
				RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
				memset(Blue_str,'\0',100);
				printf("succes set");
			}
			else{
				printf("error set\n");
				memset(Blue_str,'0',100);
			}
		}
		else if(strstr(Blue_str,"Seco=")){
			Num_get=num_get();
			if(Num_get>=0 && Num_get<=59){
				RTC_TimeStruct.RTC_Seconds=Num_get;
				RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
				printf("succes set");
			}
			else
				printf("error set\n");
			memset(Blue_str,'\0',100);
		}
		else if(strstr(Blue_str,"Break")){
			printf("exit now");
			PWR_BackupAccessCmd(DISABLE);
			Usart_get=0;
			memset(Blue_str,'\0',100);
			
			break;
		}
		//delay_ms(100);
	}
	*/
	}
	RCC_RTCCLKCmd(ENABLE);
}

void Time_judge(u8 *Year,char *mouth,char *day){
	if(*mouth>12){    //月份判断
		*mouth=1;
		*Year+=1;
	}
	else if(*mouth<=0){
		*mouth=12;
		*Year-=1;
	}
	
	if(*Year>150)
		*Year=99;
	else if(*Year>99)
		*Year=0;
	
	if(*day>28 && *mouth==2 && *day<100){
		if(!(*Year%400) || (!(*Year%4) && *Year%400)){
			if(*day>29){
				*day=1;
				*mouth+=1;
			}
		}
		else{
			*day=1;
			*mouth+=1;
		}
	}
	else if(*day>30 && *day<100){
		if(!(*mouth==1 || *mouth==3 || *mouth==5 || *mouth==7 || *mouth==8 || *mouth==10 ||*mouth==12)){
			*day=1;
			*mouth+=1;
		}
		else if(*day>31){
			*day=1;
			*mouth+=1;
		}
	}
	else if(*day<=0){
		if((*mouth==2 || *mouth==4 || *mouth==6 || *mouth==8 || *mouth==9 || *mouth==11 || *mouth==1)){
			*day=31;
			*mouth-=1;
		}
		else if(*mouth==3){
			if(!(*Year%400) || (!(*Year%4) && *Year%400))
				*day=29;
			else
				*day=28;
			*mouth-=1;
		}
	}
		

}

void Date_judge(unsigned char *hour,unsigned char *minu,unsigned char *seco){
	if(*hour>=24 && *hour<100)
		*hour=0;
	else if(*hour<0 || *hour>150)
		*hour=23;
	
	if(*minu>=60 && *minu<100)
		*minu=0;
	if(*minu<0 || *minu>150)
		*minu=59;
	
	if(*seco>=60 && *seco<100)
		*seco=0;
	else if(*seco<0 || *seco>150)
		*seco=59;
}

void flash_font(char x,char y,char str[],char Compulsory){
	static char Flash_sta=0;
	static int Flash_num=0;
	Flash_num++;
	if(Flash_num>=10000 || Compulsory){
		if(!Flash_sta && !Compulsory){
			if(strlen(str)>=4){
				OLED_ShowCN(x,y,29);
				OLED_ShowCN(x+16,y,29);			
			}
			else
				OLED_ShowCN(x,y,29);
		}
		else
			OLED_ShowStr(x,y,(unsigned char *)str,2);
		Flash_sta=!Flash_sta;
		Flash_num=0;
	}
}

int num_get(){
	int Num_get=0;
	int i=0;
	while(Blue_str[i]==' ')
		i++;
	if(Blue_str[i+5]=='\0'){
		Num_get=0;
	}
	else if(Blue_str[i+6]=='\0')
		Num_get=Blue_str[i+5]-48;
	else
		Num_get=((Blue_str[i+5]-48)*10+(int)Blue_str[i+6]-48);
#ifdef __TEXT__
	printf("Blue_str[i+5]=%c\n",Blue_str[i+5]);
#endif
	return Num_get;
}

void RTC_text(){
	if(Usart_get==1 ){
		if(strstr(Blue_str,"Settime")){
			Usart_get=0;
			RTC_set();
		}
	}
}

void RTC_Alarm_IRQHandler(){
	PEout(14)=0;
	//while(1);
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET){
		//立马清楚中断线
		RTC_ClearFlag(RTC_FLAG_ALRAF);
		PEout(14)=0;
	}
	

}
