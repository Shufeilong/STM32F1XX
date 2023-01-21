#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "TIME.h"
#include "mpu6050.h"
#include "string.h"	
#include "led.h"

extern short gyro[3], accel[3], sensors;    //������MPU6050�У�	  DMP_Init()��ͨ��dmp_read_fifo����������ֵ��
float Pitch,Roll,Yaw;

int main(void)
{	
		unsigned char string [20];	
		delay_init();	    	 
		OLED_Init();							//oled��i2c	
		LED_Init();     
		OLED_Clear(); 	          
		IIC_Init();               // MPU6050��i2c
	  MPU6050_initialize();     //=====MPU6050��ʼ��	
	  DMP_Init();
		TIM2_Getsample_Int(1999,719);		//50ms����ʱ�ж�
			
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
				GPIO_ResetBits(GPIOC,GPIO_Pin_13);          //�Ƶ���˸����־�����Ƿ�������
				delay_ms(80);
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
				OLED_Clear();   //��Ļ�ϲ����в��������ص㡣			
		}
}	  
	



	 
