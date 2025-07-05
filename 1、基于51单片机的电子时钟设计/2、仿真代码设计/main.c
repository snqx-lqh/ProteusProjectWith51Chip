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
  * Date       : 2022-07-03         
  * Description: CSDN��ҳ�� https://blog.csdn.net/wan1234512?type=blog
			     Gitee��ַ��https://gitee.com/snqx-lqh
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

//�����Ƿ��µı�־λ������Ϊ1��û����Ϊ0
bit  isKey0 = 0; //�޸�
bit  isKey1 = 0; //����---�޸���ʾ
bit  isKey2 = 0; //����
bit  isKey3 = 0; //ȷ��

//0 ������ʾ״̬  1 �޸�ʱ��״̬
bit  changeOrNormalState = 0;
//��ʾģʽ 0 ��ʾ����ʱ����  1 ��ʾ����������  2 ��ʾ�޸�ʱ���µ�ʱ����  3��ʾ�޸�ʱ���µ�������
u8   showMode = 0;
//�����޸ĵı��� 0 �� 1 �� 2 ʱ 3 �� 4 �� 5 ��
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
  * @brief   ������ʾ״̬�������޸����ڵ���ʾ����
  * @param   : [����/��] 
  * @retval  ����ֵ
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
  * @brief   ��������ʱ������״ֵ̬��1
  * @param   mode: 1 ���� 0 ������ 
  * @retval  ����ֵ
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
  * @brief   �������º��ִ�к���
  * @param   : [����/��] 
  * @retval  ����ֵ
  */
void ClockChangeFunction()
{
	if(isKey0 == 1)
	{
		isKey0 = 0;
		if(changeCount == 0 && changeOrNormalState == 0)//�޸ĵ�λѡΪ0����Ϊ�룬ͬʱ��ʾ״̬Ϊ������ʾ״̬��
		{
			changeOrNormalState = 1;//��ʾ״̬��Ϊ�޸�ʱ��ģʽ
			DataTempGet();//��ȡ�޸�ǰ��ʱ�����ֵ
			showMode = 2;//��ʾģʽΪ��ʾ�޸�ʱ���µ�ʱ����
		}else if(changeOrNormalState == 1)//����Ϊ��ʾģʽ
		{
			changeCount++;//��������λ��++
			if(changeCount > 5)
				changeCount = 0;
			if(changeCount > 2)//ʱ������0��1��2������2��Ҫ������������ʾ
				showMode = 3;//��ʾģʽΪ��ʾ�޸�ʱ���µ�������
			else
				showMode = 2;
		}
	}else if(isKey1 == 1)
	{
		isKey1 = 0;
		if(changeOrNormalState == 1)//��ʾ״̬Ϊ�޸�ʱ��ģʽ
		{
			dataAdd();//��Ӧλ��ʱ���������
		}else //������ʾģʽ
		{
			if(showMode == 0)//�л�ʱ������ڵ���ʾ
				showMode = 1;
			else if(showMode == 1)
				showMode = 0;
		}
	}else if(isKey2 == 1)
	{
		isKey2 = 0;
		if(changeOrNormalState == 1)//������һ��
		{
			dataSub();
		}
	}else if(isKey3 == 1)
	{
		isKey3 = 0;
		if(changeOrNormalState == 1)//���޸�ģʽ�°���
		{
			changeOrNormalState = 0;//��Ϊ������ʾģʽ
			ds1302writetime();//д���޸ĺ��ʱ��
			showMode = 0;//��ʾģʽΪʱ����
			changeCount = 0;//�޸�λ�ι���
		}
	}
}

/**
  * @name    DataTempGet
  * @brief   �޸İ������º󣬽����ڵ�ʱ�䣬����temp��
  * @param   : [����/��] 
  * @retval  ����ֵ
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
  * @brief   ���Ӱ������º�ʱ������ı仯��ʽ����Ҫ���и�ƽ����Ĵ����������Ƚ�����
  * @param   : [����/��] 
  * @retval  ����ֵ
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
  * @brief   ���ٰ������º�ʱ������ı仯��ʽ
  * @param   : [����/��] 
  * @retval  ����ֵ
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
  * @brief   �޸�ʱ��ģʽ�£�ѡ�е�ʱ�������˸
  * @param   : [����/��] 
  * @retval  ����ֵ
  */
void dataBlink()
{
	static u8  blinkCount = 0;
	static bit blinkState = 0;
	if(changeOrNormalState == 1)//�ڰ����޸�ģʽ��
	{
		blinkCount++;
		if(blinkCount == 80)//ÿ��80*5 ��˸״ֵ̬ת��
		{
			blinkState = !blinkState;
			blinkCount = 0;
		}
	}else
	{
		blinkState = 0;
	}
	if(blinkState == 1)//������˸״ֵ̬Ϊ1����λ�α�������ܲ���ʾ������seg.c���ж��壬10���ǲ���ʾ
	{
		segBuff[(2-changeCount%3)*2+1] = 10;
		segBuff[(2-changeCount%3)*2]   = 10;
	}
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
	KeyScan(0);//����ɨ��
	segBuffChange();//��ʾBUFF�ı�
	dataBlink();//����Ҫ��˸��λ�α����޸�
	SegShow();//�������ʾ
}
