#include "dma.h"
#include "usartplus.h"

uint8_t printf_dma_flag=0;

static void DMA_NVIC_Init()
{
	NVIC_InitTypeDef temp;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	temp.NVIC_IRQChannelPreemptionPriority=1;
	temp.NVIC_IRQChannelSubPriority=0;
	temp.NVIC_IRQChannel=DMA2_Channel4_5_IRQn;
	temp.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&temp);
	DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE);
}

void to_extern_Init()
{
	DMA_InitTypeDef temp;
	RCC_AHBPeriphClockCmd(DMA_CLK,ENABLE);
	temp.DMA_PeripheralBaseAddr=(uint32_t)(&(UART4->DR));
	temp.DMA_MemoryBaseAddr=(uint32_t)send_buff;
	temp.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	temp.DMA_PeripheralDataSize=DMA_MemoryDataSize_Byte;
	temp.DMA_DIR=DMA_DIR_PeripheralSRC;
	temp.DMA_BufferSize=1;
	temp.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	temp.DMA_MemoryInc=DMA_MemoryInc_Enable;
	temp.DMA_M2M = DMA_M2M_Disable;
	temp.DMA_Mode=DMA_Mode_Normal;
	temp.DMA_Priority=DMA_Priority_High;
	DMA_Init(DMA2_Channel5,&temp);
	DMA_Cmd(DMA2_Channel5,DISABLE);
	DMA_NVIC_Init();
}

void DMA2_Channel4_5_IRQHandler()
{
	if(DMA_GetITStatus(DMA2_IT_TC5)!=RESET)
	{
		printf_dma_flag=0;
		DMA_Cmd(DMA2_Channel5,DISABLE);
		USART_DMACmd(myUSARTx,USART_DMAReq_Tx,DISABLE);
		DMA_ClearITPendingBit(DMA2_IT_TC5);
	}
}

