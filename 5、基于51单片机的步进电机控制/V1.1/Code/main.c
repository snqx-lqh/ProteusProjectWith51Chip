#include "reg52.h"
#include "step_motor.h"
#include "bsp_key.h"
#include "LCD1602.h"
#include "stdio.h"

void Timer0_Init(void);
 
u8  MotorRunMode = STOP;   //步进电机转动状态
int StepMs       = 100;    //步进电机转动一步进的毫秒数

int main()
{
	u8 keyValue = 0;
	u8 lcdShowBuff[16] = {0};
	
	EA = 0;   // 失能全部中断
    
	Timer0_Init();
	KeyInit();
	LcdInit();
	
	sprintf((char*)lcdShowBuff,"Mode : STOP  ");
	LcdShowStr(0,0,lcdShowBuff);
	sprintf((char*)lcdShowBuff,"StepMs : %4d  ",StepMs);
	LcdShowStr(0,1,lcdShowBuff);
	
	EA = 1;   // 使能全部中断
    while (1)
    {
		
		keyValue = KeyScan(0);
		if( keyValue == 1 )
		{
			MotorRunMode = CW;
			sprintf((char*)lcdShowBuff,"Mode : CW  ");
			LcdShowStr(0,0,lcdShowBuff);
		}else if( keyValue == 2 )
		{
			MotorRunMode = CCW;
			sprintf((char*)lcdShowBuff,"Mode : CCW ");
			LcdShowStr(0,0,lcdShowBuff);
		}else if( keyValue == 3 )
		{
			StepMs += 100;
			if(StepMs > 1000)
				StepMs = 1000;
			sprintf((char*)lcdShowBuff,"StepMs : %4d  ",StepMs);
			LcdShowStr(0,1,lcdShowBuff);
		}else if( keyValue == 4 )
		{
			StepMs -= 100;
			if(StepMs < 0)
				StepMs = 0;
			sprintf((char*)lcdShowBuff,"StepMs : %4d  ",StepMs);
			LcdShowStr(0,1,lcdShowBuff);
		}	
    }
}

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;                //使能定时器0
}

// 1ms定时器中断
void Timer0() interrupt 1
{
    static int counts = 0;

    TL0 = 0x18; //设置定时0初始值
    TH0 = 0xFC; //设置定时0初始值
	
    counts++;
    if (counts > StepMs) // 定时大于指定MS数的时候，运动一步进
    {
        MotorTurn(MotorRunMode); //转动一步进
        counts = 0;
    }
}

