/**********************718 创新实验室开发板例程*********************
*  编写：718 创新实验室
*  平台：STM32F103RCT6
*  说明：由于作者的水平有限，若有不足之处，还请大家谅解。
*		 建议大家多看看数据手册。
******************************************************************/
#include "stm32f10x.h"
#include "include.h"

// 将 ADC1 转换的电压值通过 DMA 方式传到 SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// 局部变量，用于保存转换计算后的电压值
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
		Servo_drive(0,SERVO1_TIM_CH1_PIN);/* 底层舵机 *//* X 轴 *//* Yaw 轴 */
		Servo_drive(0,SERVO1_TIM_CH2_PIN);/* 顶层舵机 *//* Y 轴 *//* Pitch 轴 */
		//Test_Servo();
	}
}
