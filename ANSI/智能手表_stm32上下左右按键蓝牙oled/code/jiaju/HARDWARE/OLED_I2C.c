/************************************************************************************
* Function List:
*	1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
* 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
* 4. void WriteDat(unsigned char I2C_Data) -- 写数据
* 5. void OLED_Init(void) -- OLED屏初始化
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
* 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
* 8. void OLED_CLS(void) -- 清屏
* 9. void OLED_ON(void) -- 唤醒
* 10. void OLED_OFF(void) -- 睡眠
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
*************************************************************************************/

#include "OLED_I2C.h"
#include "delay.h"
#include "codetab.h"
#include "stdio.h"

void WriteCmd(unsigned char I2C_Command)//写命令
{
	I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
	I2C_WriteByte(0x40, I2C_Data);
}


void I2C_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin	=	GPIO_Pin_8 | GPIO_Pin_9; 	//引脚
	GPIO_InitStruct.GPIO_Mode	=   GPIO_Mode_OUT; 	//输出模式
	GPIO_InitStruct.GPIO_OType	=	GPIO_OType_PP;  //推挽
	GPIO_InitStruct.GPIO_PuPd	=	GPIO_PuPd_UP;	//上拉
	GPIO_InitStruct.GPIO_Speed	=   GPIO_Speed_50MHz; //速度50MHZ
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//空闲状态
	SCL 	= 1;
	SDA_OUT = 1;
}

//数据线输入输出模式
void Iic_SDA_Mode(u8 mode)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin	=	GPIO_Pin_9; 	//引脚
	GPIO_InitStruct.GPIO_Mode	=   mode; 			//输出模式
	GPIO_InitStruct.GPIO_OType	=	GPIO_OType_PP;  //推挽
	GPIO_InitStruct.GPIO_PuPd	=	GPIO_PuPd_UP;	//上拉
	GPIO_InitStruct.GPIO_Speed	=   GPIO_Speed_50MHz; //速度50MHZ
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}


//IIC开始信号
void Iic_Start(void)
{
	//数据线输出模式
	Iic_SDA_Mode(GPIO_Mode_OUT);
	
	//空闲模式
	SCL 	= 1;
	SDA_OUT = 1;
	
	delay_us(5);
	//数据线拉低
	SDA_OUT = 0;
	
	delay_us(5);
	//SDA拉低之后，SCL再拉低，可钳住总线
	SCL 	= 0;	
}


//IIC停止信号
void Iic_Stop(void)
{
	//数据线输出模式
	Iic_SDA_Mode(GPIO_Mode_OUT);
	
	SCL 	= 0;
	SDA_OUT = 0;
	
	delay_us(5);
	//时钟拉高
	SCL = 1;
	
	delay_us(5);
	//SDA拉高， SCL及SDA都为高，总线这空闲状态
	SDA_OUT 	= 1;	
}

//等待应答信号，函数返回值：0，函数返回值说明为有效应答，函数返回值：1，函数返回值说明为无效应答，

u8 Iic_Wait_Ack(void)
{
	u8 ack = 0;
	//数据线输入模式
	Iic_SDA_Mode(GPIO_Mode_IN);	
	
	
	SCL = 1;
	delay_us(5);
	
	if(SDA_IN == 1)
	{
		ack = 1; 	//无效应答
		Iic_Stop(); //接收无效应答，则停止发送数据
	}
	else
	{
		ack = 0;
	}
	
	SCL = 0;
	delay_us(5);

	return ack;
}


u8 Iic_Send_Byte(u8 txd){ //0x54  0101 0100

	u8 i;
	
	//数据线输出模式
	Iic_SDA_Mode(GPIO_Mode_OUT);

	SCL = 0;
	delay_us(5);
	
	for(i=0; i<8; i++)
	{
		//依次拆分txd的位 从高位先拆
		
		if(  txd  & 1<<(7-i))
		{
			SDA_OUT = 1;
		}
		else
		{
			SDA_OUT = 0;
		}
		
	
		delay_us(5);
		
		SCL = 1;
		//延时，保持数据稳定
		delay_us(5);
		
		SCL = 0;
		
	}
	return  0;
}


void I2C_WriteByte(uint8_t addr,uint8_t data){

	u8 ack;
	
	Iic_Start();
	
	Iic_Send_Byte(OLED_ADDRESS);
	
	ack = Iic_Wait_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return;
	}
	Iic_Send_Byte(addr);
	ack = Iic_Wait_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return;
	}
	Iic_Send_Byte(data);	

	ack = Iic_Wait_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return;
	}
	
	Iic_Stop();
}




void OLED_Init(void){

	delay_ms(100); //这里的延时很重要
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0x80); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y){ //设置起始点坐标
 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data){//全屏填充{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void){//清屏

	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize){
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP_num)
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP1[BMP_num][j++]);
		}
	}
}
/*数字转字符串*/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

//void show_Time(int x){
//	char str[30]={0};
//	RTC_TimeTypeDef RTC_TimeStruct;
//	RTC_DateTypeDef RTC_DateStruct;
//	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
//	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
//	sprintf(str,"%02d",RTC_TimeStruct.RTC_Hours);
//	OLED_ShowStr(x,5,str,2);//测试6*8字符
//	OLED_ShowStr(x+17,5,":",2);//测试6*8字符
//	sprintf(str,"%02d",RTC_TimeStruct.RTC_Minutes);
//	OLED_ShowStr(x+25,5,str,2);//测试6*8字符
//	OLED_ShowStr(x+41,5,":",2);//测试6*8字符
//	sprintf(str,"%02d",RTC_TimeStruct.RTC_Seconds);
//	OLED_ShowStr(x+49,5,str,2);//测试6*8字符
//	
//	x-=7;
//	sprintf(str,"20%02d",RTC_DateStruct.RTC_Year);
//	OLED_ShowStr(x+2,3,str,2);//测试6*8字符
//	OLED_ShowStr(x+33,3,"/",2);//测试6*8字符
//	sprintf(str,"%02d",RTC_DateStruct.RTC_Month);
//	OLED_ShowStr(x+41,3,str,2);//测试6*8字符
//	OLED_ShowStr(x+57,3,"/",2);//测试6*8字符
//	sprintf(str,"%02d",RTC_DateStruct.RTC_Date);
//	OLED_ShowStr(x+65,3,str,2);//测试6*8字符

//}
