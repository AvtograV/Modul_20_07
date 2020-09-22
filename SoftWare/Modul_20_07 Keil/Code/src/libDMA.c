#include "libDMA.h"
#include "stm32f10x.h"

uint8_t buf_DS18B20_USART1_DMA1_tx[8];														// TX - буфер для передачи
uint8_t buf_DS18B20_USART1_DMA1_rx[8];														// RX	- буфер для приёма

uint8_t buf_iButton_USART3_DMA1_tx[8];
uint8_t buf_iButton_USART3_DMA1_rx[8];


/************************************ TX DS18B20 *********************************/
void Init_DMA1_USART1_TX(void)	{
	
	// Включить тактирование DMA1
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	DMA1_Channel4 -> CCR  &= ~DMA_CCR4_EN;      	
	
	DMA1_Channel4 -> CPAR = (uint32_t)& USART1 -> DR;
	DMA1_Channel4 -> CMAR = (uint32_t) buf_DS18B20_USART1_DMA1_tx;		// адрес буфера в памяти		

	DMA1_Channel4 -> CCR |= DMA_CCR4_DIR;															// read: memory -> periphery
	DMA1_Channel4 -> CCR |= DMA_CCR4_MINC;														// инкремент указателя памяти включен	
	DMA1_Channel4 -> CCR |= DMA_CCR4_TCIE;														// transfer complete interrupt enable
	
	USART1 				-> CR3 |=  USART_CR3_DMAT;													// enable DMA for USART1
}


/************************************ RX DS18B20 *********************************/
void Init_DMA1_USART1_RX(void)	{
	
	DMA1_Channel5 -> CCR  &= ~DMA_CCR5_EN;
	DMA1_Channel5 -> CPAR = (uint32_t) & USART1 -> DR;
	DMA1_Channel5 -> CMAR = (uint32_t) & buf_DS18B20_USART1_DMA1_rx;	// адрес начала буфера в памяти 	
	DMA1_Channel5 -> CCR |= DMA_CCR5_MINC;														// инкремент указателя памяти включен
	
	USART1 				-> CR3 |= USART_CR3_DMAR;
}


/******************** обмен "память-DMA1-USART1-DMA1-память **********************/
void Exchange_DMA1_USART1 (void) {
	
	DMA1_Channel4 -> CCR &= ~DMA_CCR4_EN;
	DMA1_Channel5 -> CCR &= ~DMA_CCR5_EN;
	
	DMA1_Channel4 -> CNDTR = sizeof(buf_DS18B20_USART1_DMA1_tx);			// указать количество передаваемых -
	DMA1_Channel5 -> CNDTR = sizeof(buf_DS18B20_USART1_DMA1_rx);			// принимаемых данных
		
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


/************************************ TX iButton *********************************/
void Init_DMA1_USART3_TX(void)	{
	
	RCC -> AHBENR |= RCC_AHBENR_DMA1EN;
	
	DMA1_Channel2 -> CCR  &= ~DMA_CCR2_EN;
	
	DMA1_Channel2 -> CPAR = (uint32_t)& USART3 -> DR;
	DMA1_Channel2 -> CMAR = (uint32_t) buf_iButton_USART3_DMA1_tx; 	// адрес буфера в памяти		

	DMA1_Channel2 -> CCR |= DMA_CCR2_DIR;														// read: memory -> periphery
	DMA1_Channel2 -> CCR |= DMA_CCR2_MINC;													// инкремент указателя памяти включен	
	DMA1_Channel2 -> CCR |= DMA_CCR2_TCIE;													// transfer complete interrupt enable
	
	USART3 				-> CR3 |=  USART_CR3_DMAT;												// enable DMA for USART1
}

/************************************ RX iButton *********************************/
void Init_DMA1_USART3_RX(void)	{
	
	DMA1_Channel3 -> CCR  &= ~DMA_CCR3_EN;
	DMA1_Channel3 -> CPAR = (uint32_t) & USART3 -> DR;
	DMA1_Channel3 -> CMAR = (uint32_t) & buf_iButton_USART3_DMA1_rx;	// адрес начала буфера в памяти 	
	DMA1_Channel3 -> CCR |= DMA_CCR3_MINC;														// инкремент указателя памяти включен
	
	USART3 				-> CR3 |= USART_CR3_DMAR;
}

/*************** обмен "память-DMA1-USART3-DMA1-память (iButton) *****************/
void Exchange_DMA1_USART3 (void) {
	
	DMA1_Channel2 -> CCR &= ~DMA_CCR2_EN;
	DMA1_Channel3 -> CCR &= ~DMA_CCR3_EN;
	
	DMA1_Channel2 -> CNDTR = sizeof(buf_iButton_USART3_DMA1_tx);			// указать количество передаваемых -
	DMA1_Channel3 -> CNDTR = sizeof(buf_iButton_USART3_DMA1_rx);			// принимаемых данных
		
	USART3 -> SR &= ~USART_SR_TC;

	USART3 -> CR3 &= ~USART_CR3_DMAR;
	USART3 -> CR3 |= USART_CR3_DMAR;

	DMA1_Channel2 -> CCR |= DMA_CCR2_EN;
	DMA1_Channel3 -> CCR |= DMA_CCR3_EN;

	// ждём полного завершения пересылки данных
	while((DMA1 -> ISR & (DMA_ISR_TCIF2 | DMA_ISR_TCIF3))!=
							(DMA_ISR_TCIF2 | DMA_ISR_TCIF3)) {}
				
	DMA1 -> IFCR |= DMA_IFCR_CTCIF2 | DMA_IFCR_CTCIF3;
				
	// ждём завершения передачи данных через USART
	while(!(USART3 -> SR & USART_SR_TC)) {}
}
