#include "bsp_exti.h"
#include "bsp_tim.h"
#include <stdio.h>


int i = 10000;
int key = 0;
uint16_t j = 72 - 1;
float DutyRatio;
float F;


static void EXTI_NVIC_Config(void)//防止别的函数调用
{
  NVIC_InitTypeDef            NVIC_Initstruct;	
	NVIC_Initstruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Initstruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Initstruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstruct);
}



void EXTI_KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef         GPIO_InitStruct;
	EXTI_InitTypeDef         EXTI_InitStruct;
	
	
	EXTI_NVIC_Config();
	
	
	//初始化GPIO
	RCC_APB2PeriphClockCmd(KEY_1_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY_1_GPIO_PIN | KEY_2_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY_1_GPIO_PORT, &GPIO_InitStruct);
	GPIO_Init(KEY_2_GPIO_PORT, &GPIO_InitStruct);
	
	
	//初始化EXTI
	RCC_APB2PeriphClockCmd(EXTI_GPIO_CLK, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
	
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line8 | EXTI_Line9;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	
	
	EXTI_Init(&EXTI_InitStruct);
	
	
}





void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)/* 外侧 */
		 {
			 if(key == 0)
				 key = 1;
			 if(key == 1)
			 {
				i += 100;
				TIM_SetCompare2(TIM,i);
			 }
			 if(key == 2)
			 {
				 j += 1;
				 TIM2 ->PSC = j;
			 }
			 EXTI_ClearITPendingBit(EXTI_Line8);
		 }
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)/* 内侧 */
		 {
			if(key == 0)
				key = 2;
			if(key == 1)
			{
				i -= 100;
				TIM_SetCompare2(TIM,i);
			}
			if(key == 2)
			 {
				 j -= 1;
				 TIM2 ->PSC = j;
			 }
			 EXTI_ClearITPendingBit(EXTI_Line9);
		 }
		 
		 DutyRatio = (float)(TIM2 ->CCR2/(float)(TIM2 ->ARR))*100;
		 F = (float)(((TIM2 ->ARR + 1)*(TIM2 ->PSC + 1))/72000.0);
		 
		 
			 printf("PWM的占空比和频率分别为:%f,%f\n",DutyRatio,F);			 
	
}





