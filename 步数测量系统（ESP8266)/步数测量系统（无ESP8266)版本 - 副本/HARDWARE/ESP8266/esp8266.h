#ifndef _ESP8266_H
#define _ESP8266_H
#include "delay.h"
#include "sys.h"
#define Ssid  "mawei"											//WIFI���ƣ����޸�Ϊ�Լ�·������WIFI���ƣ�����֧������
#define PassWord  "12345678"              //WIFI���룬�޸�Ϊ�Լ�·������
#define Uid  "bfcf65510b5a4895a30b449eaaeb4b34" //�ͷ���UID��Կ������̨��ȡ
#define Topic  "MPU6050"          //�ͷ��ƿ���̨�����������Զ��壬app��������Ҫ�������������һ��
extern	unsigned char Receive_shuju[150];
extern	unsigned char Okword;
extern  char char_step[4],char_caloreis[6],char_sporttime[4];//��������·��˶�ʱ��

void usart3_init(u32 bound);
void POST(char data1[] ,char data2[],char data3[],char data4[]);//POST���ݺ���
void  ESP8266_Set_ok(USART_TypeDef* USARTx, char *str);
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data);
void ESP8266_Set_Only(USART_TypeDef* USARTx, char *str);
void  ESP8266_Set(USART_TypeDef* USARTx, char *str);
void Ping(void);
void connect_topic(void);
void connect_bemfa(void);
void connect_fasong(char data1[],char data2[],char data3[],char data4[]	);
#endif

