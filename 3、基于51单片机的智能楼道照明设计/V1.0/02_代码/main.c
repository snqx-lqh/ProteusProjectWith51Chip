#include "main.h"
#include "delay.h"
#include "i2c.h"
#include "pcf8591.h"
#include "LCD1602.h"
#include "key.h"

// ��������
sbit LED = P2^0;          // ¥����
sbit MAN = P2^1;          // ������⴫���� ���˵�ʱ���ź�Ϊ0
uint8_t  key_value = 0;   // �������µ�ֵ
uint8_t  adcx      = 0;   // ��ǰ��ADC��⵽��ֵ
uint8_t  run_mode  = 0;   // ����ģʽ 0 ���Զ�ģʽ  1���ֶ�ģʽ

// ��������
void Timer0_Init(void);		//5����@11.0592MHz

void main()
{
	IIC_init();
	LcdInit();
	Timer0_Init();
	
	EA = 1;                 //�������ж�
	
	while(1)
	{
		LcdShowStr(0,0,"RUN Mode:");
		LcdShowNumInt(9,0,run_mode,1,0);
		delay_ms(5);
	}
}

// 5ms��ʱ���жϣ�5MSִ��һ��
void Timer0Routine(void) interrupt 1
{
	static int count = 0;
	// 5ms ��ʱֵ����
	TL0 = 0x00;				//���ö�ʱ��ʼֵ
	TH0 = 0xEE;				//���ö�ʱ��ʼֵ
	
	// ����ɨ�貢���ֵ
	key_value = KeyScan();
	// ����0�����л�����ģʽ
	if(key_value == KEY0_PRES)
	{
		if(run_mode == 0) // �������Զ�ģʽ�����л����ֶ�ģʽ
			run_mode = 1;
		else if(run_mode == 1) // �������ֶ�ģʽ�����л����Զ�ģʽ
			run_mode = 0;
	}
	// ����1���£�������ֶ�ģʽ�Ϳ��ص�
	else if(key_value == KEY1_PRES)
	{
		if(run_mode == 1)
		{
			LED = !LED;
		}
	}
	// ������Զ�ģʽ����ȡ���������ѹ��ֵ��Ȼ����ݶ�������ֵ���Ƿ��⵽�˿��ص�
	if(run_mode == 0)
	{
		// ��ȡ��������͵����ѹֵ
		adcx = pcf_analog_read(0);
		if(adcx > 128 && MAN == 0)
			LED = 0;
		else
			LED = 1;
	}
}


void Timer0_Init(void)		//5����@11.0592MHz
{
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x01;			//���ö�ʱ��ģʽ
	TL0 = 0x00;				//���ö�ʱ��ʼֵ
	TH0 = 0xEE;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;                //�򿪶�ʱ���ж�
}