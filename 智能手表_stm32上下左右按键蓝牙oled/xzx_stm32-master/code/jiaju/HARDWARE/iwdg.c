#include "iwdg.h"

void Iwdg_Init(void)
{
	//ȥ��д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//������д��0x5555
	
	//����Ԥ��Ƶ��   ԭ��ʹ�õ�ʱ����LSI 32KHz
	IWDG_SetPrescaler(IWDG_Prescaler_4);//32KHz/4=8KHz  8000--->1s 8--->1ms
	
	
	IWDG_SetReload(80);//ι��ʱ�� 10ms   ���ֵ����0 - 4096
	
	IWDG_Enable();//�����������Ź�      ������д��0xCCCC
	
	IWDG_ReloadCounter();//ʹ����װ�ؼ�����  ������д��0xAAAA
}
