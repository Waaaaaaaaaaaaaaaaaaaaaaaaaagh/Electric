#include "usartplus.h"
#include "dma.h"
#include <stdarg.h>

uint8_t send_buff[PRINT_SIZE];

static void dma_size_cmd(int number)
{
    printf_dma_flag=1;
    DMA2_Channel5->CNDTR=number*4+4;
    DMA2_Channel5->CMAR=(uint32_t)send_buff;
		//USART_DMACmd(myUSARTx,USART_DMAReq_Tx,ENABLE);
		DMA_Cmd(DMA2_Channel5,ENABLE);
}

static void RTX_Init()
{
	RCC_APB2PeriphClockCmd(TX_CLK|RX_CLK,ENABLE);
	GPIO_InitTypeDef temp;
	temp.GPIO_Pin=TX_Pin;
	temp.GPIO_Mode=GPIO_Mode_AF_PP;
	temp.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(TX_Port,&temp);
	temp.GPIO_Pin=RX_Pin;
	temp.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(RX_Port,&temp);
}

static void USART_NVIC_Init()
{
	NVIC_InitTypeDef temp;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	temp.NVIC_IRQChannelPreemptionPriority=1;
	temp.NVIC_IRQChannelSubPriority=1;
	temp.NVIC_IRQChannel=USARTx_IRQ;
	temp.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&temp);
    USART_ITConfig(myUSARTx, USART_IT_RXNE, ENABLE);
}

void MYUSART_Init()
{
    RTX_Init();
	RCC_APB1PeriphClockCmd(USARTx_CLK,ENABLE);
	USART_InitTypeDef temp;
	temp.USART_WordLength=USART_WordLength_8b;
	temp.USART_BaudRate=115200;
	temp.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	temp.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	temp.USART_Parity=USART_Parity_No;
	temp.USART_StopBits=USART_StopBits_1;
	USART_Init(myUSARTx,&temp);
	USART_Cmd(myUSARTx,ENABLE);
	USART_DMACmd(myUSARTx,USART_DMAReq_Tx,ENABLE);
	USART_NVIC_Init();
}

void USARTx_handler()
{
	if (USART_GetITStatus(myUSARTx, USART_IT_RXNE) != RESET)
    {
		USART_ClearITPendingBit(myUSARTx,USART_IT_RXNE);
    }
}

void print_plus(char*fmt,...)
{
    if(printf_dma_flag)return;
    int number=0;
    float fval;
    va_list ap;
    va_start(ap,fmt);
    for(char*p=fmt;*p!='\0';p++){
        if(*p!='%')continue;
        switch(*(++p))
        {
            case 'd':
                fval=(float)va_arg(ap,int);
                //uart_putbuff(SENT_UART,(uint8*)&fval,sizeof(float));
                *((float*)(send_buff+number*4))=fval;
                number++;
                break;
            case 'l':
                if(*(p+1)=='f'){
                    p++;
                    fval=(float)va_arg(ap,double);
                }
                else if(*(p+1)=='l')
                {
                    p++;
                    fval=(float)va_arg(ap,long long);
                }
                //uart_putbuff(SENT_UART,(uint8*)&fval,sizeof(float));
                *((float*)(send_buff+number*4))=fval;
                number++;
                break;
            case 'f':
                fval=(float)va_arg(ap,double);
                //uart_putbuff(SENT_UART,(uint8*)&fval,sizeof(float));
                *((float*)(send_buff+number*4))=fval;
                number++;
                break;
        }
        if(number*4>PRINT_SIZE)return;
    }
    va_end(ap);
    send_buff[(number)*4+0]=0x00;
    send_buff[(number)*4+1]=0x00;
    send_buff[(number)*4+2]=0x80;
    send_buff[(number)*4+3]=0x7f;
    dma_size_cmd(number);
    //uart_putbuff(SENT_UART,tail,4);
}
