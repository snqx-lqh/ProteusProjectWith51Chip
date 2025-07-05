#include "main.h"
#include "delay.h"
#include "LCD1602.h"
#include "key.h"
#include "dht11.h"
#include "usart.h"
// 变量定义
sbit reduce_temp = P2^0;      
sbit add_temp = P2^1;    
sbit reduce_hum = P2^2;    
sbit add_hum = P2^3;    
 
uint8_t  key_value = 0;   // 按键按下的值
uint8_t  hum_high  = 60, hum_low  = 30;//设置的湿度最大值和最小值
uint8_t  temp_high = 40, temp_low = 20;//设置的温度最大值和最小值
uint8_t humidity=0,      temperature=0;//实际的湿度值和温度值
uint8_t ui_page = 0;//实际的湿度值和温度值
uint8_t set_pos = 0;//设置序号 0：设置hum_high 1：设置hum_low 2：设置temp_high 3：设置temp_low
// 函数声明
void Timer0_Init(void);		//5毫秒@11.0592MHz

void main()
{
	uint8_t  ret;
	uint8_t  blink_count = 0;
	LcdInit();
	Timer0_Init();
	UartInit();
	
	EA = 1;                 //打开所有中断
	
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

// 检查当前温湿度的状态
// 如果温度大于设定值，LED1亮，如果温度小于设定值，LED2亮。
// 如果湿度大于设定值，LED3亮，如果湿度小于设定值，LED4亮。
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

// 5ms定时器中断，5MS执行一次
void Timer0Routine(void) interrupt 1
{
//	static int count = 0;
	// 5ms 定时值重置
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0xEE;				//设置定时初始值
	
	// 按键扫描并获得值
	key_value = KeyScan();
	// 按键0按下 
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
	// 按键1按下 
	else if(key_value == KEY1_PRES)
	{
		if(ui_page == 1)
		{
			if(set_pos == 0) hum_high--;
			else if(set_pos == 1) hum_low--;
			else if(set_pos == 2) temp_high--;
			else if(set_pos == 3) temp_low--;
		}
	}// 按键2按下 
	else if(key_value == KEY2_PRES)
	{
		if(ui_page == 0)
			ui_page = 1;
		else if(ui_page == 1)
			ui_page = 0;
	}// 按键3按下 
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


void Timer0_Init(void)		//5毫秒@11.0592MHz
{
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0xEE;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;                //打开定时器中断
}