#include "adc.h"
#include "show.h"
#include "Uart.h"
#include "OLED_I2C.h"
#include "delay.h"
#include "stdio.h"
#include "key.h"
extern char Key;
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_CommonInitTypeDef  ADC_CommonInitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	//必须先初始化时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   =   GPIO_Pin_7;//PF7
	GPIO_InitStruct.GPIO_Mode  =   GPIO_Mode_AN;//模拟模式
	GPIO_InitStruct.GPIO_OType =   GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_PuPd  =   GPIO_PuPd_UP;//上拉
	GPIO_InitStruct.GPIO_Speed =   GPIO_Fast_Speed;//高速   50MHz
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//复位ADC
	ADC_DeInit();
	
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//关闭DMA
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//最好ADC不要超过35MHz  84/4=21MHz
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样周期延时5个钟
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//选择数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//关闭外部触发
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//12位精度
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//关闭连续扫描
	ADC_Init(ADC3,&ADC_InitStruct);
	//adc1外设使能
	ADC_Cmd(ADC3,ENABLE);
	//规则通道配置
	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,1,ADC_SampleTime_3Cycles);
}
uint16_t get_value(void)
{
	uint16_t value=0;
	//开启转换
	ADC_SoftwareStartConv(ADC3);
	//等待转换结束
	while(ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC)==0);
	//获取转换后的值
	value = ADC_GetConversionValue(ADC3);
}

//光强调用
void show_adc(){
	int i;
	char str[20];
	char x=24;
	int num_adc=0;
	OLED_CLS();
	while(1){
		num_adc=get_value();
		show_Time(87,0,Time_time);
		Button_check_all();
		OLED_ShowCN(x+8,3,3);
		OLED_ShowCN(x+24,3,4);
		OLED_ShowStr(x+40,3,":",2);
		sprintf((unsigned char *)str,"%04d",num_adc);
		OLED_ShowStr(x+48,3,(unsigned char *)str,2);
		if(num_adc<150){
			for(i=0;i<8;i++){
				OLED_ShowCN(i*16,5,21+i);			
			}
		}
		Usart_text();
		delay_s(1);
		if(Key==4)
			break;
		//OLED_ShowStr(10,2,str,2);
	
	}
	OLED_CLS();
}

