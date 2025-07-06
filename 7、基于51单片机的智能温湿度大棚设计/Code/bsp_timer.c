#include "bsp_timer.h"

/**
  * @brief   定时器中断初始化函数，设置定时时间（单位：微秒）
  *          还未测试其鲁棒性，建议11.0592MHZ下不超过50000us。理论该频率能到71ms
  * @param   Timer：Timer0 Timer1   xus：定时时间长度
  * @retval  void
 **/
void bsp_timer_set_us_IT(Timer_t Timer, uint16_t xus) 
{
    uint16_t reload_value;  // 用于存储定时器初值
	
    reload_value = 65536 - (uint16_t)(SOC_FREQ * xus / 12.0f);

    // 设置定时器模式
	if(Timer == Timer0){
		TMOD &= 0xF0;  // 清除低四位，保留高四位的定时器1模式
		TMOD |= 0x01;  // 设置定时器0为模式1（16位定时器模式）
		 // 将定时器初值分别写入低8位和高8位
		TL0 = (uint8_t)(reload_value & 0xFF);  // 低8位
		TH0 = (uint8_t)(reload_value >> 8);   // 高8位
		
		TF0 = 0;// 清除TF0标志		
		TR0 = 1;// 启动定时器0
		ET0 = 1;// 打开定时器0中断
	}else if(Timer == Timer1){
		TMOD &= 0x0F;  // 清除高四位，保留低四位的定时器0模式
		TMOD |= 0x10;  // 设置定时器1为模式1（16位定时器模式）
		TL1 = (uint8_t)(reload_value & 0xFF);  // 低8位
		TH1 = (uint8_t)(reload_value >> 8);   // 高8位
		
		TF1 = 0;// 清除TF0标志		
		TR1 = 1;// 启动定时器0
		ET1 = 1;// 打开定时器0中断
	}
}

/**
  * @brief   定时器中断初始化函数，设置定时时间（单位：毫秒）
  *          还未测试其鲁棒性，建议11.0592MHZ下不超过50ms。理论该频率能到71ms
  * @param   Timer：Timer0 Timer1   xms：定时时间长度 
  * @retval  void
 **/
void bsp_timer_set_ms_IT(Timer_t Timer, uint16_t xms) 
{
	bsp_timer_set_us_IT(Timer, xms*1000);
}

/**
  * @brief   重新设置单片机的TH和TL寄存器，使得重新定时的时长，和定义的一样。
  * @param   Timer：Timer0 Timer1   xus：定时时间长度 
  * @retval  void
 **/
void timer_reload_us(Timer_t Timer, uint16_t xus)
{
	uint16_t reload_value;  // 用于存储定时器初值
	
    reload_value = 65536 - (uint16_t)(SOC_FREQ * xus / 12.0f);

    // 设置定时器模式
	if(Timer == Timer0){
		 // 将定时器初值分别写入低8位和高8位
		TL0 = (uint8_t)(reload_value & 0xFF);  // 低8位
		TH0 = (uint8_t)(reload_value >> 8);   // 高8位
	}else if(Timer == Timer1){
		TL1 = (uint8_t)(reload_value & 0xFF);  // 低8位
		TH1 = (uint8_t)(reload_value >> 8);   // 高8位
	}
}

/**
  * @brief   重新设置单片机的TH和TL寄存器，使得重新定时的时长，和定义的一样。
  * @param   Timer：Timer0 Timer1   xms：定时时间长度 
  * @retval  void
 **/
void timer_reload_ms(Timer_t Timer, uint16_t xms)
{
	timer_reload_us(Timer, xms * 1000);
}

//void Timer0Routine(void) interrupt 1 {}
//void Timer1Routine(void) interrupt 3 {}

