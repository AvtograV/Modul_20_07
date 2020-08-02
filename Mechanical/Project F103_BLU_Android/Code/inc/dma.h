#ifndef _DMA_H
#define _DMA_H

#include "stm32f10x.h"

void exchange_mem_DMA_USART (void);

/*****************************************************************************************************************/
void USART1_TX_DMA_Init(void);
void USART1_RX_DMA_Init(void);


#endif 			// DMA_H
