#ifndef __MY_USART_H
#define __MY_USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h"  	
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void My_usart_init(void);//���ڳ�ʼ��
void NVIC_Configuration(void);//�ж����ú���
void Usart_SendByte(USART_TypeDef *pUSART,uint8_t ch);//�������ݷ��ͺ���
#endif

