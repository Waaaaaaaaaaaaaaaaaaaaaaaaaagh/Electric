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
	OLED_init();
	OLED_clear();
	while(1)
	{
		//OLED_show8x16number(5,15,3);
	}
}
