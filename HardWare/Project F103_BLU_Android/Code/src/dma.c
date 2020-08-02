#include "stm32f10x.h"
#include "dma.h"

uint8_t ow_t_buf[8];																							// Буфер для передачи по 1-wire
uint8_t ow_r_buf[8];																							// Буфер для приёма по 1-wire


/*****************************************************************************************************************/
void USART1_TX_DMA_Init(void)	{
	
	// Включить тактирование DMA1
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	DMA1_Channel4 -> CCR  &= ~DMA_CCR4_EN;      	
	
	DMA1_Channel4 -> CPAR = (uint32_t)& USART1 -> DR;
	DMA1_Channel4 -> CMAR = (uint32_t) ow_t_buf;										// адрес буфера в памяти		

	DMA1_Channel4 -> CCR |= DMA_CCR4_DIR;														// read: memory -> periphery
	DMA1_Channel4 -> CCR |= DMA_CCR4_MINC;													// инкремент указателя памяти включен	
	DMA1_Channel4 -> CCR |= DMA_CCR4_TCIE;													// transfer complete interrupt enable
	
	USART1 				-> CR3 |=  USART_CR3_DMAT;												// enable DMA for USART1
}


/*****************************************************************************************************************/
void USART1_RX_DMA_Init(void)	{
	
	DMA1_Channel5 -> CCR  &= ~DMA_CCR5_EN;
	DMA1_Channel5 -> CPAR = (uint32_t) & USART1 -> DR;
	DMA1_Channel5 -> CMAR = (uint32_t) & ow_r_buf[0];								// адрес начала буфера в памяти 	
	DMA1_Channel5 -> CCR |= DMA_CCR5_MINC;													// инкремент указателя памяти включен
	
	USART1 				-> CR3 |= USART_CR3_DMAR;
}


/******************************* обмен в канале "память-DMA1-USART2-DMA1-память *********************************/
void exchange_mem_DMA_USART (void) {
	
	DMA1_Channel4 -> CCR &= ~DMA_CCR4_EN;
	DMA1_Channel5 -> CCR &= ~DMA_CCR5_EN;
	
	DMA1_Channel4 -> CNDTR = sizeof(ow_t_buf); 												// указать количество передаваемых -
	DMA1_Channel5 -> CNDTR = sizeof(ow_r_buf);												// принимаемых данных
		
	// Перед пересылкой данных через USART с использованием DMA, требуется
	// сбросить флаг TC записью в него 0.
	USART1 -> SR &= ~USART_SR_TC;

	// Datasheet требует сброса этого бита после завершения передачи блока данных
	// с помощью DMA; здесь сброс и установка бита выполняются последовательно.
	USART1 -> CR3 &= ~USART_CR3_DMAR;
	USART1 -> CR3 |= USART_CR3_DMAR;

	// Включаем каналы DMA (первым - канал для получения данных от USART).
	DMA1_Channel4 -> CCR |= DMA_CCR4_EN;
	DMA1_Channel5 -> CCR |= DMA_CCR5_EN;

	// Ждём полного завершения пересылки данных
	// (что сильно снижает полезность применения DMA в этом примере).
	while((DMA1 -> ISR & (DMA_ISR_TCIF4 | DMA_ISR_TCIF5))!=
							(DMA_ISR_TCIF4 | DMA_ISR_TCIF5)) {}
				
	DMA1 -> IFCR |= DMA_IFCR_CTCIF4 | DMA_IFCR_CTCIF5;
				
	// Ждём завершения передачи данных через USART
	while(!(USART1 -> SR & USART_SR_TC)) {}
}
