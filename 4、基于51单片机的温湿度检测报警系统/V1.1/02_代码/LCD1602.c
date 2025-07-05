#include "LCD1602.h"

/**
  * @name    Lcd1602_Delay1ms
  * @brief   延时函数，延时1ms
  * @param   : [输入/出] 
  * @retval  返回值
  */
void Lcd1602_Delay1ms(uint16_t c)   
{
    uint8_t a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
    	
}
/**
  * @brief   向LCD写入一个字节的命令
  * @param   com
  * @retval  返回值
  */
void LcdWriteCom(uint8_t com)	  //写入命令
{
	Lcd1602_Delay1ms(1);
	LCD1602_E = 0;     //使能
	LCD1602_RS = 0;	   //选择发送命令
	LCD1602_RW = 0;	   //选择写入
	
	LCD1602_DATAPINS = com;     //放入命令
//	Lcd1602_Delay1ms(1);		//等待数据稳定

	LCD1602_E = 1;	          //写入时序
//	Lcd1602_Delay1ms(5);	  //保持时间
	LCD1602_E = 0;
}
/**
  * @brief   向LCD写入一个字节的数据
  * @param   dat
  * @retval  返回值
  */
void LcdWriteData(uint8_t dat)			//写入数据
{
	Lcd1602_Delay1ms(1);
	LCD1602_E = 0;	//使能清零
	LCD1602_RS = 1;	//选择输入数据
	LCD1602_RW = 0;	//选择写入

	LCD1602_DATAPINS = dat; //写入数据
//	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //写入时序
//	Lcd1602_Delay1ms(5);   //保持时间
	LCD1602_E = 0;
}	
/**
  * @brief   初始化LCD屏
  * @param   无
  * @retval  返回值
  */
void LcdInit()						  //LCD初始化子程序
{
 	LcdWriteCom(0x38);  //开显示
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);  //设置数据指针起点
}	
/**
  * @brief   设置显示 RAM 起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标
  * @param   x y
  * @retval  返回值
  */
void LcdSetCursor(uint8_t x, uint8_t y)
{
	uint8_t addr;

	if (y == 0) //由输入的屏幕坐标计算显示 RAM 的地址
		addr = 0x00 + x; //第一行字符地址从 0x00 起始
	else
		addr = 0x40 + x; //第二行字符地址从 0x40 起始
	LcdWriteCom(addr | 0x80); //设置 RAM 地址
}
/**
  * @brief   在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针
  * @param   x y str
  * @retval  返回值
  */
void LcdShowStr(uint8_t x, uint8_t y, uint8_t *str)
{
	LcdSetCursor(x, y); //设置起始地址
	while (*str != '\0') //连续写入字符串数据，直到检测到结束符
	{
		LcdWriteData(*str++); //先取 str 指向的数据，然后 str 自加 1
	}
}
/**
  * @brief   在液晶上显示字符串，大于长度的数字只显示低位
  * @param   (x,y)-对应屏幕上的起始坐标，
  *          num-显示的数字，
  *          len显示数字长度，
  *          mode为0，长度不足的前面补零，为1，长度不足的前面补空格
  * @retval  返回值
  */
void LcdShowNumInt(uint8_t x,uint8_t y,uint16_t num,uint8_t len,uint8_t mode)
{
	uint8_t buffer[10];
    uint8_t i = 0;
	int j=0;
    uint8_t fillChar = (mode == 0) ? '0' : ' ';
	
    // 将数字转换为字符串
    do {
        buffer[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0 && i < len);

    // 如果长度不足，根据模式补零或空格
    while (i < len) {
        buffer[i++] = fillChar;
    }

    LcdSetCursor(x, y); // 设置起始地址

    // 逆序输出字符串
    for (j = i - 1; j >= 0; j--) {
        LcdWriteData(buffer[j]);
    }
}

/**
  * @brief   在液晶上显示字符串，大于长度的数字只显示低位，小数显示离点近的
  * @param   (x,y)-对应屏幕上的起始坐标，
  *          num-显示的数字，
  *          intLen 小数点前显示数字的长度，floatLen 小数点后显示数字的长度
  *          mode为0，长度不足的整数前面补零，小数后面补零，为1，补空格
  * @retval  返回值
  */
void LcdShowNumFloat(uint8_t x,uint8_t y,float num,uint8_t intLen,uint8_t floatLen,uint8_t mode)
{
	uint16_t intPart = (uint16_t)num;
    float decimalPart = num - intPart;
    uint8_t buffer[20];
    uint8_t i,k = 0;
	int j=0;
    uint8_t fillCharInt = (mode == 0) ? '0' : ' ';
    uint8_t fillCharFloat = (mode == 0) ? '0' : ' ';

    // 处理整数部分
    do {
        buffer[i++] = intPart % 10 + '0';
        intPart /= 10;
    } while (intPart > 0 && i < intLen);

    // 如果长度不足，根据模式补零或空格
    while (i < intLen) {
        buffer[i++] = fillCharInt;
    }

    // 逆序输出整数部分
    LcdSetCursor(x, y);
    for (j = i - 1; j >= 0; j--) {
        LcdWriteData(buffer[j]);
    }

    // 输出小数点
    LcdWriteData('.');

    // 处理小数部分
    i = 0;
    for (k = 0; k < floatLen; k++) {
        decimalPart *= 10;
        buffer[i++] = (uint8_t)decimalPart + '0';
        decimalPart -= (uint8_t)decimalPart;
    }

    // 输出小数部分
    for (j = 0; j < i; j++) {
        LcdWriteData(buffer[j]);
    }
}