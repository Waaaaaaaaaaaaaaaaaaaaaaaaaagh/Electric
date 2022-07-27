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

// 将 ADC1 转换的电压值通过 DMA 方式传到 SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// 局部变量，用于保存转换计算后的电压值
float ADC_ConvertedValueLocal[NOFCHANEL];

int main()
{
	//TIM_SERVO_Init();
	OLED_init();
	Delay_Init();
	ray_init();
	OLED_clear();
	MYUSART_Init();
	to_extern_Init();
	while(1)
	{
		//OLED_show8x16number(5,15,3);
		print_plus("%d",1);
		Delay_MS(100);
	}
}
