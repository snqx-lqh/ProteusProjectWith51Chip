#include "main.h"

#include "LCD1602.h" 
#include "keyScan.h"  
#include "bsp_timer.h"
#include "bsp_uart.h"

#include "dht11.h"

// 变量定义
sbit reduce_temp = P2^3;    //控制降温机  
sbit add_temp    = P2^2;    //控制升温机
sbit reduce_hum  = P2^1;    //控制减湿器
sbit add_hum     = P2^0;    //控制加湿器

sbit LED = P1^7;            //心跳灯
 
uint8_t  g_key_value = 0;                // 按键按下的值
uint8_t  hum_high  = 30,  hum_low     = 25;//设置的湿度最大值和最小值
uint8_t  temp_high = 35,  temp_low    = 25;//设置的温度最大值和最小值
uint8_t  humidity  = 0 ,  temperature =  0;//实际的湿度值和温度值
uint8_t  ui_page   = 0 ;                //显示页面
uint8_t  set_pos   = 0 ;                //设置序号 0：设置hum_high 1：设置hum_low 2：设置temp_high 3：设置temp_low

uint8_t  read_dht11_flag = 0;           //读取DHT11的标志位。
uint8_t  ui_refresh_flag = 0;           //界面刷新的标志位。

void main()
{
	uint8_t  ret;
	uint8_t  blink_count = 0;
	uint8_t  lcdShowBuff[16] = {0};
	
	EA = 0;                 //关闭所有中断
	LcdInit();            // LCD显示初始化
	key_init();
	LED = 0;
	bsp_timer_set_ms_IT(Timer0,5);
	Uart1_Init();
	EA = 1;                 //打开所有中断 
	
	while(1)
	{
		if(read_dht11_flag == 1)
		{
			EA = 0;
			ret = DHT11_Get_Data(&humidity, &temperature) ;
			EA = 1;
			printf("hum :%2d  temp:%2d\r\n",(int)humidity,(int)temperature); //发送一次温湿度
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
				
				// 计数值为偶数的时候闪烁
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
  * @brief   根据按键值决定当前的功能处理
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
  * @brief   检查当前温湿度的状态
  *          如果温度大于设定值，控制减温器继电器打开，如果温度小于设定值，控制升温器继电器打开。
  *          如果湿度大于设定值，控制减湿器继电器打开，如果湿度小于设定值，控制加湿器继电器打开。
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
// 	timer_reload_ms(Timer0,5); //这方式，对周期计时影响较大
	
	TL0 = 0x00;				//设置定时初始值 5ms
	TH0 = 0xEE;				//设置定时初始值
	
	timerCount++;
	
	g_key_value = key_scan(5);
	
	// 计算到200取余 ，这样就相当于 200×5 == 1000ms 就是1s
	if( timerCount % 200 == 0){
		LED = !LED;
		read_dht11_flag = 1; //读取DHT11的标志置1，表示可以开始读DHT11
	}
	// 计算到20取余 ，这样就相当于 20×5 == 100ms  
	if( timerCount % 20 == 0){
		ui_refresh_flag = 1; //界面可刷新
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




