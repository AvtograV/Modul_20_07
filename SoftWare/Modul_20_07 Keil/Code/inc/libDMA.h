#ifndef _LIBDMA_H
#define _LIBDMA_H

#include "stm32f10x.h"

/*******************************************************************************************************/
void Init_DMA1_USART1_TX(void);
void Init_DMA1_USART1_RX(void);

/******************************* обмен "память-DMA1-USART1-DMA1-память *********************************/
void Exchange_DMA1_USART1(void);

#endif // LIBDMA_H
