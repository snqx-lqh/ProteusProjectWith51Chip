#include "bsp_timer.h"

/**
  * @brief   ��ʱ���жϳ�ʼ�����������ö�ʱʱ�䣨��λ��΢�룩
  *          ��δ������³���ԣ�����11.0592MHZ�²�����50000us�����۸�Ƶ���ܵ�71ms
  * @param   Timer��Timer0 Timer1   xus����ʱʱ�䳤��
  * @retval  void
 **/
void bsp_timer_set_us_IT(Timer_t Timer, uint16_t xus) 
{
    uint16_t reload_value;  // ���ڴ洢��ʱ����ֵ
	
    reload_value = 65536 - (uint16_t)(SOC_FREQ * xus / 12.0f);

    // ���ö�ʱ��ģʽ
	if(Timer == Timer0){
		TMOD &= 0xF0;  // �������λ����������λ�Ķ�ʱ��1ģʽ
		TMOD |= 0x01;  // ���ö�ʱ��0Ϊģʽ1��16λ��ʱ��ģʽ��
		 // ����ʱ����ֵ�ֱ�д���8λ�͸�8λ
		TL0 = (uint8_t)(reload_value & 0xFF);  // ��8λ
		TH0 = (uint8_t)(reload_value >> 8);   // ��8λ
		
		TF0 = 0;// ���TF0��־		
		TR0 = 1;// ������ʱ��0
		ET0 = 1;// �򿪶�ʱ��0�ж�
	}else if(Timer == Timer1){
		TMOD &= 0x0F;  // �������λ����������λ�Ķ�ʱ��0ģʽ
		TMOD |= 0x10;  // ���ö�ʱ��1Ϊģʽ1��16λ��ʱ��ģʽ��
		TL1 = (uint8_t)(reload_value & 0xFF);  // ��8λ
		TH1 = (uint8_t)(reload_value >> 8);   // ��8λ
		
		TF1 = 0;// ���TF0��־		
		TR1 = 1;// ������ʱ��0
		ET1 = 1;// �򿪶�ʱ��0�ж�
	}
}

/**
  * @brief   ��ʱ���жϳ�ʼ�����������ö�ʱʱ�䣨��λ�����룩
  *          ��δ������³���ԣ�����11.0592MHZ�²�����50ms�����۸�Ƶ���ܵ�71ms
  * @param   Timer��Timer0 Timer1   xms����ʱʱ�䳤�� 
  * @retval  void
 **/
void bsp_timer_set_ms_IT(Timer_t Timer, uint16_t xms) 
{
	bsp_timer_set_us_IT(Timer, xms*1000);
}

/**
  * @brief   �������õ�Ƭ����TH��TL�Ĵ�����ʹ�����¶�ʱ��ʱ�����Ͷ����һ����
  * @param   Timer��Timer0 Timer1   xus����ʱʱ�䳤�� 
  * @retval  void
 **/
void timer_reload_us(Timer_t Timer, uint16_t xus)
{
	uint16_t reload_value;  // ���ڴ洢��ʱ����ֵ
	
    reload_value = 65536 - (uint16_t)(SOC_FREQ * xus / 12.0f);

    // ���ö�ʱ��ģʽ
	if(Timer == Timer0){
		 // ����ʱ����ֵ�ֱ�д���8λ�͸�8λ
		TL0 = (uint8_t)(reload_value & 0xFF);  // ��8λ
		TH0 = (uint8_t)(reload_value >> 8);   // ��8λ
	}else if(Timer == Timer1){
		TL1 = (uint8_t)(reload_value & 0xFF);  // ��8λ
		TH1 = (uint8_t)(reload_value >> 8);   // ��8λ
	}
}

/**
  * @brief   �������õ�Ƭ����TH��TL�Ĵ�����ʹ�����¶�ʱ��ʱ�����Ͷ����һ����
  * @param   Timer��Timer0 Timer1   xms����ʱʱ�䳤�� 
  * @retval  void
 **/
void timer_reload_ms(Timer_t Timer, uint16_t xms)
{
	timer_reload_us(Timer, xms * 1000);
}

//void Timer0Routine(void) interrupt 1 {}
//void Timer1Routine(void) interrupt 3 {}

