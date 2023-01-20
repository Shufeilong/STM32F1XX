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
 
	USART_DeInit(USART3);  //��λ����3
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB11��GPIOB10��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��GPIOB11����GPIOB10
	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3	  
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�  

	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	

}
      




//�����ֽ�
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data)
{
	USART_SendData(USARTx,Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}	

/********************************************************************
���ƣ�WIFIģ�����ú���
����: ����ģ�飬�Ա����ʵ�����߽���Ϳ���,���ȴ�OK�ظ�
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
////////////////////////////û�еȴ�OK
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
���ƣ�WIFIģ�����ú���
����: ����ģ�飬û�лس�����
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
	 

	//�� RDR ��λ�Ĵ����������Ѵ��䵽 USART_DR �Ĵ���ʱ����λ��Ӳ���� 1
	if (USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//��ȡ�жϱ�־λ,Ϊ1
		{
				   Receive_shuju[im] = USART_ReceiveData( USART3 );
			     im++;
			  
			  	USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//����жϱ�־λ
			if((Receive_shuju[im-2]=='O')&&(Receive_shuju[im-1]=='K'))//���յ�OK֮�� OkwordΪ1
			{
				im=0;
				Okword=1;
			}
				
		}
		if( im>140)
			im=0;
	}
	
/********************************************************************
���ƣ���������
���ã��������������һ���Ӳ������������������ͻ���Ϊ�豸���ߣ��ʽ���30s����һ������
���ж�1�е��ã��ͷ������ݺ�������һ�����У���������Ҳ�൱������
********************************************************************/
void Ping(void)
{        
  ESP8266_Set(USART3,"cmd=0&msg=ping"); //��������
}


void connect_topic(void)
	{
        //���Ͷ���ָ�ԭ��ʽΪ��cmd=1&uid=***UID***&topic=***Topic***
        ESP8266_Set_Only(USART3,"cmd=1&uid=");
        ESP8266_Set_Only(USART3,Uid);
        ESP8266_Set_Only(USART3,"&topic=");
        
        //************ ��Ҫ�޸����²��� ************
   
        ESP8266_Set_Only(USART3,Topic);
    

        
       delay_us(5);
        USART_SendByte(USART3,'\r');//�س�
       delay_us(5);
        USART_SendByte(USART3,'\n');   //����
}
	
void connect_bemfa(void)
	{
       
        ESP8266_Set(USART3,"AT+CIPMODE=1"); //����͸������ģʽ
        delay_ms(1000);
        ESP8266_Set(USART3,"AT+CIPSTART=\"TCP\",\"bemfa.com\",8344");  // ���ӷ������Ͷ˿�
        delay_ms(1000);
        ESP8266_Set(USART3,"AT+CIPSEND"); //����͸��ģʽ�����淢�Ķ�������������
        delay_ms(1000);
        
        connect_topic(); //���Ӷ�������
      
      
               

}
	void connect_fasong(char data1[],char data2[],char data3[],char data4[]	)
	{
		    //���Ͷ���ָ�ԭ��ʽΪ��cmd=1&uid=***UID***&topic=***Topic***
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
        USART_SendByte(USART3,'\r');//�س�
        delay_us(5);
        USART_SendByte(USART3,'\n');   //����
	
		
	}
	

	

	
	