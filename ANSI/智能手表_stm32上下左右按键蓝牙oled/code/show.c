#include "OLED_I2C.h"
#include "delay.h"
#include "show.h"

void main_screen()
{
	OLED_ShowCN(0,4,12);
	OLED_ShowCN(0,6,12);
	OLED_ShowCN(112,4,13);
	OLED_ShowCN(112,6,14);
	show_Time(87,0,Time_time);
	OLED_DrawBMP(17,2,113,8,2);
}
void select_mode(int mode)
{
	int n;
	show_Time(87,0,Time_time);
	switch(mode)
	{
		case 1:
		{
			OLED_DrawBMP(14,2,30,4,temperature);
			OLED_DrawBMP(14,4,30,6,sunlight);
			OLED_DrawBMP(14,6,30,8,clock);
			OLED_DrawBMP(113,6,129,8,down_arrow);
			for(n=0;n<3;n++)//温湿度
			{
				OLED_ShowCN(32+n*16,2,n);
			}
			for(n=0;n<2;n++)//光强
			{
				OLED_ShowCN(32+n*16,4,n+3);
			}
			for(n=0;n<2;n++)//闹钟
			{
				OLED_ShowCN(32+n*16,6,n+5);
			}
			break;
		}
		case 2:
		{
			OLED_DrawBMP(14,2,30,4,calendar);
			OLED_DrawBMP(14,4,30,6,sunlight);
			OLED_DrawBMP(14,6,30,8,clock);
			OLED_DrawBMP(113,6,129,8,down_arrow);
			for(n=0;n<3;n++)//万年历
			{
				OLED_ShowCN(32+n*16,2,n+7);
			}
			for(n=0;n<2;n++)//休眠
			{
				OLED_ShowCN(32+n*16,4,n+10);
			}
			for(n=0;n<2;n++)//时间
			{
				OLED_ShowCN(32+n*16,6,n+15);
			}
			//break;
		}
		case 3:
		{
			OLED_DrawBMP(14,2,30,4,timer);
			OLED_DrawBMP(113,6,129,8,up_arrow);			
			for(n=0;n<2;n++)//秒表
			{
				OLED_ShowCN(32+n*16,2,n+19);
			}
		}
	}
	
}
void show_Time(int x,int y,int Time_Class)
{
	char str[30]={0};
	int week;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	switch(Time_Class)
	{
		case 0:		//显示不带秒的时间
		{
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Hours);
			OLED_ShowStr(x,y,str,2);
			OLED_ShowStr(x+17,y,":",2);
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Minutes);
			OLED_ShowStr(x+25,y,str,2);
			break;
		}
		case 1:		//显示带秒的时间
		{
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Hours);
			OLED_ShowStr(x,y,str,2);
			OLED_ShowStr(x+17,y,":",2);
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Minutes);
			OLED_ShowStr(x+25,y,str,2);
			OLED_ShowStr(x+41,y,":",2);
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Seconds);
			OLED_ShowStr(x+49,y,str,2);
			break;
		}
		case 2:		//显示星期
		{
			week=RTC_DateStruct.RTC_WeekDay;
			switch(week)
			{
				case RTC_Weekday_Monday:	//星期一
				{
					OLED_ShowStr(x,y,"Mon",2);
					break;
				}
				case RTC_Weekday_Tuesday:	//星期二
				{
					OLED_ShowStr(x,y,"Tue",2);
					break;
				}
				case RTC_Weekday_Wednesday:	//星期三
				{
					OLED_ShowStr(x,y,"Wed",2);
					break;
				}	
				case RTC_Weekday_Thursday:	//星期四
				{
					OLED_ShowStr(x,y,"Thu",2);
					break;
				}
				case RTC_Weekday_Friday:	//星期五
				{
					OLED_ShowStr(x,y,"Fri",2);
					break;
				}
				case RTC_Weekday_Saturday:	//星期六
				{
					OLED_ShowStr(x,y,"Sat",2);
					break;
				}
				case RTC_Weekday_Sunday:	//星期天
				{
					OLED_ShowStr(x,y,"Sun",2);
					break;
				}
				break;
			}
			
		}
		case 3:		//显示日期
		{
			sprintf(str,"20%02d",RTC_DateStruct.RTC_Year);
			OLED_ShowStr(x+2,y,str,2);//测试6*8字符
			OLED_ShowStr(x+33,y,"/",2);//测试6*8字符
			sprintf(str,"%02d",RTC_DateStruct.RTC_Month);
			OLED_ShowStr(x+41,y,str,2);//测试6*8字符
			OLED_ShowStr(x+57,y,"/",2);//测试6*8字符
			sprintf(str,"%02d",RTC_DateStruct.RTC_Date);
			OLED_ShowStr(x+65,y,str,2);//测试6*8字符
			break;
		}
	}
}
