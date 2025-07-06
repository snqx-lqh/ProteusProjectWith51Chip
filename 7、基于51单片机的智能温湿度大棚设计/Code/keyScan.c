#include "keyScan.h"

/**
  * @brief   按键相关初始化，就是将按键对应的引脚拉高
  * @param   
  * @retval  void
 **/
void key_init(void)
{
	KEY1 = 1;
	KEY2 = 1;
	KEY3 = 1;
	KEY4 = 1;
}

/**
  * @brief   按键扫描，将其放入周期调用的函数中，最好放入定时器中断
  * @param   xms 周期调用的时间 单位/ms 假如5ms调用一次，这个值就设置成5
  * @retval  uint8_t 对应按键代表的按键值
 **/
uint8_t key_scan(uint8_t xms)
{
	static uint8_t key_state       = 0;  //按键当前状态
	static uint8_t key_delay_count = 0;  //按键延时计数
	// 当按键状态为0  同时检测到某个按键按下的情况
	if(key_state == 0 && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0))
	{
		// 按键延时计数计一个数
		key_delay_count ++;
		// 按键延时计数 × 周期调用值 = 总的延时时长
		if(key_delay_count * xms > 20)
		{
			// 设置按键状态为按下
			key_state       = 1;
			// 延时计数值清零
			key_delay_count = 0;
			// 检测到对应的按键按下，就返回对应的按键
			if(KEY1 == 0) return KEY1_VALUE;
			else if(KEY2 == 0) return KEY2_VALUE;
			else if(KEY3 == 0) return KEY3_VALUE;
			else if(KEY4 == 0) return KEY4_VALUE;
		}
	}
	// 假如全部按键处于抬起状态
	else if(key_state == 1 && KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1)
	{
		// 清零所有状态位
		key_state = 0;
		key_delay_count = 0;
	}
	return 0;
}
