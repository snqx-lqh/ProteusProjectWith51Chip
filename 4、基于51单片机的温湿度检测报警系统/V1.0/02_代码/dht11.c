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
   DHT11_delay_ms(30);   // 延时18ms以上
   DHT11_PIN=1;
   Delay40us();          //延时20-40us
}

uint8_t DHT11_rec_byte()      //接收一个字节
{
   uint8_t i,read_data=0;
   for(i=0;i<8;i++)    //从高到低依次接收8位数据
   {          
		while(!DHT11_PIN);          //等待50us低电平过去
		Delay40us();                //0-高电平持续26~28us；1-高电平持续70us,
		read_data<<=1;              //移位使正确接收8位数据，数据为0时直接移位
		if(DHT11_PIN==1)            //数据为1时，使read_data加1来把最低位数据写1
			read_data += 1;
		while(DHT11_PIN);           //等待数据线拉低    
    }  
    return read_data;
}

/**
 * 从 DHT11 获取温湿度
 * @param   hum  指向存放湿度整数部分的变量
 * @param   temp 指向存放温度整数部分的变量
 * @return  0 正常获取数据 1 获取数据失败
 */
uint8_t DHT11_Get_Data(uint8_t *hum, uint8_t *temp)      //接收40位的数据
{
    uint8_t R_H,R_L,T_H,T_L,RH,RL,TH,TL,revise; 
    DHT11_start();
    if(DHT11_PIN==0)
    {
        while(DHT11_PIN==0);   //等待拉高     
        Delay80us();      //拉高后延时80us
        R_H=DHT11_rec_byte();    //接收湿度高八位  
        R_L=DHT11_rec_byte();    //接收湿度低八位  
        T_H=DHT11_rec_byte();    //接收温度高八位  
        T_L=DHT11_rec_byte();    //接收温度低八位
        revise=DHT11_rec_byte(); //接收校正位
        if((R_H+R_L+T_H+T_L)==revise)      //校正
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


