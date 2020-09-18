#ifndef _LIBDMA_H
#define _LIBDMA_H

#include "stm32f10x.h"

/************************************ TX DS18B20 *********************************/
void Init_DMA1_USART1_TX(void);
/************************************ RX DS18B20 *********************************/
void Init_DMA1_USART1_RX(void);


/************************************ TX iButton *********************************/
void Init_DMA1_USART3_TX(void);
/************************************ RX iButton *********************************/
void Init_DMA1_USART3_RX(void);



/*************** обмен "память-DMA1-USART1-DMA1-память (DS18B20) *****************/
void Exchange_DMA1_USART1(void);
/*************** обмен "память-DMA1-USART3-DMA1-память (iButton) *****************/
void Exchange_DMA1_USART3 (void);

#endif // LIBDMA_H
