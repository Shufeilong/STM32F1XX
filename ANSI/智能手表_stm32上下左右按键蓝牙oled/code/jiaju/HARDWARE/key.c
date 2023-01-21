#include "key.h"
#include "sys.h"
#include "uart.h"
extern unsigned int Key;
void delay_xd(unsigned int i)
{
	unsigned int a=0;
	while(i--)
	{
		for(a=0;a<12000;a++);//��Ӧ��һ��12Mhz����   �����Ҫ1ms
	}
}

void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	Key=0;
	//�����ȳ�ʼ��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_0;//PA0
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//����   50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruct);
    
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;//PE3  PE4   PE2
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//����   50MHz
	GPIO_Init(GPIOE,&GPIO_InitStruct);
}

int Key_Scan(void)
{
	
	static int key_a=0;
	if(PAin(0)==0)
	{
		//����
		delay_xd(10);//�����ʱ10ms
		if(PAin(0)==0)
		{
			Key=1;
			key_a++;
		}
	}
	else if(PEin(2)==0)
	{
		//����
		delay_xd(10);//�����ʱ10ms
		if(PEin(2)==0)
		{
			Key=2;	
			key_a++;
		}
	}
	else if(PEin(3)==0)
	{
		//����
		delay_xd(10);//�����ʱ10ms
		if(PEin(3)==0)
		{
			Key=3;	
			key_a++;
		}
	}
	else if(PEin(4)==0)
	{
		//����
		delay_xd(10);//�����ʱ10ms
		if(PEin(4)==0)
		{
			Key=4;	
			key_a++;
		}
	}
	else{
		key_a=0;
		Key=0;
	}
	return key_a;
}

char Button_check_all(){
	int key_a=0;
	key_a=Key_Scan();
	if(key_a)
		return key_a;
	Key=Uart_control_text();
	if(Key)
		return 1; //����ֻ��һ�ε�
	return 0;
}

