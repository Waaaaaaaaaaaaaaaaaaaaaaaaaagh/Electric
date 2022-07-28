#ifndef _SERVO_H_
#define _SERVO_H_

#include "stdint.h"

#define	RCC_APBPeriph_TIMx_GPIOx1			RCC_APB2Periph_GPIOA
#define	RCC_APBPeriph_TIMx1					RCC_APB2Periph_TIM1
#define	SERVO1_PORT							GPIOA
#define SERVO1_TIM							TIM1
#define SERVO1_Low					        GPIO_Pin_8
#define SERVO1_High					        GPIO_Pin_9
#define SERVO1_TIM_CH3_PIN					GPIO_Pin_10
#define SERVO1_TIM_CH4_PIN					GPIO_Pin_11

#define	RCC_APBPeriph_TIMx_GPIOx2			RCC_APB2Periph_GPIOB
#define	RCC_APBPeriph_TIMx2					RCC_APB1Periph_TIM2
#define	SERVO2_PORT							GPIOB
#define SERVO2_TIM							TIM2
//#define SERVO2_TIM_CH1_PIN						GPIO_Pin_8
#define SERVO2_TIM_CH2_PIN					GPIO_Pin_3
#define SERVO2_TIM_CH3_PIN					GPIO_Pin_10
#define SERVO2_TIM_CH4_PIN					GPIO_Pin_11


#define	RCC_APBPeriph_TIMx_GPIOx3			RCC_APB2Periph_GPIOA
#define	RCC_APBPeriph_TIMx3					RCC_APB1Periph_TIM2
#define	SERVO3_PORT							GPIOA
#define SERVO3_TIM							TIM2
#define SERVO3_TIM_CH1_PIN					GPIO_Pin_15
//#define SERVO3_TIM_CH2_PIN						GPIO_Pin_3
//#define SERVO3_TIM_CH3_PIN						GPIO_Pin_10
//#define SERVO3_TIM_CH4_PIN						GPIO_Pin_11

//#define fix
#ifndef fix
#define common
#endif

extern void TIM_SERVO_Init(void);
extern void Servo_Open(void);
extern void Servo_Close(void);
extern void Test_Servo(void);
extern void Servo_drive(float angle,uint16_t Ch);
extern void Servo_reset(void);
#endif
