#include "OLED_I2C.h"
#include "delay.h"
#include "show.h"
#include "dht11.h"
#include "stdio.h"
#include "key.h"
#include "uart.h"
extern char Key;

void open_show()
{
	int num_del=0;
	int num_1=0;
	OLED_DrawBMP(0,0,128,8,open);
	WriteCmd(0x2E);
	//WriteCmd(0x81);
	//WriteCmd(0x00);
	while(num_del<0xff){
		delay_ms(5);
		WriteCmd(0x81); //--set contrast control register
		WriteCmd(num_del); //���ȵ��� 0x00~0xff
		num_del+=1;
		WriteCmd(0x2E);      //�رչ���
		WriteCmd(0x26);      //ˮƽ��������ҹ��� 26/27
		WriteCmd(0x00);       //�����ֽ�
		WriteCmd(0x00);       //��ʼҳ 0
		WriteCmd(0x07);      //����ʱ����
		WriteCmd(0x07);       //��ֹҳ 7
		WriteCmd(0x00);        //�����ֽ�
		WriteCmd(0xFF);       //�����ֽ�
		WriteCmd(0x2F);       //��������		
	}
		//OLED_Fill(0x00);
	WriteCmd(0x2E);
	num_del=0;
	OLED_DrawBMP(0,0,128,8,14);
	while(num_del<0xff){
		delay_ms(5);
		WriteCmd(0x81); //--set contrast control register
		WriteCmd(num_del); //���ȵ��� 0x00~0xff
		num_del+=1;
		WriteCmd(0x2E);      //�رչ���
		WriteCmd(0x27);      //ˮƽ��������ҹ��� 26/27
		WriteCmd(0x00);       //�����ֽ�
		WriteCmd(0x00);       //��ʼҳ 0
		WriteCmd(0x07);      //����ʱ����
		WriteCmd(0x07);       //��ֹҳ 7
		WriteCmd(0x00);        //�����ֽ�
		WriteCmd(0xFF);       //�����ֽ�
		WriteCmd(0x2F);       //��������			
	}
	WriteCmd(0x2E);
			//OLED_Fill(0x00);
	OLED_DrawBMP(0,0,128,8,15);
	while(num_1<0xff){
		delay_ms(10);
		WriteCmd(0x81); //--set contrast control register
		WriteCmd(num_1); //���ȵ��� 0x00~0xff
		num_1+=1;
		WriteCmd(0x2E);      //�رչ���
		WriteCmd(0x29);      //ˮƽ��������ҹ��� 26/27
		WriteCmd(0x00);       //�����ֽ�
		WriteCmd(0x00);       //��ʼҳ 0
		WriteCmd(0x07);      //����ʱ����
		WriteCmd(0x07);       //��ֹҳ 7
		//WriteCmd(0x00);        //�����ֽ�
		WriteCmd(0x01);       //�����ֽ�
		WriteCmd(0x2F);       //��������
	}
	WriteCmd(0x2E);      //�رչ���
	OLED_Fill(0x00);
	delay_s(1);
	WriteCmd(0x81);
	WriteCmd(0x00);
	WriteCmd(0XAE);  //OLED����
	OLED_DrawBMP(17,2,113,8,logo_overwatch);
	WriteCmd(0XAF);  //OLED����
	num_1=0;
	while(num_1<0xff){
		delay_ms(15);
		WriteCmd(0x81); //--set contrast control register
		WriteCmd(num_1); //���ȵ��� 0x00~0xff 
		num_1+=1;
	}
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0x00); //���ȵ��� 0x00~0xff 
	num_1+=1;
	while(num_1<0xff){
		delay_ms(15);
		WriteCmd(0x81); //--set contrast control register
		WriteCmd(num_1); //���ȵ��� 0x00~0xff 
		num_1+=1;
	
	}
		
	WriteCmd(0x81);
	WriteCmd(0xFF); 
}

void main_screen()
{  int num_1=0;
	 
	OLED_DrawBMP(17,2,113,8,logo_overwatch);
	
	//OLED_DrawBMP(17,2,113,8,logo_overwatch);
	
	//OLED_Fill(0x00);//ȫ����
	//OLED_DrawBMP(17,2,113,8,logo_overwatch);
	OLED_ShowCN(0,4,12);
	OLED_ShowCN(0,6,12);
	OLED_ShowCN(112,4,13);
	OLED_ShowCN(112,6,14);
	show_Time(87,0,Time_time);
	//OLED_DrawBMP(17,2,113,8,logo_overwatch);
}


void select_screen(int mode)//ѡ�����
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
			for(n=0;n<3;n++)//��ʪ��
			{
				OLED_ShowCN(32+n*16,2,n);
			}
			for(n=0;n<2;n++)//��ǿ
			{
				OLED_ShowCN(32+n*16,4,n+3);
			}
			for(n=0;n<2;n++)//����
			{
				OLED_ShowCN(32+n*16,6,n+5);
			}
			break;
		}
		case 2:
		{
			OLED_DrawBMP(14,2,30,4,calendar);
			OLED_DrawBMP(14,4,30,6,sleep);
			OLED_DrawBMP(14,6,30,8,clock);
			OLED_DrawBMP(113,6,129,8,down_arrow);
			for(n=0;n<3;n++)//������
			{
				OLED_ShowCN(32+n*16,2,n+7);
			}
			for(n=0;n<2;n++)//����
			{
				OLED_ShowCN(32+n*16,4,n+10);
			}
			for(n=0;n<2;n++)//ʱ��
			{
				OLED_ShowCN(32+n*16,6,n+15);
			}
			break;
		}
		case 3:
		{
			OLED_DrawBMP(14,2,30,4,timer);
			OLED_DrawBMP(14,4,30,6,none);
			OLED_DrawBMP(14,6,30,8,none);
			OLED_DrawBMP(113,6,129,8,up_arrow);			
			for(n=0;n<2;n++)//���
			{
				OLED_ShowCN(32+n*16,2,n+19);
			}
			OLED_ShowCN(32+2*16,2,29);
			for(n=0;n<3;n++)//���
			{
				OLED_ShowCN(32+n*16,4,29);
			}
			for(n=0;n<3;n++)//���
			{
				OLED_ShowCN(32+n*16,6,29);
			}
		}
	}
	
}


void temp()
{
	uint32_t Data=0;
	int temp,humi=0;
	char str[30]={0};
	while(1)
	{
		
		show_Time(87,0,Time_time);
		Usart_text();
		Data = get_dht11_data();
		humi = Data & 0xff;
		temp = (Data>>16) & 0xff;
		if(humi==0 || temp==0)continue;
		OLED_ShowCN(14,3,0);
		OLED_ShowCN(30,3,2);
		OLED_ShowStr(46,3,":",2);
		sprintf(str,"%02d",temp);
		OLED_ShowStr(54,3,(unsigned char *)str,2);
		OLED_ShowCN(14,5,1);
		OLED_ShowCN(30,5,2);
		OLED_ShowStr(46,5,":",2);
		sprintf(str,"%02d",humi);
		OLED_ShowStr(54,5,(unsigned char *)str,2);
		if((Button_check_all()==1)&&(Key==4))break;
	}
	return;
}
void select_arr(int line)
{
	int j;
	for(j=1;j<4;j++)
	{
		if(j==line)continue;
		OLED_DrawBMP(94,2*j,110,2+2*j,none);
	}
	OLED_DrawBMP(94,2*line,110,2+2*line,select_arrow);
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
		case 0:		//��ʾ�������ʱ��
		{
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Hours);
			OLED_ShowStr(x,y,(unsigned char *)str,2);
			OLED_ShowStr(x+17,y,":",2);
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Minutes);
			OLED_ShowStr(x+25,y,(unsigned char *)str,2);
			break;
		}
		case 1:		//��ʾ�����ʱ��
		{
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Hours);
			OLED_ShowStr(x,y,(unsigned char *)str,2);
			OLED_ShowStr(x+17,y,":",2);
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Minutes);
			OLED_ShowStr(x+25,y,(unsigned char *)str,2);
			OLED_ShowStr(x+41,y,":",2);
			sprintf(str,"%02d",RTC_TimeStruct.RTC_Seconds);
			OLED_ShowStr(x+49,y,(unsigned char *)str,2);
			break;
		}
		case 2:		//��ʾ����
		{
			
			week=RTC_DateStruct.RTC_WeekDay;
			switch(week)
			{
				case RTC_Weekday_Monday:	//����һ
				{
					OLED_ShowStr(x,y,"Mon",2);
					break;
				}
				case RTC_Weekday_Tuesday:	//���ڶ�
				{
					OLED_ShowStr(x,y,"Tue",2);
					break;
				}
				case RTC_Weekday_Wednesday:	//������
				{
					OLED_ShowStr(x,y,"Wed",2);
					break;
				}	
				case RTC_Weekday_Thursday:	//������
				{
					OLED_ShowStr(x,y,"Thu",2);
					break;
				}
				case RTC_Weekday_Friday:	//������
				{
					OLED_ShowStr(x,y,"Fri",2);
					break;
				}
				case RTC_Weekday_Saturday:	//������
				{
					OLED_ShowStr(x,y,"Sat",2);
					break;
				}
				case RTC_Weekday_Sunday:	//������
				{
					OLED_ShowStr(x,y,"Sun",2);
					break;
				}
			}
			break;
		}
		case 3:		//��ʾ����
		{
			sprintf(str,"20%02d",RTC_DateStruct.RTC_Year);
			OLED_ShowStr(x+2,y,(unsigned char *)str,2);//����6*8�ַ�
			OLED_ShowStr(x+33,y,"/",2);//����6*8�ַ�
			sprintf(str,"%02d",RTC_DateStruct.RTC_Month);
			OLED_ShowStr(x+41,y,(unsigned char *)str,2);//����6*8�ַ�
			OLED_ShowStr(x+57,y,"/",2);//����6*8�ַ�
			sprintf(str,"%02d",RTC_DateStruct.RTC_Date);
			OLED_ShowStr(x+65,y,(unsigned char *)str,2);//����6*8�ַ�
			break;
		}
	}
}
