#include "main.h"
#include "delay.h"
#include "LCD1602.h"
#include "key.h"
#include "dht11.h"
#include "usart.h"
// ��������
sbit reduce_temp = P2^0;      
sbit add_temp = P2^1;    
sbit reduce_hum = P2^2;    
sbit add_hum = P2^3;    
 
uint8_t  key_value = 0;   // �������µ�ֵ
uint8_t  hum_high  = 60, hum_low  = 30;//���õ�ʪ�����ֵ����Сֵ
uint8_t  temp_high = 40, temp_low = 20;//���õ��¶����ֵ����Сֵ
uint8_t humidity=0,      temperature=0;//ʵ�ʵ�ʪ��ֵ���¶�ֵ
uint8_t ui_page = 0;//ʵ�ʵ�ʪ��ֵ���¶�ֵ
uint8_t set_pos = 0;//������� 0������hum_high 1������hum_low 2������temp_high 3������temp_low
// ��������
void Timer0_Init(void);		//5����@11.0592MHz

void main()
{
	uint8_t  ret;
	uint8_t  blink_count = 0;
	LcdInit();
	Timer0_Init();
	UartInit();
	
	EA = 1;                 //�������ж�
	
	SendBuffLen("hello world\r\n",sizeof("hello world\r\n"));
	
	while(1)
	{	
		EA = 0;
		ret = DHT11_Get_Data(&humidity, &temperature) ;
		EA = 1;
		if(ui_page == 0)
		{
			LcdShowStr(0,0,"  real  data  ");
			LcdShowStr(0,1,"hum:");
			LcdShowNumInt(4,1,humidity,2,1);
			LcdShowStr(6,1,"   ");
			LcdShowStr(9,1,"temp:");
			LcdShowNumInt(14,1,temperature,2,1);
		}else if(ui_page == 1)
		{
			blink_count ++;
			LcdShowStr(0,0,"HumH:");
			LcdShowNumInt(5,0,hum_high,2,1);
			if(set_pos == 0 && (blink_count % 2 == 0) ){
				LcdShowStr(5,0,"  ");
			}
			LcdShowStr(7,0,"  ");
			LcdShowStr(9,0,"HumL:");
			LcdShowNumInt(14,0,hum_low,2,1);
			if(set_pos == 1 && (blink_count % 2 == 0) ){
				LcdShowStr(14,0,"  ");
			}
			LcdShowStr(0,1,"TmpH:");
			LcdShowNumInt(5,1,temp_high,2,1);
			if(set_pos == 2 && (blink_count % 2 == 0) ){
				LcdShowStr(5,1,"  ");
			}
			LcdShowStr(7,1,"  ");
			LcdShowStr(9,1,"TmpL:");
			LcdShowNumInt(14,1,temp_low,2,1);
			if(set_pos == 3 && (blink_count % 2 == 0) ){
				LcdShowStr(14,1,"  ");
			}
		}
		delay_ms(200);
	}
}

// ��鵱ǰ��ʪ�ȵ�״̬
// ����¶ȴ����趨ֵ��LED1��������¶�С���趨ֵ��LED2����
// ���ʪ�ȴ����趨ֵ��LED3�������ʪ��С���趨ֵ��LED4����
void CheckLimits()
{
	if(temperature > temp_high)
	{
		reduce_temp = 0;add_temp = 1;
	}else if(temperature < temp_low)
	{
		reduce_temp = 1;add_temp = 0;
	}else  
	{
		reduce_temp = 1;add_temp = 1;
	}
	
	if(humidity > hum_high)
	{
		reduce_hum = 0;add_hum = 1;
	}else if(humidity < hum_low)
	{
		reduce_hum = 1;add_hum = 0;
	}else  
	{
		reduce_hum = 1;add_hum = 1;
	}
}

// 5ms��ʱ���жϣ�5MSִ��һ��
void Timer0Routine(void) interrupt 1
{
//	static int count = 0;
	// 5ms ��ʱֵ����
	TL0 = 0x00;				//���ö�ʱ��ʼֵ
	TH0 = 0xEE;				//���ö�ʱ��ʼֵ
	
	// ����ɨ�貢���ֵ
	key_value = KeyScan();
	// ����0���� 
	if(key_value == KEY0_PRES)
	{
		if(ui_page == 1)
		{
			if(set_pos == 0) hum_high++;
			else if(set_pos == 1) hum_low++;
			else if(set_pos == 2) temp_high++;
			else if(set_pos == 3) temp_low++;
		}
	}
	// ����1���� 
	else if(key_value == KEY1_PRES)
	{
		if(ui_page == 1)
		{
			if(set_pos == 0) hum_high--;
			else if(set_pos == 1) hum_low--;
			else if(set_pos == 2) temp_high--;
			else if(set_pos == 3) temp_low--;
		}
	}// ����2���� 
	else if(key_value == KEY2_PRES)
	{
		if(ui_page == 0)
			ui_page = 1;
		else if(ui_page == 1)
			ui_page = 0;
	}// ����3���� 
	else if(key_value == KEY3_PRES)
	{
		if(ui_page == 1)
		{
			set_pos ++;
			if(set_pos > 3)
				set_pos = 0;
		}
	}
	
	CheckLimits();
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