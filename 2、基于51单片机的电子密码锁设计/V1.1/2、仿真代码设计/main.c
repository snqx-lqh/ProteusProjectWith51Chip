/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2022, LQH, China, GCU.
  *                                All Rights Reserved
  *                              
  *                                      By LQH
  *                       
  * FileName   : main.c   	
  * Author     : ����Ǳ��(snqx-lgh)			
  * Date       : 2023-06-14         
  * Description: CSDN��ҳ�� https://blog.csdn.net/wan1234512?type=blog
			     Gitee��ַ��https://gitee.com/snqx-lqh
  ******************************************************************************
  */
#include <reg52.h>
#include "main.h"
#include <stdio.h>
#include <string.h>

#include "LCD1602.h"
#include "KeyScan.h"

u8 DisBuff[17]={0};//��ʾ������
u8 Password[7] = "666666";//��ʼ����
u8 KeyValueTable[16]={1 ,2 ,3 ,10,
                      4 ,5 ,6 ,10,
                      7 ,8 ,9 ,10,
                      10,0 ,10,10};//����ӳ���

u8 KeyValueGet = 0;

bit OpenFlag   = 0;//�������ܺ�����־λ
bit CloseFlag  = 0;//�������ܺ�����־λ
bit ChangeFlag = 0;//�޸����빦�ܺ�����־λ

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
  * @brief   ������صĹ��ܺ�������������
  * @param   KeyValueTemp: ʵ�ʵİ���ֵ 
  * @retval
 **/
void OpenLock(u8 KeyValueTemp)
{
    static u8 OpenCount = 0;
    static u8 PasswordTemp[7] = {0};
    if(OpenCount == 0)//һ����������Ϊ0ʱ�����Ǹս��뿪��״̬��ʱ����ʾpwd :�ַ���
    {
        sprintf((char*)DisBuff,"pwd :           ");
        LcdShowStr(0,1,DisBuff);
        OpenCount++;//��ʾ���Ѽ�������һ
    }else//�´ε���ͽ���else
    {
        if(KeyValueTemp == KEY_Back && OpenCount > 1)//��������˰�ť
        {
            OpenCount --;//�ص���һ����������λ��
            DisBuff[OpenCount+5] = ' ';//����ʾλ���
            PasswordTemp[OpenCount-1] = 0;//����λ������0����Ϊ����ʹ�õ��ַ�����һ��ASCLL����Ϊ0
            LcdShowStr(0,1,DisBuff);
        }else if(OpenCount < 7)//�����趨Ϊ6λ������6λ�����Ͳ�����������Ĺ��ܿ���
        {
            /*
                �����и�KeyValueTable����Ϊ�������µ�ֵ����ʵ�����Ű��ֵ����������������һ��ת��
            */
            DisBuff[OpenCount+5] = '0' + KeyValueTable[KeyValueTemp-1];//����ʾλ��ʾ�������ֵ
            PasswordTemp[OpenCount-1] = '0' + KeyValueTable[KeyValueTemp-1];//���������ֵ����
            LcdShowStr(0,1,DisBuff);
            OpenCount++;//��λ������������һλ
        }
    }
    if(OpenCount > 6 && KeyValueTemp == KEY_Confirm)//6λ�������꣬�ֵ����ȷ�ϰ�ť�Ļ����Ϳ�ʼ�Ƚ�����
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
  * @brief   �޸��������غ���
  * @param   KeyValueTemp: ʵ�ʵİ���ֵ
  * @retval
 **/
void ChangePassword(u8 KeyValueTemp)
{
    static u8 ChangeCount = 0;
    static u8 ChangeMode  = 0;//0����������룬1������������
    static u8 PasswordTemp[7] = {0};
    static u8 NewPasswordTemp[7] = {0};
    if(ChangeMode == 0)
    {
        if(ChangeCount == 0)//һ����������Ϊ0ʱ�����Ǹս����޸�����״̬��ʱ����ʾpwd :�ַ���
        {
            sprintf((char*)DisBuff,"pwd :           ");
            LcdShowStr(0,1,DisBuff);
            ChangeCount++;//��ʾ���Ѽ�������һ
        }else//�´ε���ͽ���else
        {
            if(KeyValueTemp == KEY_Back && ChangeCount > 1)//��������˰�ť
            {
                ChangeCount --;//�ص���һ����������λ��
                DisBuff[ChangeCount+5] = ' ';//����ʾλ���
                PasswordTemp[ChangeCount-1] = 0;//����λ������0����Ϊ����ʹ�õ��ַ�����һ��ASCLL����Ϊ0
                LcdShowStr(0,1,DisBuff);
            }else if(ChangeCount < 7)//�����趨Ϊ6λ������6λ�����Ͳ�����������Ĺ��ܿ���
            {
                /*
                    �����и�KeyValueTable����Ϊ�������µ�ֵ����ʵ�����Ű��ֵ����������������һ��ת��
                */
                DisBuff[ChangeCount+5] = '0' + KeyValueTable[KeyValueTemp-1];//����ʾλ��ʾ�������ֵ
                PasswordTemp[ChangeCount-1] = '0' + KeyValueTable[KeyValueTemp-1];//���������ֵ����
                LcdShowStr(0,1,DisBuff);
                ChangeCount++;//��λ������������һλ
            }
        }
        if(ChangeCount > 6 && KeyValueTemp == KEY_Confirm)//6λ�������꣬�ֵ����ȷ�ϰ�ť�Ļ����Ϳ�ʼ�Ƚ�����
        {
            ChangeCount = 0;
            if(strcmp(Password,PasswordTemp)==0)//������ȷ�Ϳ���������������
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
        if(KeyValueTemp == KEY_Back && ChangeCount > 1)//��������˰�ť
        {
            ChangeCount --;//�ص���һ����������λ��
            DisBuff[ChangeCount+5] = ' ';//����ʾλ���
            NewPasswordTemp[ChangeCount-1] = 0;//����λ������0����Ϊ����ʹ�õ��ַ�����һ��ASCLL����Ϊ0
            LcdShowStr(0,1,DisBuff);
        }else if(ChangeCount < 7)//�����趨Ϊ6λ������6λ�����Ͳ�����������Ĺ��ܿ���
        {
            /*
                �����и�KeyValueTable����Ϊ�������µ�ֵ����ʵ�����Ű��ֵ����������������һ��ת��
            */
            DisBuff[ChangeCount+5] = '0' + KeyValueTable[KeyValueTemp-1];//����ʾλ��ʾ�������ֵ
            NewPasswordTemp[ChangeCount-1] = '0' + KeyValueTable[KeyValueTemp-1];//���������ֵ����
            LcdShowStr(0,1,DisBuff);
            ChangeCount++;//��λ������������һλ
        }
        if(ChangeCount > 6 && KeyValueTemp == KEY_Confirm)//6λ�������꣬�ֵ����ȷ�ϰ�ť�Ļ����Ϳ�ʼ�Ƚ�����
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
  * @brief   �������º�Ĺ��ܺ���
  * @param   : [����/��] 
  * @retval  
 **/
void KeyDownFunction()
{
    u8 KeyValueTemp = 0;
//    if(KeyState == 1)
//    {
        KeyValueTemp = KeyValueGet;
//        if(KeyValue != 0)//ʵ�ֵ��μ�ⰴ��
//            KeyValue = 0;
        if(KeyValueTemp == KEY_Open && OpenFlag == 0)//ʹ�ܿ������ܺ���
        {
            OpenFlag   = 1;
        }else if(KeyValueTemp == KEY_Close && CloseFlag == 0)//ʹ�ܹ������ܺ���
        {
            CloseFlag  = 1;
        }else if(KeyValueTemp == KEY_Change && ChangeFlag == 0)//ʹ���޸����빦�ܺ���
        {
            ChangeFlag = 1;
        }
        if(OpenFlag == 1)//��������ʹ��
        {  
            OpenLock(KeyValueTemp);
        }
        if(CloseFlag == 1)//��������ʹ��
        {
            sprintf((char*)DisBuff,"----  lock  ----");
            LcdShowStr(0,1,DisBuff);
            CloseFlag = 0;
            LED = 1;
        }
        if(ChangeFlag == 1)//�޸����빦��ʹ��
        {
            ChangePassword(KeyValueTemp);
        }
//    }
}

void Timer0Init(void) //5����@12MHz
{
    TMOD = 0x01; //���ö�ʱ��ģʽ
    TL0 = 0x78;  //���ö�ʱ0��ʼֵ
    TH0 = 0xEC;  //���ö�ʱ0��ʼֵ
    TF0 = 0;     //���TF0��־
    TR0 = 1;     //��ʱ��0��ʼ��ʱ
    ET0 = 1;
    EA = 1;
}

void Timer0() interrupt 1
{
    TL0 = 0x78; //���ö�ʱ0��ʼֵ
    TH0 = 0xEC; //���ö�ʱ0��ʼֵ
    KeyValueGet = KeyScan(5);

}
