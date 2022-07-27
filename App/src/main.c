/**********************718 ����ʵ���ҿ���������*********************
*  ��д��718 ����ʵ����
*  ƽ̨��STM32F103RCT6
*  ˵�����������ߵ�ˮƽ���ޣ����в���֮�����������½⡣
*		 �����Ҷ࿴�������ֲᡣ
******************************************************************/
#include "stm32f10x.h"
#include "include.h"

// �� ADC1 ת���ĵ�ѹֵͨ�� DMA ��ʽ���� SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// �ֲ����������ڱ���ת�������ĵ�ѹֵ
float ADC_ConvertedValueLocal[NOFCHANEL];

int main()
{
	TIM_SERVO_Init();
	OLED_init();
	Delay_Init();
	ray_init();
	OLED_clear();


	Servo_reset();

	while(1)
	{
		ray_on();
		Servo_drive(0,SERVO1_TIM_CH1_PIN);/* �ײ��� *//* X �� *//* Yaw �� */
		Servo_drive(0,SERVO1_TIM_CH2_PIN);/* ������ *//* Y �� *//* Pitch �� */
		//Test_Servo();
	}
}
