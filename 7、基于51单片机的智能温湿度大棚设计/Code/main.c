#include "main.h"

#include "LCD1602.h" 
#include "keyScan.h"  
#include "bsp_timer.h"
#include "bsp_uart.h"

#include "dht11.h"

// ��������
sbit reduce_temp = P2^3;    //���ƽ��»�  
sbit add_temp    = P2^2;    //�������»�
sbit reduce_hum  = P2^1;    //���Ƽ�ʪ��
sbit add_hum     = P2^0;    //���Ƽ�ʪ��

sbit LED = P1^7;            //������
 
uint8_t  g_key_value = 0;                // �������µ�ֵ
uint8_t  hum_high  = 30,  hum_low     = 25;//���õ�ʪ�����ֵ����Сֵ
uint8_t  temp_high = 35,  temp_low    = 25;//���õ��¶����ֵ����Сֵ
uint8_t  humidity  = 0 ,  temperature =  0;//ʵ�ʵ�ʪ��ֵ���¶�ֵ
uint8_t  ui_page   = 0 ;                //��ʾҳ��
uint8_t  set_pos   = 0 ;                //������� 0������hum_high 1������hum_low 2������temp_high 3������temp_low

uint8_t  read_dht11_flag = 0;           //��ȡDHT11�ı�־λ��
uint8_t  ui_refresh_flag = 0;           //����ˢ�µı�־λ��

void main()
{
	uint8_t  ret;
	uint8_t  blink_count = 0;
	uint8_t  lcdShowBuff[16] = {0};
	
	EA = 0;                 //�ر������ж�
	LcdInit();            // LCD��ʾ��ʼ��
	key_init();
	LED = 0;
	bsp_timer_set_ms_IT(Timer0,5);
	Uart1_Init();
	EA = 1;                 //�������ж� 
	
	while(1)
	{
		if(read_dht11_flag == 1)
		{
			EA = 0;
			ret = DHT11_Get_Data(&humidity, &temperature) ;
			EA = 1;
			printf("hum :%2d  temp:%2d\r\n",(int)humidity,(int)temperature); //����һ����ʪ��
			read_dht11_flag = 0;
		}
		
		if( ui_refresh_flag == 1)
		{
			if(ui_page == 0)
			{
				sprintf((char*)lcdShowBuff,"  real  data    ");
				LcdShowStr(0, 0, lcdShowBuff);
				sprintf((char*)lcdShowBuff,"hum :%2d  temp:%2d",(int)humidity,(int)temperature);
				LcdShowStr(0, 1, lcdShowBuff);
			}else if(ui_page == 1)
			{
				blink_count ++;
				
				sprintf((char*)lcdShowBuff,"HumH:%2d  HumL:%2d",(int)hum_high, (int)hum_low);
				LcdShowStr(0, 0, lcdShowBuff);
				sprintf((char*)lcdShowBuff,"TmpH:%2d  TmpL:%2d",(int)temp_high,(int)temp_low);
				LcdShowStr(0, 1, lcdShowBuff);
				
				// ����ֵΪż����ʱ����˸
				if(blink_count % 2 == 0)
				{
					switch(set_pos)
					{
						case 0:LcdShowStr(5,0,"  ");break;
						case 1:LcdShowStr(14,0,"  ");break;
						case 2:LcdShowStr(5,1,"  ");break;
						case 3:LcdShowStr(14,1,"  ");break;
					}
				}
			}
			ui_refresh_flag = 0;
		}
	}
}

/**
  * @brief   ���ݰ���ֵ������ǰ�Ĺ��ܴ���
  * @param    
  * @retval  void
 **/
void KeyControlFunction(uint8_t key_value)
{
	if(key_value == KEY1_VALUE)
	{
		if(ui_page == 1)
		{
			if(set_pos == 0) hum_high++;
			else if(set_pos == 1) hum_low++;
			else if(set_pos == 2) temp_high++;
			else if(set_pos == 3) temp_low++;
		}
	}
	else if(key_value == KEY2_VALUE)
	{
		if(ui_page == 1)
		{
			if(set_pos == 0) hum_high--;
			else if(set_pos == 1) hum_low--;
			else if(set_pos == 2) temp_high--;
			else if(set_pos == 3) temp_low--;
		}
	}
	else if(key_value == KEY3_VALUE)
	{
		if(ui_page == 0)
			ui_page = 1;
		else if(ui_page == 1)
			ui_page = 0;
	}
	else if(key_value == KEY4_VALUE)
	{
		if(ui_page == 1)
		{
			set_pos ++;
			if(set_pos > 3)
				set_pos = 0;
		}
	}
}

/**
  * @brief   ��鵱ǰ��ʪ�ȵ�״̬
  *          ����¶ȴ����趨ֵ�����Ƽ������̵����򿪣�����¶�С���趨ֵ�������������̵����򿪡�
  *          ���ʪ�ȴ����趨ֵ�����Ƽ�ʪ���̵����򿪣����ʪ��С���趨ֵ�����Ƽ�ʪ���̵����򿪡�
  * @param    
  * @retval  void
 **/
void ControlRelay()
{
	if(temperature > temp_high)
	{
		reduce_temp = 1;add_temp = 0;
	}else if(temperature < temp_low)
	{
		reduce_temp = 0;add_temp = 1;
	}else  
	{
		reduce_temp = 0;add_temp = 0;
	}
	
	if(humidity > hum_high)
	{
		reduce_hum = 1;add_hum = 0;
	}else if(humidity < hum_low)
	{
		reduce_hum = 0;add_hum = 1;
	}else  
	{
		reduce_hum = 0;add_hum = 0;
	}
}



void Timer0Routine(void) interrupt 1
{
	static uint16_t timerCount = 0;
// 	timer_reload_ms(Timer0,5); //�ⷽʽ�������ڼ�ʱӰ��ϴ�
	
	TL0 = 0x00;				//���ö�ʱ��ʼֵ 5ms
	TH0 = 0xEE;				//���ö�ʱ��ʼֵ
	
	timerCount++;
	
	g_key_value = key_scan(5);
	
	// ���㵽200ȡ�� ���������൱�� 200��5 == 1000ms ����1s
	if( timerCount % 200 == 0){
		LED = !LED;
		read_dht11_flag = 1; //��ȡDHT11�ı�־��1����ʾ���Կ�ʼ��DHT11
	}
	// ���㵽20ȡ�� ���������൱�� 20��5 == 100ms  
	if( timerCount % 20 == 0){
		ui_refresh_flag = 1; //�����ˢ��
	}

	KeyControlFunction(g_key_value);
	ControlRelay();
}

void UartRoutine(void) interrupt 4
{
	uint8_t res;	
	if(RI)
	{
		res = SBUF;
		RI = 0;
	}	
}




