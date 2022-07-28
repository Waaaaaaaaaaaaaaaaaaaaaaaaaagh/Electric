#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H



#include "stm32f10x.h"


#define KEY_1_GPIO_PIN                GPIO_Pin_8
#define KEY_1_GPIO_PORT               GPIOC
#define KEY_1_GPIO_CLK                RCC_APB2Periph_GPIOC


#define KEY_2_GPIO_PIN                GPIO_Pin_9
#define KEY_2_GPIO_PORT               GPIOC
#define KEY_2_GPIO_CLK                RCC_APB2Periph_GPIOC


#define EXTI_GPIO_CLK                 RCC_APB2Periph_AFIO


void EXTI_KEY_GPIO_Config(void);









#endif /* __BSP_EXTI_H */
