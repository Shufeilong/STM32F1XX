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
typedef enum {no=0,yes=!no}bool;//����һ��bool����
extern bool key_flag;
bool  key_flag=no;

float weight=60,weight_index=0.8214;//��������Ϊ60kG,�˶�ָ��
void OLED1_showchinese(void);//��һҳ��ʾ����
void OLED_2showchinese(void);//�ڶ�ҳ��ʾ
int main(void)
{   
	  u8 key_number;//��ȡ����ֵ
	 
	  unsigned long	walk_count ,walk_time;//���в���������ʱ�䡣
    delay_init(168);		  
	  LED_Init();		
    usart3_init(115200);		//��ʼ������1������Ϊ115200

    IIC_Init();         //MPU6050��IIC��ʼ��
	  IIC_Init1();       //OLED��IIC��ʼ��
    OLED_Init();       //OLED��ʼ��     
		MPU_Init();					//��ʼ��MPU6050
    mpu_dmp_init();        //��ʼ��6050��DMP��
	OLED_init_showchinese();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	TIM3_Int_Init(60000-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����400000��Ϊ40000ms     
	ESP8266_Set_ok(USART3,"AT+RST");
	delay_ms(1000);	
	ESP8266_Set_ok(USART3,"AT+CWMODE=3");
	delay_ms(1000);
	ESP8266_Set_ok(USART3,"AT+CWJAP=\""Ssid"\",\""PassWord"\"");
	delay_ms(1000);
	connect_bemfa();//���Ӱͷ���
	while(1)
	{
		
		key_number=KEY_Scan(1);//��ȡ��������ֵ
    if(key_number==1)//�л�ҳ�溯��
		{
			key_flag=!key_flag;
		  OLED_CLS();
		}

		
		if(key_flag==no)
			
		   {   OLED1_showchinese();//��һҳ��ʾ����
				   //connect_fasong(char_step,char_sporttime,char_caloreis,"50"	);//����
				   LED2=!LED2;
			 }
		else
		{  
			
			   OLED_2showchinese();
			   dmp_get_pedometer_step_count(&walk_count); 
		     dmp_get_pedometer_walk_time(&walk_time);
			   sprintf(char_step,"%lu",walk_count );//unsigend long �ͱ�Ϊ�޷�������
			   sprintf(char_caloreis,"%.2f",walk_count*30*weight*weight_index/100000);//�������ĵ�����
			   OLED_P8x16Str(56,0,char_step );//��ʾ�Ʋ�����
			   OLED_P8x16Str(48,2,char_caloreis );//��ʾ��������
			   sprintf(char_sporttime,"%lu",walk_time/1000 );
			   OLED_P8x16Str(70,4,char_sporttime);//��ʾ�˶�ʱ��
			   OLED_P8x16Str(84,6,"50"); //��ʾĿ�경
				 delay_ms(10);
			if(jishi==2)
				connect_fasong(char_step,char_sporttime,char_caloreis,"50"	);//����
			
		 }
             
			
         
	                     
	}
}


