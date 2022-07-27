#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include <stdio.h>

// 串口宏定义，不同的串口挂载的总线不一样，移植时需要修改这几个宏
#define USARTx                     UART5
#define USART_CLK                  RCC_APB1Periph_UART5
#define USART_APBxClkCmd           RCC_APB1PeriphClockCmd
#define USART_BAUDRATE             115200

// USART GPIO 引脚宏定义
#define USART_GPIO_CLK             (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
#define USART_GPIO_APBxClkCmd      RCC_APB2PeriphClockCmd

#define USART_TX_GPIO_PORT         GPIOC
#define USART_TX_GPIO_PIN          GPIO_Pin_12
#define USART_RX_GPIO_PORT         GPIOD
#define USART_RX_GPIO_PIN          GPIO_Pin_2

#define USART_IRQ                  UART5_IRQn
#define USART_IRQHandler           UART5_IRQHandler

extern void USART_Config(void);
extern void Test_ADC_PRINTF(void);
extern void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
extern void Usart_SendString(USART_TypeDef *pUSARTx, char *str);
extern void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);

#endif /* __USART_H__ */
