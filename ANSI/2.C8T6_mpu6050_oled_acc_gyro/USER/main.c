#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "TIME.h"
#include "mpu6050.h"
#include "string.h"	
#include "led.h"

extern short gyro[3], accel[3], sensors;    //定义在MPU6050中，	  DMP_Init()中通过dmp_read_fifo（）函数赋值。
float Pitch,Roll,Yaw;

int main(void)
{	
		unsigned char string [20];	
		delay_init();	    	 
		OLED_Init();							//oled的i2c	
		LED_Init();     
		OLED_Clear(); 	          
		IIC_Init();               // MPU6050的i2c
	  MPU6050_initialize();     //=====MPU6050初始化	
	  DMP_Init();
		TIM2_Getsample_Int(1999,719);		//50ms任务定时中断
			
		while(1)
	  {		
				sprintf((char *)string,"Pitch:%.2f",Pitch);
				OLED_ShowString(0,0,string,0);
				sprintf((char *)string,"Roll :%.2f",Roll);
				OLED_ShowString(0,1,string,0);
				sprintf((char *)string,"Yaw  :%.2f",Yaw);
				OLED_ShowString(0,2,string,0);
				sprintf((char *)string,"gyro :%d",gyro[1]);
				OLED_ShowString(0,3,string,0);
				sprintf((char *)string,"gyro :%d",accel[1]);
				OLED_ShowString(0,3,string,0);		

				delay_ms(80);
				GPIO_ResetBits(GPIOC,GPIO_Pin_13);          //灯的闪烁，标志程序是否在运行
				delay_ms(80);
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
				OLED_Clear();   //屏幕上不会有残留的像素点。			
		}
}	  
	



	 
