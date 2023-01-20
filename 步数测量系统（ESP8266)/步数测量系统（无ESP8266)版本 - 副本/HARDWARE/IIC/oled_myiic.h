
#ifndef __OLED_MYIIC_H
#define __OLED_MYIIC_H
#include "sys.h"
 
//这两句的意思分别是：PB15的输入/输出模式
#define SDA_IN1()  {GPIOB->MODER&=~(3<<(5*2));GPIOB->MODER|=0<<5*2;}	
#define SDA_OUT1() {GPIOB->MODER&=~(3<<(5*2));GPIOB->MODER|=1<<5*2;} 
	 
#define IIC_SCL1    PBout(4) //SCL
#define IIC_SDA1    PBout(5) //SDA	 
#define READ_SDA1   PBin(5)  //输出SDA 
 
 
void IIC_Init1(void);                				 
void IIC_Start1(void);				
void IIC_Stop1(void);	  			
void IIC_Send_Byte1(u8 txd);			
u8 IIC_Read_Byte1(unsigned char ack);
u8 IIC_Wait_Ack1(void); 				
void IIC_Ack1(void);					
void IIC_NAck1(void);				
 
void IIC_Write_One_Byte1(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte1(u8 daddr,u8 addr);	  
#endif
