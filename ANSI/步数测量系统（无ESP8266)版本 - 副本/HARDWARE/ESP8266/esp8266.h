#ifndef _ESP8266_H
#define _ESP8266_H
#include "delay.h"
#include "sys.h"
#define Ssid  "mawei"											//WIFI名称，，修改为自己路由器的WIFI名称，好像不支持中文
#define PassWord  "12345678"              //WIFI密码，修改为自己路由器的
#define Uid  "bfcf65510b5a4895a30b449eaaeb4b34" //巴法云UID密钥，控制台获取
#define Topic  "MPU6050"          //巴法云控制台创建，名称自定义，app订阅主题要和这个推送主题一致
extern	unsigned char Receive_shuju[150];
extern	unsigned char Okword;
extern  char char_step[4],char_caloreis[6],char_sporttime[4];//步数，卡路里，运动时间

void usart3_init(u32 bound);
void POST(char data1[] ,char data2[],char data3[],char data4[]);//POST数据函数
void  ESP8266_Set_ok(USART_TypeDef* USARTx, char *str);
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data);
void ESP8266_Set_Only(USART_TypeDef* USARTx, char *str);
void  ESP8266_Set(USART_TypeDef* USARTx, char *str);
void Ping(void);
void connect_topic(void);
void connect_bemfa(void);
void connect_fasong(char data1[],char data2[],char data3[],char data4[]	);
#endif

