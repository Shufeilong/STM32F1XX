#include "key.h"
#include "delay.h"


/*������ʼ��*/
 void Key_Init()
	 
 {
	GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�ĳ�ʼ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; //KEY1 KEY0 ��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PB8,PB9
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
 
 
 }
 
 
 /*������⺯��*/
 u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		//else if(KEY2==0)return 3;
		else if(WK_UP==1)return 4;
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}
