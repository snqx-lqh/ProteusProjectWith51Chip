/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2022, LQH, China, GCU.
  *                                All Rights Reserved
  *                              
  *                                      By LQH
  *                       
  * FileName   : main.c   	
  * Author     : 少年潜行(snqx-lgh)			
  * Date       : 2022-07-03         
  * Description: CSDN主页： https://blog.csdn.net/wan1234512?type=blog
			     Gitee地址：https://gitee.com/snqx-lqh
  ******************************************************************************
  */
#include <reg52.h>
#include "main.h"

#include "seg.h"
#include "ds1302.h"

sbit KEY0 = P3^4;
sbit KEY1 = P3^5;
sbit KEY2 = P3^6;
sbit KEY3 = P3^7;

//按键是否按下的标志位，按下为1，没按下为0
bit  isKey0 = 0; //修改
bit  isKey1 = 0; //增加---修改显示
bit  isKey2 = 0; //减少
bit  isKey3 = 0; //确认

//0 正常显示状态  1 修改时间状态
bit  changeOrNormalState = 0;
//显示模式 0 显示正常时分秒  1 显示正常年月日  2 显示修改时间下的时分秒  3显示修改时间下的年月日
u8   showMode = 0;
//正在修改的变量 0 秒 1 分 2 时 3 日 4 月 5 年
u8   changeCount = 0;

void Timer0Init(void);
void segBuffChange(void);
void ClockChangeFunction(void);
void dataAdd(void);
void dataSub(void);
void DataTempGet(void);

void main()
{
	ds1302init();
	Timer0Init();
	while(1)
	{
		ds1302readtime();
		ClockChangeFunction();
	}
}

/**
  * @name    SegBuffChange
  * @brief   根据显示状态变量，修改现在的显示变量
  * @param   : [输入/出] 
  * @retval  返回值
  */
void SegBuffChange()
{
	if(showMode == 0)
	{
		segBuff[5] = second%10;
		segBuff[4] = second/10;
		segBuff[3] = minute%10;
		segBuff[2] = minute/10;
		segBuff[1] = hour%10;
		segBuff[0] = hour/10;
	}else if(showMode == 1)
	{
		segBuff[5] = day%10;
		segBuff[4] = day/10;
		segBuff[3] = month%10;
		segBuff[2] = month/10;
		segBuff[1] = year%10;
		segBuff[0] = year/10;
	}else if(showMode == 2)
	{
		segBuff[5] = secondTemp%10;
		segBuff[4] = secondTemp/10;
		segBuff[3] = minuteTemp%10;
		segBuff[2] = minuteTemp/10;
		segBuff[1] = hourTemp%10;
		segBuff[0] = hourTemp/10;
	}else if(showMode == 3)
	{
		segBuff[5] = dayTemp%10;
		segBuff[4] = dayTemp/10;
		segBuff[3] = monthTemp%10;
		segBuff[2] = monthTemp/10;
		segBuff[1] = yearTemp%10;
		segBuff[0] = yearTemp/10;
	}
}

/**
  * @name    KeyScan
  * @brief   按键按下时，各个状态值变1
  * @param   mode: 1 连续 0 不连续 
  * @retval  返回值
  */
void KeyScan(u8 mode)
{
    static int keyCount = 0;
    static int keyState = 0;
    if(mode == 1) keyState=0;
    if (keyState == 0 && (KEY0 == 0||KEY1 == 0||KEY2 == 0||KEY3 == 0))
    {
        keyCount++;
        if(keyCount>2)
        {
            keyState = 1;
            keyCount=0;
            if(KEY0 == 0) isKey0 = 1;
            else if(KEY1 == 0) isKey1 = 1;
			else if(KEY2 == 0) isKey2 = 1;
			else if(KEY3 == 0) isKey3 = 1;
        }
    } else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && KEY3 == 1)
    {
        keyState = 0;
    }
}

/**
  * @name    ClockChangeFunction
  * @brief   按键按下后的执行函数
  * @param   : [输入/出] 
  * @retval  返回值
  */
void ClockChangeFunction()
{
	if(isKey0 == 1)
	{
		isKey0 = 0;
		if(changeCount == 0 && changeOrNormalState == 0)//修改的位选为0，即为秒，同时显示状态为正常显示状态下
		{
			changeOrNormalState = 1;//显示状态改为修改时间模式
			DataTempGet();//获取修改前的时间变量值
			showMode = 2;//显示模式为显示修改时间下的时分秒
		}else if(changeOrNormalState == 1)//假如为显示模式
		{
			changeCount++;//按键按下位次++
			if(changeCount > 5)
				changeCount = 0;
			if(changeCount > 2)//时分秒是0、1、2，大于2就要换成年月日显示
				showMode = 3;//显示模式为显示修改时间下的年月日
			else
				showMode = 2;
		}
	}else if(isKey1 == 1)
	{
		isKey1 = 0;
		if(changeOrNormalState == 1)//显示状态为修改时间模式
		{
			dataAdd();//对应位次时间变量增加
		}else //正常显示模式
		{
			if(showMode == 0)//切换时间和日期的显示
				showMode = 1;
			else if(showMode == 1)
				showMode = 0;
		}
	}else if(isKey2 == 1)
	{
		isKey2 = 0;
		if(changeOrNormalState == 1)//和增加一样
		{
			dataSub();
		}
	}else if(isKey3 == 1)
	{
		isKey3 = 0;
		if(changeOrNormalState == 1)//在修改模式下按下
		{
			changeOrNormalState = 0;//变为正常显示模式
			ds1302writetime();//写入修改后的时间
			showMode = 0;//显示模式为时分秒
			changeCount = 0;//修改位次归零
		}
	}
}

/**
  * @name    DataTempGet
  * @brief   修改按键按下后，将现在的时间，放入temp中
  * @param   : [输入/出] 
  * @retval  返回值
  */
void DataTempGet()
{
	secondTemp = second;
	minuteTemp = minute;
	hourTemp   = hour;
	dayTemp    = day;
	monthTemp  = month;
	weekTemp   = week;
	yearTemp   = year;
}

/**
  * @name    dataAdd
  * @brief   增加按键按下后，时间变量的变化方式，主要是有个平闰年的处理，其他都比较正常
  * @param   : [输入/出] 
  * @retval  返回值
  */
void dataAdd()
{
	if(changeCount == 0)
	{
		secondTemp ++;
		if(secondTemp > 59)
			secondTemp = 0;
	}else if(changeCount == 1)
	{
		minuteTemp ++;
		if(minuteTemp > 59)
			minuteTemp = 0;
	}else if(changeCount == 2)
	{
		hourTemp ++;
		if(hourTemp > 23)
			hourTemp = 0;
	}else if(changeCount == 3)
	{
		dayTemp ++;
		if(monthTemp == 1 || monthTemp == 3 || monthTemp == 5 || monthTemp == 7 || monthTemp == 8 || monthTemp == 10 || monthTemp == 12)
		{
			if(dayTemp > 31)
				dayTemp = 1;
		}else if(monthTemp == 3 || monthTemp == 6 || monthTemp == 9 || monthTemp == 11)
		{
			if(dayTemp > 30)
				dayTemp = 1;
		}else if(monthTemp == 2)
		{
			if((2000+year)%400==0)
			{
				if(dayTemp > 29)
					dayTemp = 1;
			}
			else
			{
				if((2000+year)%4==0&&(2000+year)%100!=0)
				{
					if(dayTemp > 29)
						dayTemp = 1;
				}else
				{
					if(dayTemp > 28)
						dayTemp = 1;
				}
			}
		}
	}else if(changeCount == 4)
	{
		monthTemp ++;
		if(monthTemp > 12)
			monthTemp = 1;
	}else if(changeCount == 5)
	{
		yearTemp ++;
		if(yearTemp > 99)
			yearTemp = 0;
	}
}

/**
  * @name    dataSub
  * @brief   减少按键按下后，时间变量的变化方式
  * @param   : [输入/出] 
  * @retval  返回值
  */
void dataSub()
{
	if(changeCount == 0)
	{
		secondTemp --;
		if((char)secondTemp < 0)
			secondTemp = 59;
	}else if(changeCount == 1)
	{
		minuteTemp --;
		if((char)minuteTemp < 0)
			minuteTemp = 59;
	}else if(changeCount == 2)
	{
		hourTemp --;
		if((char)hourTemp < 0)
			hourTemp = 23;
	}else if(changeCount == 3)
	{
		dayTemp --;
		if(monthTemp == 1 || monthTemp == 3 || monthTemp == 5 || monthTemp == 7 || monthTemp == 8 || monthTemp == 10 || monthTemp == 12)
		{
			if(dayTemp < 1)
				dayTemp = 31;
		}else if(monthTemp == 3 || monthTemp == 6 || monthTemp == 9 || monthTemp == 11)
		{
			if(dayTemp < 1)
				dayTemp = 30;
		}else if(monthTemp == 2)
		{
			if((2000+year)%400==0)
			{
				if(dayTemp < 1)
					dayTemp = 29;
			}
			else
			{
				if((2000+year)%4==0&&(2000+year)%100!=0)
				{
					if(dayTemp < 1)
						dayTemp = 29;
				}else
				{
					if(dayTemp < 1)
						dayTemp = 28;
				}
			}
			
		}
	}else if(changeCount == 4)
	{
		monthTemp --;
		if(monthTemp < 1)
			monthTemp = 12;
	}else if(changeCount == 5)
	{
		yearTemp --;
		if((char)yearTemp < 0)
			yearTemp = 99;
	}
}

/**
  * @name    dataBlink
  * @brief   修改时间模式下，选中的时间变量闪烁
  * @param   : [输入/出] 
  * @retval  返回值
  */
void dataBlink()
{
	static u8  blinkCount = 0;
	static bit blinkState = 0;
	if(changeOrNormalState == 1)//在按键修改模式下
	{
		blinkCount++;
		if(blinkCount == 80)//每隔80*5 闪烁状态值转换
		{
			blinkState = !blinkState;
			blinkCount = 0;
		}
	}else
	{
		blinkState = 0;
	}
	if(blinkState == 1)//假如闪烁状态值为1，该位次变量数码管不显示，我在seg.c中有定义，10就是不显示
	{
		segBuff[(2-changeCount%3)*2+1] = 10;
		segBuff[(2-changeCount%3)*2]   = 10;
	}
}

void Timer0Init(void) //5毫秒@12MHz
{
    TMOD = 0x01; //设置定时器模式
    TL0 = 0x78;  //设置定时0初始值
    TH0 = 0xEC;  //设置定时0初始值
    TF0 = 0;     //清除TF0标志
    TR0 = 1;     //定时器0开始计时
    ET0 = 1;
    EA = 1;
}

void Timer0() interrupt 1
{
    TL0 = 0x78; //设置定时0初始值
    TH0 = 0xEC; //设置定时0初始值
	KeyScan(0);//按键扫描
	segBuffChange();//显示BUFF改变
	dataBlink();//将需要闪烁的位次变量修改
	SegShow();//数码管显示
}
