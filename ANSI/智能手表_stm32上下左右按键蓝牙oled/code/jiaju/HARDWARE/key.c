#include "key.h"
#include "sys.h"
#include "uart.h"
extern unsigned int Key;
void delay_xd(unsigned int i)
{
	unsigned int a=0;
	while(i--)
	{
		for(a=0;a<12000;a++);//对应有一个12Mhz晶振   大概需要1ms
	}
}

void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	Key=0;
	//必须先初始化时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_0;//PA0
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruct);
    
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;//PE3  PE4   PE2
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOE,&GPIO_InitStruct);
}

int Key_Scan(void)
{
	
	static int key_a=0;
	if(PAin(0)==0)
	{
		//消抖
		delay_xd(10);//大概延时10ms
		if(PAin(0)==0)
		{
			Key=1;
			key_a++;
		}
	}
	else if(PEin(2)==0)
	{
		//消抖
		delay_xd(10);//大概延时10ms
		if(PEin(2)==0)
		{
			Key=2;	
			key_a++;
		}
	}
	else if(PEin(3)==0)
	{
		//消抖
		delay_xd(10);//大概延时10ms
		if(PEin(3)==0)
		{
			Key=3;	
			key_a++;
		}
	}
	else if(PEin(4)==0)
	{
		//消抖
		delay_xd(10);//大概延时10ms
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
		return 1; //返回只按一次的
	return 0;
}

