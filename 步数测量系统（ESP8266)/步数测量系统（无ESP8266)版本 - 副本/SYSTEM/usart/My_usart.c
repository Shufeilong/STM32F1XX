#include "My_usart.h"
#include "sys.h"


void My_usart_init()
{
	
  GPIO_InitTypeDef GPIO_Initstructure;//GPIO��ʼ���ṹ��
	USART_InitTypeDef USART_Initstructure;//���ڳ�ʼ���ṹ��

	//(1)��һ����ʹ�ܴ���ʱ�ӺͶ�ӦGPIO�ڵ�ʱ��
	//GPIOA �� USART1 ʱ��ʹ��

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ�� GPIOA ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE );//ʹ�� USART1 
	
	// (2)�������Ÿ�����ӳ�䣺���� GPIO_PinAFConfig ������
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//PA9 ����Ϊ USART1
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//PA10 ����Ϊ USART1
	
// (3)GPIO ��ʼ�����ã�Ҫ����ģʽΪ���ù��ܡ�
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//GPIO��A9��A10
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_Initstructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Initstructure.GPIO_Speed = GPIO_Fast_Speed;//�ٶ� 50MHz
	GPIO_Initstructure.GPIO_OType = GPIO_OType_PP;//���츴�����
	GPIO_Init(GPIOA,&GPIO_Initstructure); //��ʼ�� PA9��PA10
	
	//(4)USART ��ʼ������
	USART_Initstructure.USART_BaudRate =115200;//������
	USART_Initstructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ��λ���ݸ�ʽ
	USART_Initstructure.USART_StopBits =USART_StopBits_1 ;//һ��ֹͣλ
	USART_Initstructure.USART_Parity = USART_Parity_No ;//����żУ��
	USART_Initstructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx ;//���պͷ���ģʽ
	USART_Initstructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;//�첽����ҪӲ��������
	USART_Init(USART1,&USART_Initstructure);//��ʼ������
	//�����ж�
NVIC_Configuration();
//ʹ�ܴ����ж�
USART_Cmd(USART1, ENABLE); 

}
//�жϷ�������
void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;//�жϽṹ��
	 	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	//ʹ�ܴ��ڽ����ж�
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	
	
}
//���͵����ַ�����
void Usart_SendByte(USART_TypeDef *pUSART,uint8_t ch)
{
	USART_SendData(USART1,ch);
	while( USART_GetITStatus(USART1,USART_IT_TXE)==RESET);//��ȡ�ж�״̬��־λ.�ȴ�������

}

