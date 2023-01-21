#include "dht11.h"
#include "sys.h"
#include "delay.h"

unsigned char buff,cheak;
unsigned char th_data[4]={0};

GPIO_InitTypeDef GPIO_InitStruct;

void Dht11_Init(void)
{
	//必须先初始化时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin =     GPIO_Pin_9;//PG9
	GPIO_InitStruct.GPIO_Mode =    GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd =    GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOG,&GPIO_InitStruct);	
}

/**************传入参数确定是输出还是输入***********************/
void change_mode(GPIOMode_TypeDef mode)
{
	GPIO_InitStruct.GPIO_Mode =  mode;//模式
	GPIO_Init(GPIOG,&GPIO_InitStruct);	
}

/***************启动初始化***************************************/
int start_dht11(void)
{
	//1.设置为输入模式开始空闲状态为高电平
	change_mode(GPIO_Mode_OUT);
	PGout(9)=1;
	delay_us(5);
	//2.拉低不小于18ms
	PGout(9)=0;
	delay_ms(20);
	//3.拉高在 20-40 us之间
	PGout(9)=1;
	delay_us(30);
	//4.转换为输入模式
	change_mode(GPIO_Mode_IN);
	//5.两个跳变
	if(!PGin(9))//if(PGin(9)==0)
	{
		while(!PGin(9));//由低到高的跳变
		while(PGin(9));//由高到低的跳变
		return 1;
	}
	return 0;
}

/************获取8位数据*******************************************/
void get_8bit(void)
{
	int i=0;
	for(i=0;i<8;i++)
	{
		buff = buff<<1;
		
		while(!PGin(9));//正式进入高电平
		delay_us(30);
		
		if(PGin(9))//如果30us过后还是高电平，就是数据 1
		{
			buff |= 0x1;
			while(PGin(9));
		}
		else//如果30us过后是低电平，就是数据 0
		{
			buff &= 0xfe;
		}	
	}
}

/*************获取温湿度******************************************/
int get_dht11_data(void)
{
	u8 add;
	if(start_dht11())
	{
		get_8bit();
		th_data[0]=buff;//获取的是湿度的整数
		
		get_8bit();//获取的是湿度的小数
		th_data[1]=buff;
		
		get_8bit();//获取的是温度的整数
		th_data[2]=buff;
		
		get_8bit();//获取的是温度的小数
		th_data[3]=buff;
		
		get_8bit();//检验和
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
	//写现实
	
	//写显示结束
	printf("temp:%d  humi:%d\n",temp,humi);

}
