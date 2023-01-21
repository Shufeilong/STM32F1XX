#include <stdio.h>	 	  
#include "esp8266.h"
#include "string.h"	
#include "sys.h"
unsigned char Receive_shuju[150];
unsigned char Okword=0;
char char_step[4],char_caloreis[6],char_sporttime[4];
void usart3_init(u32 bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	USART_DeInit(USART3);  //复位串口3
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB11和GPIOB10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化GPIOB11，和GPIOB10
	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART3	  
	
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断  

	USART_Cmd(USART3, ENABLE);                    //使能串口 
	
 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	

}
      




//发送字节
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data)
{
	USART_SendData(USARTx,Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}	

/********************************************************************
名称：WIFI模块设置函数
作用: 启动模块，以便可以实现无线接入和控制,并等待OK回复
********************************************************************/
void  ESP8266_Set_ok(USART_TypeDef* USARTx, char *str)
{
	uint16_t i = 0;
	do{
			USART_SendByte(USARTx, *(str + i));
			i++;
	}while(*(str + i) !='\0');
	delay_us(5);
	USART_SendByte( USART3,'\r');
	delay_us(5);
	USART_SendByte( USART3,'\n');

	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	while(!Okword);
		Okword=0;
}
////////////////////////////没有等待OK
void  ESP8266_Set(USART_TypeDef* USARTx, char *str)
{
	uint16_t i = 0;
	do{
			USART_SendByte(USARTx, *(str + i));
			i++;
	}while(*(str + i) !='\0');
	delay_us(5);
	USART_SendByte( USART3,'\r');
	delay_us(5);
	USART_SendByte( USART3,'\n');
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

}
/********************************************************************
名称：WIFI模块设置函数
作用: 启动模块，没有回车换行
********************************************************************/
void ESP8266_Set_Only(USART_TypeDef* USARTx, char *str)
{
	uint16_t i = 0;
	do{
			USART_SendByte(USARTx, *(str + i));
			i++;
	}while(*(str + i) !='\0');

	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}



void USART3_IRQHandler()
{ 
  
static int im=0;
	 

	//当 RDR 移位寄存器的内容已传输到 USART_DR 寄存器时，该位由硬件置 1
	if (USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//获取中断标志位,为1
		{
				   Receive_shuju[im] = USART_ReceiveData( USART3 );
			     im++;
			  
			  	USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//清除中断标志位
			if((Receive_shuju[im-2]=='O')&&(Receive_shuju[im-1]=='K'))//接收到OK之后 Okword为1
			{
				im=0;
				Okword=1;
			}
				
		}
		if( im>140)
			im=0;
	}
	
/********************************************************************
名称：心跳函数
作用：发送心跳。如果一分钟不发送心跳，服务器就会认为设备掉线，故建议30s发送一次心跳
在中断1中调用，和发送数据函数，留一个就行，发送数据也相当于心跳
********************************************************************/
void Ping(void)
{        
  ESP8266_Set(USART3,"cmd=0&msg=ping"); //发送心跳
}


void connect_topic(void)
	{
        //发送订阅指令，原格式为：cmd=1&uid=***UID***&topic=***Topic***
        ESP8266_Set_Only(USART3,"cmd=1&uid=");
        ESP8266_Set_Only(USART3,Uid);
        ESP8266_Set_Only(USART3,"&topic=");
        
        //************ 需要修改以下部分 ************
   
        ESP8266_Set_Only(USART3,Topic);
    

        
       delay_us(5);
        USART_SendByte(USART3,'\r');//回车
       delay_us(5);
        USART_SendByte(USART3,'\n');   //换行
}
	
void connect_bemfa(void)
	{
       
        ESP8266_Set(USART3,"AT+CIPMODE=1"); //开启透明传输模式
        delay_ms(1000);
        ESP8266_Set(USART3,"AT+CIPSTART=\"TCP\",\"bemfa.com\",8344");  // 连接服务器和端口
        delay_ms(1000);
        ESP8266_Set(USART3,"AT+CIPSEND"); //进入透传模式，下面发的都会无条件传输
        delay_ms(1000);
        
        connect_topic(); //连接订阅主题
      
      
               

}
	void connect_fasong(char data1[],char data2[],char data3[],char data4[]	)
	{
		    //发送订阅指令，原格式为：cmd=1&uid=***UID***&topic=***Topic***
        ESP8266_Set_Only(USART3,"cmd=2&uid=");
        ESP8266_Set_Only(USART3,Uid);
        ESP8266_Set_Only(USART3,"&topic=");
        ESP8266_Set_Only(USART3,Topic);
		    ESP8266_Set_Only(USART3,"&msg=");
		    ESP8266_Set_Only(USART3,"#");
        ESP8266_Set_Only(USART3,data1);
		    ESP8266_Set_Only(USART3,"#");
		    ESP8266_Set_Only(USART3,data2);
		    ESP8266_Set_Only(USART3,"#");
		    ESP8266_Set_Only(USART3,data3);
		    ESP8266_Set_Only(USART3,"#");
		    ESP8266_Set_Only(USART3,data4);
		    ESP8266_Set_Only(USART3,"#");
        delay_us(5);
        USART_SendByte(USART3,'\r');//回车
        delay_us(5);
        USART_SendByte(USART3,'\n');   //换行
	
		
	}
	

	

	
	