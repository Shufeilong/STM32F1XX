#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "oled_myiic.h"
#include "myiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "mpu6050.h" 
#include <string.h>
#include "esp8266.h"
#include "timer.h"
typedef enum {no=0,yes=!no}bool;//定义一个bool类型
extern bool key_flag;
bool  key_flag=no;

float weight=60,weight_index=0.8214;//假设体重为60kG,运动指数
void OLED1_showchinese(void);//第一页显示函数
void OLED_2showchinese(void);//第二页显示
int main(void)
{   
	  u8 key_number;//获取按键值
	 
	  unsigned long	walk_count ,walk_time;//步行步数，步行时间。
    delay_init(168);		  
	  LED_Init();		
    usart3_init(115200);		//初始化串口1波特率为115200

    IIC_Init();         //MPU6050的IIC初始化
	  IIC_Init1();       //OLED的IIC初始化
    OLED_Init();       //OLED初始化     
		MPU_Init();					//初始化MPU6050
    mpu_dmp_init();        //初始化6050的DMP库
	OLED_init_showchinese();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	TIM3_Int_Init(60000-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数400000次为40000ms     
	ESP8266_Set_ok(USART3,"AT+RST");
	delay_ms(1000);	
	ESP8266_Set_ok(USART3,"AT+CWMODE=3");
	delay_ms(1000);
	ESP8266_Set_ok(USART3,"AT+CWJAP=\""Ssid"\",\""PassWord"\"");
	delay_ms(1000);
	connect_bemfa();//连接巴发云
	while(1)
	{
		
		key_number=KEY_Scan(1);//读取按键返回值
    if(key_number==1)//切换页面函数
		{
			key_flag=!key_flag;
		  OLED_CLS();
		}

		
		if(key_flag==no)
			
		   {   OLED1_showchinese();//第一页显示函数
				   //connect_fasong(char_step,char_sporttime,char_caloreis,"50"	);//传输
				   LED2=!LED2;
			 }
		else
		{  
			
			   OLED_2showchinese();
			   dmp_get_pedometer_step_count(&walk_count); 
		     dmp_get_pedometer_walk_time(&walk_time);
			   sprintf(char_step,"%lu",walk_count );//unsigend long 型变为无符号整型
			   sprintf(char_caloreis,"%.2f",walk_count*30*weight*weight_index/100000);//计算消耗的能量
			   OLED_P8x16Str(56,0,char_step );//显示计步步数
			   OLED_P8x16Str(48,2,char_caloreis );//显示消耗能量
			   sprintf(char_sporttime,"%lu",walk_time/1000 );
			   OLED_P8x16Str(70,4,char_sporttime);//显示运动时间
			   OLED_P8x16Str(84,6,"50"); //显示目标步
				 delay_ms(10);
			if(jishi==2)
				connect_fasong(char_step,char_sporttime,char_caloreis,"50"	);//传输
			
		 }
             
			
         
	                     
	}
}


