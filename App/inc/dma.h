#ifndef __DMA__
#define __DMA__

#include "stm32f10x.h"
#include "stm32f10x_dma.h"

#define DMA_CLK RCC_AHBPeriph_DMA2
#define BufferSize 8

extern uint8_t printf_dma_flag;
extern uint8_t send_buff[52];

void to_mem_Init(void);
void to_extern_Init(void);

#endif

