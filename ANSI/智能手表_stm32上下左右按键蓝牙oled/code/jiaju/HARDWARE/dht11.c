#include "dht11.h"
#include "sys.h"
#include "delay.h"

unsigned char buff,cheak;
unsigned char th_data[4]={0};

GPIO_InitTypeDef GPIO_InitStruct;

void Dht11_Init(void)
{
	//�����ȳ�ʼ��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_9;//PG9
	GPIO_InitStruct.GPIO_Mode =    GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd =    GPIO_PuPd_UP;//����
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//����   50MHz
	GPIO_Init(GPIOG,&GPIO_InitStruct);	
}

/**************�������ȷ���������������***********************/
void change_mode(GPIOMode_TypeDef mode)
{
	GPIO_InitStruct.GPIO_Mode =  mode;//ģʽ
	GPIO_Init(GPIOG,&GPIO_InitStruct);	
}

/***************������ʼ��***************************************/
int start_dht11(void)
{
	//1.����Ϊ����ģʽ��ʼ����״̬Ϊ�ߵ�ƽ
	change_mode(GPIO_Mode_OUT);
	PGout(9)=1;
	delay_us(5);
	//2.���Ͳ�С��18ms
	PGout(9)=0;
	delay_ms(20);
	//3.������ 20-40 us֮��
	PGout(9)=1;
	delay_us(30);
	//4.ת��Ϊ����ģʽ
	change_mode(GPIO_Mode_IN);
	//5.��������
	if(!PGin(9))//if(PGin(9)==0)
	{
		while(!PGin(9));//�ɵ͵��ߵ�����
		while(PGin(9));//�ɸߵ��͵�����
		return 1;
	}
	return 0;
}

/************��ȡ8λ����*******************************************/
void get_8bit(void)
{
	int i=0;
	for(i=0;i<8;i++)
	{
		buff = buff<<1;
		
		while(!PGin(9));//��ʽ����ߵ�ƽ
		delay_us(30);
		
		if(PGin(9))//���30us�����Ǹߵ�ƽ���������� 1
		{
			buff |= 0x1;
			while(PGin(9));
		}
		else//���30us�����ǵ͵�ƽ���������� 0
		{
			buff &= 0xfe;
		}	
	}
}

/*************��ȡ��ʪ��******************************************/
int get_dht11_data(void)
{
	u8 add;
	if(start_dht11())
	{
		get_8bit();
		th_data[0]=buff;//��ȡ����ʪ�ȵ�����
		
		get_8bit();//��ȡ����ʪ�ȵ�С��
		th_data[1]=buff;
		
		get_8bit();//��ȡ�����¶ȵ�����
		th_data[2]=buff;
		
		get_8bit();//��ȡ�����¶ȵ�С��
		th_data[3]=buff;
		
		get_8bit();//�����
		cheak=buff;
		
		add = th_data[0]+th_data[1]+th_data[2]+th_data[03];
				
	}
	if(cheak == add)
	{
		return *(int *)th_data;
	}
	return 0;
}

void show_Dht(){
	uint32_t Data=0;
	int temp,humi=0;
	Data = get_dht11_data();
	humi = Data & 0xff;
	temp = (Data>>16) & 0xff;
	if(humi==0 || temp==0)
		return;
	//д��ʵ
	
	//д��ʾ����
	printf("temp:%d  humi:%d\n",temp,humi);

}
