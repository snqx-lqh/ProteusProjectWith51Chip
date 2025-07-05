#include "hc_sr04.h"
#include "intrins.h"

u16 flag=0;

void HCSR04_Init()
{
	Trig = 0;
	Echo = 1;
}

//定时器0初始化   
void HCSR04_Timer0Init()
{
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0x00;				//设置定时初始值
	TF0 = 1;				//清除TF0标志
	TR0 = 0;				 
	ET0 = 1;                //打开定时器中断
	EA = 1;                 //打开所有中断
}

void  HCSR04_trigSendOnePulse() 		         //启动模块
{
	Trig=1;			                     //启动一次模块
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	Trig=0;
}

//如果距离超出计数限制，就加一个完整的计数周期
void Timer0() interrupt 1
{
	flag++;
}

//计算函数
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
