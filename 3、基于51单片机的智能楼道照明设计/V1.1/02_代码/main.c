#include "main.h"
#include "delay.h"
#include "i2c.h"
#include "pcf8591.h"
#include "LCD1602.h"
#include "key.h"

// 变量定义
sbit LED = P2^0;          // 楼道灯
sbit MAN = P2^1;          // 人体红外传感器 有人的时候信号为0
uint8_t  key_value = 0;   // 按键按下的值
uint8_t  adcx      = 0;   // 当前的ADC检测到的值
uint8_t  run_mode  = 0;   // 运行模式 0 ：自动模式  1：手动模式

// 函数声明
void Timer0_Init(void);		//5毫秒@11.0592MHz

void main()
{
	IIC_init();
	LcdInit();
	Timer0_Init();
	
	EA = 1;                 //打开所有中断
	
	while(1)
	{
		LcdShowStr(0,0,"RUN Mode:");
		LcdShowNumInt(9,0,run_mode,1,0);
		delay_ms(5);
	}
}

// 5ms定时器中断，5MS执行一次
void Timer0Routine(void) interrupt 1
{
	static int count = 0;
	// 5ms 定时值重置
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0xEE;				//设置定时初始值
	
	// 按键扫描并获得值
	key_value = KeyScan();
	// 按键0按下切换工作模式
	if(key_value == KEY0_PRES)
	{
		if(run_mode == 0) // 假如是自动模式，就切换到手动模式
			run_mode = 1;
		else if(run_mode == 1) // 假如是手动模式，就切换到自动模式
			run_mode = 0;
	}
	// 按键1按下，如果在手动模式就开关灯
	else if(key_value == KEY1_PRES)
	{
		if(run_mode == 1)
		{
			LED = !LED;
		}
	}
	// 如果在自动模式，读取光敏电阻分压的值，然后根据读到得阈值和是否检测到人开关灯
	if(run_mode == 0)
	{
		// 读取光敏电阻和电阻分压值
		adcx = pcf_analog_read(0);
		if(adcx > 128 && MAN == 0)
			LED = 0;
		else
			LED = 1;
	}
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