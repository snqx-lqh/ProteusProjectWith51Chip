#include "step_motor.h"

/**
  * @Name    setMotorLevel
  * @brief   ULN2003��ɵĿ��Ƶ�·���ƺ���
  * @param   �������ŵĵ�ƽ
  * @retval  ����ֵ
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
  * @brief   ���õ����ÿ��λ�õ����ŵ�ƽ 0-7 ��������� 8 �����������
  * @param   motorPos: ���ת��λ��
  * @retval  ����ֵ
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
  * @brief   ULN2003��ɵĿ��Ƶ�·���ƺ���
  * @param   motorState: CW ��ת CCW ��ת STOP ֹͣ
  * @retval  ����ֵ
  */
void MotorTurn(unsigned char motorState)
{
    static int motor1Step = 0; //��������ĵ�ǰ�Ƕ�
    
    if (motorState == CCW) //��ת
    {
		motor1Step++;
		if (motor1Step > 7)
		{
			motor1Step = 0;
		}
    }else if (motorState == CW) //��ת
    {
		motor1Step--;
		if (motor1Step < 0)
		{
			motor1Step = 7;
		}
	}
	
	motorPosSet(motor1Step);
}
