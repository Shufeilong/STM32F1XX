#ifndef __MY_USART_H
#define __MY_USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h"  	
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
//如果想串口中断接收，请不要注释以下宏定义
void My_usart_init(void);//串口初始化
void NVIC_Configuration(void);//中断配置函数
void Usart_SendByte(USART_TypeDef *pUSART,uint8_t ch);//单个数据发送函数
#endif

