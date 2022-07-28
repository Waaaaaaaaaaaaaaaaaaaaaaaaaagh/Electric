/**********************718 ����ʵ���ҿ���������*********************
*  ��д��718 ����ʵ����
*  ƽ̨��STM32F103RCT6
*  ˵�����������ߵ�ˮƽ���ޣ����в���֮�����������½⡣
*		 �����Ҷ࿴�������ֲᡣ
******************************************************************/
#include "stm32f10x.h"
#include "include.h"
#include  "usartplus.h"
#include "dma.h"
#include "stdlib.h"

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
	GPIO_EXIT_Init();
	OLED_clear();
	MYUSART_Init();
	to_extern_Init();

	Servo_reset();
	ray_on();

	while(1)
	{
		//OLED_show8x16number(5,15,3);
		//print_plus("%d%d%d%d",rand()%2,rand()%3,rand()%4,rand()%5);
		//Usart_SendByte(UART4,0x01);
		Delay_MS(100);
	}
}
