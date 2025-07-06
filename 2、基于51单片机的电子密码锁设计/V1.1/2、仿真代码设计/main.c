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
  * Date       : 2023-06-14         
  * Description: CSDN主页： https://blog.csdn.net/wan1234512?type=blog
			     Gitee地址：https://gitee.com/snqx-lqh
  ******************************************************************************
  */
#include <reg52.h>
#include "main.h"
#include <stdio.h>
#include <string.h>

#include "LCD1602.h"
#include "KeyScan.h"

u8 DisBuff[17]={0};//显示缓冲区
u8 Password[7] = "666666";//初始密码
u8 KeyValueTable[16]={1 ,2 ,3 ,10,
                      4 ,5 ,6 ,10,
                      7 ,8 ,9 ,10,
                      10,0 ,10,10};//键盘映射表

u8 KeyValueGet = 0;

bit OpenFlag   = 0;//开锁功能函数标志位
bit CloseFlag  = 0;//关锁功能函数标志位
bit ChangeFlag = 0;//修改密码功能函数标志位

sbit LED = P2^4;

void KeyDownFunction(void);
void Timer0Init(void);

void main()
{
	LcdInit();
	Timer0Init();
    LED = 1;
	sprintf((char*)DisBuff,"combination lock");
	LcdShowStr(0,0,DisBuff);
    sprintf((char*)DisBuff,"----  lock  ----");
	LcdShowStr(0,1,DisBuff);
	while(1)
	{
		if(KeyValueGet != 0)
			KeyDownFunction();
	}
}

/**
  * @Name    OpenLock
  * @brief   开锁相关的功能函数，输入密码
  * @param   KeyValueTemp: 实际的按键值 
  * @retval
 **/
void OpenLock(u8 KeyValueTemp)
{
    static u8 OpenCount = 0;
    static u8 PasswordTemp[7] = {0};
    if(OpenCount == 0)//一个计数器，为0时，就是刚进入开锁状态的时候，显示pwd :字符串
    {
        sprintf((char*)DisBuff,"pwd :           ");
        LcdShowStr(0,1,DisBuff);
        OpenCount++;//显示完后把计数器加一
    }else//下次点击就进入else
    {
        if(KeyValueTemp == KEY_Back && OpenCount > 1)//当点击回退按钮
        {
            OpenCount --;//回到上一个密码数的位置
            DisBuff[OpenCount+5] = ' ';//将显示位清空
            PasswordTemp[OpenCount-1] = 0;//将该位密码置0，因为密码使用的字符串，一般ASCLL不会为0
            LcdShowStr(0,1,DisBuff);
        }else if(OpenCount < 7)//密码设定为6位，所以6位输完后就不进入输密码的功能块了
        {
            /*
                这里有个KeyValueTable是因为按键按下的值不是实际我排版的值，所以用数组做了一次转换
            */
            DisBuff[OpenCount+5] = '0' + KeyValueTable[KeyValueTemp-1];//将显示位显示输入的数值
            PasswordTemp[OpenCount-1] = '0' + KeyValueTable[KeyValueTemp-1];//将输入的数值保存
            LcdShowStr(0,1,DisBuff);
            OpenCount++;//该位处理完后进到下一位
        }
    }
    if(OpenCount > 6 && KeyValueTemp == KEY_Confirm)//6位密码输完，又点击了确认按钮的话，就开始比较密码
    {
        OpenFlag  = 0;
        OpenCount = 0;
        if(strcmp(Password,PasswordTemp)==0)
        {
            sprintf((char*)DisBuff,"----  open  ----");
            LcdShowStr(0,1,DisBuff);
            LED = 0;
        }else
        {
            sprintf((char*)DisBuff,"----  lock  ----");
            LcdShowStr(0,1,DisBuff);
            LED = 1;
        }
    }
}

/**
  * @Name    ChangePassword
  * @brief   修改密码的相关函数
  * @param   KeyValueTemp: 实际的按键值
  * @retval
 **/
void ChangePassword(u8 KeyValueTemp)
{
    static u8 ChangeCount = 0;
    static u8 ChangeMode  = 0;//0是输入旧密码，1是输入新密码
    static u8 PasswordTemp[7] = {0};
    static u8 NewPasswordTemp[7] = {0};
    if(ChangeMode == 0)
    {
        if(ChangeCount == 0)//一个计数器，为0时，就是刚进入修改密码状态的时候，显示pwd :字符串
        {
            sprintf((char*)DisBuff,"pwd :           ");
            LcdShowStr(0,1,DisBuff);
            ChangeCount++;//显示完后把计数器加一
        }else//下次点击就进入else
        {
            if(KeyValueTemp == KEY_Back && ChangeCount > 1)//当点击回退按钮
            {
                ChangeCount --;//回到上一个密码数的位置
                DisBuff[ChangeCount+5] = ' ';//将显示位清空
                PasswordTemp[ChangeCount-1] = 0;//将该位密码置0，因为密码使用的字符串，一般ASCLL不会为0
                LcdShowStr(0,1,DisBuff);
            }else if(ChangeCount < 7)//密码设定为6位，所以6位输完后就不进入输密码的功能块了
            {
                /*
                    这里有个KeyValueTable是因为按键按下的值不是实际我排版的值，所以用数组做了一次转换
                */
                DisBuff[ChangeCount+5] = '0' + KeyValueTable[KeyValueTemp-1];//将显示位显示输入的数值
                PasswordTemp[ChangeCount-1] = '0' + KeyValueTable[KeyValueTemp-1];//将输入的数值保存
                LcdShowStr(0,1,DisBuff);
                ChangeCount++;//该位处理完后进到下一位
            }
        }
        if(ChangeCount > 6 && KeyValueTemp == KEY_Confirm)//6位密码输完，又点击了确认按钮的话，就开始比较密码
        {
            ChangeCount = 0;
            if(strcmp(Password,PasswordTemp)==0)//密码正确就可以输入新密码了
            {
                sprintf((char*)DisBuff,"new :           ");
                LcdShowStr(0,1,DisBuff);
                ChangeCount ++;
                ChangeMode = 1;
            }else
            {
                ChangeFlag  = 0;
                sprintf((char*)DisBuff,"----  lock  ----");
                LcdShowStr(0,1,DisBuff);
                LED = 1;
            }
        }
    }else if(ChangeMode == 1)
    {
        if(KeyValueTemp == KEY_Back && ChangeCount > 1)//当点击回退按钮
        {
            ChangeCount --;//回到上一个密码数的位置
            DisBuff[ChangeCount+5] = ' ';//将显示位清空
            NewPasswordTemp[ChangeCount-1] = 0;//将该位密码置0，因为密码使用的字符串，一般ASCLL不会为0
            LcdShowStr(0,1,DisBuff);
        }else if(ChangeCount < 7)//密码设定为6位，所以6位输完后就不进入输密码的功能块了
        {
            /*
                这里有个KeyValueTable是因为按键按下的值不是实际我排版的值，所以用数组做了一次转换
            */
            DisBuff[ChangeCount+5] = '0' + KeyValueTable[KeyValueTemp-1];//将显示位显示输入的数值
            NewPasswordTemp[ChangeCount-1] = '0' + KeyValueTable[KeyValueTemp-1];//将输入的数值保存
            LcdShowStr(0,1,DisBuff);
            ChangeCount++;//该位处理完后进到下一位
        }
        if(ChangeCount > 6 && KeyValueTemp == KEY_Confirm)//6位密码输完，又点击了确认按钮的话，就开始比较密码
        {
            ChangeFlag  = 0;
            ChangeCount = 0;
            sprintf((char*)DisBuff,"---- success----");
            LcdShowStr(0,1,DisBuff);
            ChangeMode = 0;
            strcpy(Password,NewPasswordTemp);
        }
    }
}
/**
  * @Name    KeyDownFunction
  * @brief   按键按下后的功能函数
  * @param   : [输入/出] 
  * @retval  
 **/
void KeyDownFunction()
{
    u8 KeyValueTemp = 0;
//    if(KeyState == 1)
//    {
        KeyValueTemp = KeyValueGet;
//        if(KeyValue != 0)//实现单次检测按键
//            KeyValue = 0;
        if(KeyValueTemp == KEY_Open && OpenFlag == 0)//使能开锁功能函数
        {
            OpenFlag   = 1;
        }else if(KeyValueTemp == KEY_Close && CloseFlag == 0)//使能关锁功能函数
        {
            CloseFlag  = 1;
        }else if(KeyValueTemp == KEY_Change && ChangeFlag == 0)//使能修改密码功能函数
        {
            ChangeFlag = 1;
        }
        if(OpenFlag == 1)//开锁功能使能
        {  
            OpenLock(KeyValueTemp);
        }
        if(CloseFlag == 1)//关锁功能使能
        {
            sprintf((char*)DisBuff,"----  lock  ----");
            LcdShowStr(0,1,DisBuff);
            CloseFlag = 0;
            LED = 1;
        }
        if(ChangeFlag == 1)//修改密码功能使能
        {
            ChangePassword(KeyValueTemp);
        }
//    }
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
    KeyValueGet = KeyScan(5);

}
