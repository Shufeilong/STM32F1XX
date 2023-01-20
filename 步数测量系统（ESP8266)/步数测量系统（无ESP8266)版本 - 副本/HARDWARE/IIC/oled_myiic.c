#include "oled_myiic.h"
#include "delay.h"
	
 

void IIC_Init1(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	IIC_SCL1=1;
	IIC_SDA1=1;
}
 
 
void IIC_Start1(void)
{
	SDA_OUT1();     
	IIC_SDA1=1;	  	  
	IIC_SCL1=1;  
	delay_us(2);
 	IIC_SDA1=0;     
	delay_us(2);
	IIC_SCL1=0;     //钳住I2C，准备接受或者发送数据
}	  
 
 
 
void IIC_Stop1(void)
{
	SDA_OUT1();//sda??ê?3?
	IIC_SCL1=0;
	IIC_SDA1=0;
 	delay_us(2);
	IIC_SCL1=1; 
	IIC_SDA1=1;
	delay_us(2);							   					   	
}
 
 
//返回值：1：成功 0：失败
 
 
u8 IIC_Wait_Ack1(void)
{
	u8 ucErrTime=0;
	SDA_IN1();      
	IIC_SDA1=1;delay_us(1);   
	IIC_SCL1=1;delay_us(1);	
  
	while(READ_SDA1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop1();
			return 1;
		}
	}
	IIC_SCL1=0;  
	return 0;  
} 
 
 
void IIC_Ack1(void)
{
	IIC_SCL1=0;
	SDA_OUT1();
	IIC_SDA1=0;
	delay_us(4);
	IIC_SCL1=1;
	delay_us(4);
	IIC_SCL1=0;
}
 
	    
void IIC_NAck1(void)
{
	IIC_SCL1=0;
	SDA_OUT1();
	IIC_SDA1=1;
	delay_us(4);
	IIC_SCL1=1;
	delay_us(4);
	IIC_SCL1=0;
}					 				     
		
//u8 test=0;
void IIC_Send_Byte1(u8 txd)
{                        
    u8 t;   
	  SDA_OUT1(); 	    
    IIC_SCL1=0;
	 
    for(t=0;t<8;t++)
    {             
			if((txd&0x80)>>7)
        IIC_SDA1=1;
			else 
				IIC_SDA1=0;
			
        txd<<=1; 	 
			  delay_us(5);
				IIC_SCL1=1;
				delay_us(5); 
				IIC_SCL1=0;	
				delay_us(5);
    }	 
} 	    
 
 
 
   
 
u8 IIC_Read_Byte1(unsigned char ack)
{
	unsigned char i,receive=0;
	 SDA_IN1();
    for(i=0;i<8;i++ )
	{
        IIC_SCL1=0; 
        delay_us(5);
		    IIC_SCL1=1;
        receive<<=1;
		     
        if(READ_SDA1)receive++;   
		   delay_us(5); 
    }					 
    if (!ack)
        IIC_NAck1();
    else
        IIC_Ack1();   
    return receive;
}
