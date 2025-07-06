#include "keyScan.h"

/**
  * @brief   ������س�ʼ�������ǽ�������Ӧ����������
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
  * @brief   ����ɨ�裬����������ڵ��õĺ����У���÷��붨ʱ���ж�
  * @param   xms ���ڵ��õ�ʱ�� ��λ/ms ����5ms����һ�Σ����ֵ�����ó�5
  * @retval  uint8_t ��Ӧ��������İ���ֵ
 **/
uint8_t key_scan(uint8_t xms)
{
	static uint8_t key_state       = 0;  //������ǰ״̬
	static uint8_t key_delay_count = 0;  //������ʱ����
	// ������״̬Ϊ0  ͬʱ��⵽ĳ���������µ����
	if(key_state == 0 && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0))
	{
		// ������ʱ������һ����
		key_delay_count ++;
		// ������ʱ���� �� ���ڵ���ֵ = �ܵ���ʱʱ��
		if(key_delay_count * xms > 20)
		{
			// ���ð���״̬Ϊ����
			key_state       = 1;
			// ��ʱ����ֵ����
			key_delay_count = 0;
			// ��⵽��Ӧ�İ������£��ͷ��ض�Ӧ�İ���
			if(KEY1 == 0) return KEY1_VALUE;
			else if(KEY2 == 0) return KEY2_VALUE;
			else if(KEY3 == 0) return KEY3_VALUE;
			else if(KEY4 == 0) return KEY4_VALUE;
		}
	}
	// ����ȫ����������̧��״̬
	else if(key_state == 1 && KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1)
	{
		// ��������״̬λ
		key_state = 0;
		key_delay_count = 0;
	}
	return 0;
}
