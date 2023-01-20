#include "uart.h"
#include "sys.h"
#include "string.h"
#include "OLED_I2C.h"
#include "stdio.h"
char Blue_str[100];
char Usart_get;

int fputc(int ch,FILE *fp)
{
	USART_SendData(USART3,ch);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET);
	return ch;
}

struct __FILE{
	int a;
};

FILE __stdout;

void _sys_exit(int x){}

void Usart_Init(unsigned band,char U_num)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	if(U_num==3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	}
	else if(U_num==1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	}
	
	GPIO_InitStruct.GPIO_Mode =    GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd =    GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	if(U_num==3){
		GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_10 | GPIO_Pin_11;//PA9   PA10
		GPIO_Init(GPIOB,&GPIO_InitStruct);
	}
	else if(U_num==1){
		GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_10 | GPIO_Pin_9;//PA9   PA10
		GPIO_Init(GPIOA,&GPIO_InitStruct);
	}
	
	if(U_num==3){
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	}
	else if(U_num==1){
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	}
	
	USART_InitStruct.USART_BaudRate=band;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	
	
	
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//抢占优先级   0-3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0x3;//响应优先级           0-3
	
	if(U_num==3){
		NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;//TIM4
		USART_Init(USART3,&USART_InitStruct);
		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
		USART_Cmd(USART3,ENABLE);
	}
	else if(U_num==1){
		NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//TIM4
		USART_Init(USART1,&USART_InitStruct);
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		USART_Cmd(USART1,ENABLE);
	}
	NVIC_Init(&NVIC_InitStruct); 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//蓝牙状态位IO口
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_3 ;//PE3  PE4   PE2
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/*
*测试是否连上蓝牙
*函数内放入连上和断开的操作即可
*
*/
void Usart_text(){
	if(PAin(3)==1){
		PEout(13)=0;//OLED_Fill(0x00);//全屏灭
		OLED_DrawBMP(0,0,16,2,0);
	}
	if(PAin(3)==0){
		PEout(13)=1;//OLED_Fill(0x00);//全屏灭
		OLED_DrawBMP(0,0,16,2,1);
	}
}


//蓝牙中断服务函数     在startup_stm32f40_41xxx.s 里面找
void USART3_IRQHandler(void)
{
	static char U_str_num=0;
	static char str[100]={0};
	//判断确实进中断
	if(U_str_num==0)
		;//memset(str,100,'\0');
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		//立马清楚中断线
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		str[U_str_num]=USART_ReceiveData(USART3);
		if(str[U_str_num]=='\n'){
			Usart_get=1;
			str[U_str_num]='\0';
			memset(Blue_str,100,'\0');
			strcpy(Blue_str,str);
			//printf("%s",str);
//销毁
				U_str_num=0;
		}
		else
			U_str_num++;
		
	}
}

char Uart_control_text(){
	char back_num=0;
	if(Usart_get){
		if(strstr(Blue_str,"up"))
			back_num=1;
		else if(strstr(Blue_str,"down"))
			back_num=2;
		else if(strstr(Blue_str,"enter"))
			back_num=3;
		else if(strstr(Blue_str,"back"))
			back_num=4;
	}
	memset(Blue_str,'\0',100);
	return back_num;
}
