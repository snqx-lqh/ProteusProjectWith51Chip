#include "step_motor.h"

/**
  * @Name    setMotorLevel
  * @brief   ULN2003组成的控制电路控制函数
  * @param   各个引脚的电平
  * @retval  返回值
  */
void setMotorLevel(unsigned char MotorA,unsigned char MotorB,unsigned char MotorC,unsigned char MotorD)
{
	MotorA1 = MotorA;
	MotorB1 = MotorB;
	MotorC1 = MotorC;
	MotorD1 = MotorD;
}

/**
  * @Name    motorPosSet
  * @brief   设置电机的每个位置的引脚电平 0-7 代表步进电机 8 个方向的设置
  * @param   motorPos: 电机转动位置
  * @retval  返回值
  */
void motorPosSet(unsigned char motorPos)
{
	switch (motorPos)
	{
		case 0:setMotorLevel(0,0,0,1);break;
		case 1:setMotorLevel(0,0,1,1);break;
		case 2:setMotorLevel(0,0,1,0);break;
		case 3:setMotorLevel(0,1,1,0);break;
		case 4:setMotorLevel(0,1,0,0);break;
		case 5:setMotorLevel(1,1,0,0);break;
		case 6:setMotorLevel(1,0,0,0);break;
		case 7:setMotorLevel(1,0,0,1);break;
		default:setMotorLevel(0,0,0,1);break;
	}
}

/**
  * @Name    MotorTurn
  * @brief   ULN2003组成的控制电路控制函数
  * @param   motorState: CW 正转 CCW 反转 STOP 停止
  * @retval  返回值
  */
void MotorTurn(unsigned char motorState)
{
    static int motor1Step = 0; //步进电机的当前角度
    
    if (motorState == CCW) //反转
    {
		motor1Step++;
		if (motor1Step > 7)
		{
			motor1Step = 0;
		}
    }else if (motorState == CW) //正转
    {
		motor1Step--;
		if (motor1Step < 0)
		{
			motor1Step = 7;
		}
	}
	
	motorPosSet(motor1Step);
}
