/**********************718 创新实验室开发板例程*********************
*  编写：718 创新实验室
*  平台：STM32F103RCT6
*  说明：由于作者的水平有限，若有不足之处，还请大家谅解。
*		 建议大家多看看数据手册。
******************************************************************/
#include "stm32f10x.h"
#include "include.h"
#include  "usartplus.h"
#include "dma.h"
#include "solution.h"

// 将 ADC1 转换的电压值通过 DMA 方式传到 SRAM
extern __IO uint16_t ADC_ConvertedValue1[LOFCHANEL][NOFCHANEL];
extern __IO uint16_t ADC_ConvertedValue2[LOFCHANEL][NOFCHANEL];
extern uint8_t ADC_ConvertedValue1_flag;//置一表示数组一已经传输完毕
extern uint8_t ADC_ConvertedValue2_flag;

// 局部变量，用于保存转换计算后的电压值
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
    		// 初始化ADC 校准寄存器
    		ADC_ResetCalibration(ADC_x);
    		// 等待校准寄存器初始化完成
    		while (ADC_GetResetCalibrationStatus(ADC_x));
    		// ADC开始校准
    		ADC_StartCalibration(ADC_x);
    		// 等待校准完成
    		while (ADC_GetCalibrationStatus(ADC_x));
    		// 由于没有采用外部触发，所以使用软件触发ADC转换
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
