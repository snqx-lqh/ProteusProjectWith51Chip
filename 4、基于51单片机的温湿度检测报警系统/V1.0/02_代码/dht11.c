#include "dht11.h"

void DHT11_delay_ms(uint16_t z)
{
   uint16_t i,j;
   for(i=z;i>0;i--)
      for(j=110;j>0;j--);
}

static void Delay30us(void)	//@11.0592MHz
{
	unsigned char data i;

	i = 11;
	while (--i);
}

void Delay40us(void)	//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 15;
	while (--i);
}

static void Delay80us(void)	//@11.0592MHz
{
	unsigned char data i;

	i = 34;
	while (--i);
}

void DHT11_start()
{
   DHT11_PIN=1;
   DHT11_delay_ms(1);
   DHT11_PIN=0;
   DHT11_delay_ms(30);   // ��ʱ18ms����
   DHT11_PIN=1;
   Delay40us();          //��ʱ20-40us
}

uint8_t DHT11_rec_byte()      //����һ���ֽ�
{
   uint8_t i,read_data=0;
   for(i=0;i<8;i++)    //�Ӹߵ������ν���8λ����
   {          
		while(!DHT11_PIN);          //�ȴ�50us�͵�ƽ��ȥ
		Delay40us();                //0-�ߵ�ƽ����26~28us��1-�ߵ�ƽ����70us,
		read_data<<=1;              //��λʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
		if(DHT11_PIN==1)            //����Ϊ1ʱ��ʹread_data��1�������λ����д1
			read_data += 1;
		while(DHT11_PIN);           //�ȴ�����������    
    }  
    return read_data;
}

/**
 * �� DHT11 ��ȡ��ʪ��
 * @param   hum  ָ����ʪ���������ֵı���
 * @param   temp ָ�����¶��������ֵı���
 * @return  0 ������ȡ���� 1 ��ȡ����ʧ��
 */
uint8_t DHT11_Get_Data(uint8_t *hum, uint8_t *temp)      //����40λ������
{
    uint8_t R_H,R_L,T_H,T_L,RH,RL,TH,TL,revise; 
    DHT11_start();
    if(DHT11_PIN==0)
    {
        while(DHT11_PIN==0);   //�ȴ�����     
        Delay80us();      //���ߺ���ʱ80us
        R_H=DHT11_rec_byte();    //����ʪ�ȸ߰�λ  
        R_L=DHT11_rec_byte();    //����ʪ�ȵͰ�λ  
        T_H=DHT11_rec_byte();    //�����¶ȸ߰�λ  
        T_L=DHT11_rec_byte();    //�����¶ȵͰ�λ
        revise=DHT11_rec_byte(); //����У��λ
        if((R_H+R_L+T_H+T_L)==revise)      //У��
        {
            RH=R_H;
            RL=R_L;
            TH=T_H;
            TL=T_L;
        } 
		*hum = RH;
		*temp = TH;
		return 0;
    }
	return 1;
}


