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
#include "solution.h"

// �� ADC1 ת���ĵ�ѹֵͨ�� DMA ��ʽ���� SRAM
extern __IO uint16_t ADC_ConvertedValue1[LOFCHANEL][NOFCHANEL];
extern __IO uint16_t ADC_ConvertedValue2[LOFCHANEL][NOFCHANEL];
extern uint8_t ADC_ConvertedValue1_flag;//��һ��ʾ����һ�Ѿ��������
extern uint8_t ADC_ConvertedValue2_flag;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ
//float ADC_ConvertedValueLocal[NOFCHANEL];

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
	ADCx_Init();
	

	Servo_reset();
	ray_on();

	while(1)
	{
		if(ADC_ConvertedValue1_flag||ADC_ConvertedValue2_flag)
		{
			DMA_Cmd(ADC_DMA_CHANNEL,ENABLE);
			ADC_Cmd(ADC_x, ENABLE);
    		// ��ʼ��ADC У׼�Ĵ���
    		ADC_ResetCalibration(ADC_x);
    		// �ȴ�У׼�Ĵ�����ʼ�����
    		while (ADC_GetResetCalibrationStatus(ADC_x));
    		// ADC��ʼУ׼
    		ADC_StartCalibration(ADC_x);
    		// �ȴ�У׼���
    		while (ADC_GetCalibrationStatus(ADC_x));
    		// ����û�в����ⲿ����������ʹ���������ADCת��
    		ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
			if(ADC_ConvertedValue1_flag==1)
			{
				ADC_ConvertedValue1_flag=0;
				for_max_time(ADC_ConvertedValue1);
			}
			else 
			{
				ADC_ConvertedValue2_flag=0;
				for_max_time(ADC_ConvertedValue2);
			}
		}
	}
}
