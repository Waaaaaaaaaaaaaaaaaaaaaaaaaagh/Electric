#ifndef __USART__
#define __USART__

#include "stm32f10x.h"
#include "stm32f10x_usart.h"

#define TX_Pin GPIO_Pin_10
#define TX_Port GPIOC
#define TX_CLK RCC_APB2Periph_GPIOC

#define RX_Pin GPIO_Pin_11
#define RX_Port GPIOC
#define RX_CLK RCC_APB2Periph_GPIOC

#define myUSARTx UART4
#define USARTx_CLK RCC_APB1Periph_UART4
#define USARTx_IRQ UART4_IRQn
#define USARTx_handler UART4_IRQHandler

#define PRINT_SIZE 52

void MYUSART_Init(void);
void print_plus(char*fmt,...);

#endif

