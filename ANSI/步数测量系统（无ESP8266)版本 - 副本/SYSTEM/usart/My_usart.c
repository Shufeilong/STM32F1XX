#include "My_usart.h"
#include "sys.h"


void My_usart_init()
{
	
  GPIO_InitTypeDef GPIO_Initstructure;//GPIO初始化结构体
	USART_InitTypeDef USART_Initstructure;//串口初始化结构体

	//(1)第一步，使能串口时钟和对应GPIO口的时钟
	//GPIOA 和 USART1 时钟使能

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能 GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE );//使能 USART1 
	
	// (2)设置引脚复用器映射：调用 GPIO_PinAFConfig 函数。
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//PA9 复用为 USART1
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//PA10 复用为 USART1
	
// (3)GPIO 初始化设置：要设置模式为复用功能。
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//GPIO口A9和A10
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_Initstructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Initstructure.GPIO_Speed = GPIO_Fast_Speed;//速度 50MHz
	GPIO_Initstructure.GPIO_OType = GPIO_OType_PP;//推挽复用输出
	GPIO_Init(GPIOA,&GPIO_Initstructure); //初始化 PA9，PA10
	
	//(4)USART 初始化设置
	USART_Initstructure.USART_BaudRate =115200;//波特率
	USART_Initstructure.USART_WordLength = USART_WordLength_8b;//字长为八位数据格式
	USART_Initstructure.USART_StopBits =USART_StopBits_1 ;//一个停止位
	USART_Initstructure.USART_Parity = USART_Parity_No ;//无奇偶校验
	USART_Initstructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx ;//接收和发送模式
	USART_Initstructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;//异步不需要硬件流控制
	USART_Init(USART1,&USART_Initstructure);//初始化串口
	//配置中断
NVIC_Configuration();
//使能串口中断
USART_Cmd(USART1, ENABLE); 

}
//中断服务配置
void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;//中断结构体
	 	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	//使能串口接收中断
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	
	
}
//发送单个字符函数
void Usart_SendByte(USART_TypeDef *pUSART,uint8_t ch)
{
	USART_SendData(USART1,ch);
	while( USART_GetITStatus(USART1,USART_IT_TXE)==RESET);//获取中断状态标志位.等待发送完

}

