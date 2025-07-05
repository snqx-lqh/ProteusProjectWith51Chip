#include "hc_sr04.h"
#include "intrins.h"

u16 flag=0;

void HCSR04_Init()
{
	Trig = 0;
	Echo = 1;
}

//��ʱ��0��ʼ��   
void HCSR04_Timer0Init()
{
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x01;			//���ö�ʱ��ģʽ
	TL0 = 0x00;				//���ö�ʱ��ʼֵ
	TH0 = 0x00;				//���ö�ʱ��ʼֵ
	TF0 = 1;				//���TF0��־
	TR0 = 0;				 
	ET0 = 1;                //�򿪶�ʱ���ж�
	EA = 1;                 //�������ж�
}

void  HCSR04_trigSendOnePulse() 		         //����ģ��
{
	Trig=1;			                     //����һ��ģ��
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	Trig=0;
}

//������볬���������ƣ��ͼ�һ�������ļ�������
void Timer0() interrupt 1
{
	flag++;
}

//���㺯��
float DistCount()
{
	float dist = 0;
	unsigned long HCSR04_countTime = 0;
	
	while(!Echo);
	TR0=1;
	while(Echo);
	TR0=0;
	HCSR04_countTime = flag*65535+TH0*256+TL0;	
	TH0=0;
	TL0=0;
	flag = 0;
	dist=(float)(HCSR04_countTime*0.017);

	return dist;
}
