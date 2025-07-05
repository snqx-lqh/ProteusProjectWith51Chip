#include "seg.h"

//74HC138��ABC����
sbit HCA = P2^0;
sbit HCB = P2^1;
sbit HCC = P2^2;

#define SEG P1

//����������ܶ��� 0 1 2 3 4 5 6 7 8 9 ��
u8 code segCode[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};
//u8 code segCode[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//�洢���ݵĻ���
u8 segBuff[6] = {0,1,2,3,4,5};

/**
  * @name    SegShow
  * @brief   ����ܵ���ʾ�������ú���������5MS��ʱ����ִ�У�ÿ5MS�л�һ����ʾλ
  * @param   
  * @retval  
  */
void SegShow()
{
	static u8 segSelectCount = 0;//λѡ����
	
	segSelectCount ++;
	if(segSelectCount > 7)
		segSelectCount = 0;

	SEG = 0x00;//����
	switch (segSelectCount)
	{
		case 0: HCC = 0;HCB = 0;HCA = 0;SEG = segCode[segBuff[0]];break;
		case 1: HCC = 0;HCB = 0;HCA = 1;SEG = segCode[segBuff[1]];break;
		case 2: HCC = 0;HCB = 1;HCA = 0;SEG = 0x40               ;break;
		case 3: HCC = 0;HCB = 1;HCA = 1;SEG = segCode[segBuff[2]];break;
		case 4: HCC = 1;HCB = 0;HCA = 0;SEG = segCode[segBuff[3]];break;
		case 5: HCC = 1;HCB = 0;HCA = 1;SEG = 0x40               ;break;
		case 6: HCC = 1;HCB = 1;HCA = 0;SEG = segCode[segBuff[4]];break;
		case 7: HCC = 1;HCB = 1;HCA = 1;SEG = segCode[segBuff[5]];break;
		default:HCC = 0;HCB = 0;HCA = 0;SEG = segCode[segBuff[0]];break;
	}
}
