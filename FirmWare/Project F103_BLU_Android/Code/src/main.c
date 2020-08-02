#include "main.h"
#include "stm32f10x.h"
#include "rcc.h"
#include "usart_1.h"
#include "usart_2.h"
#include "hc_05.h"
#include "dma.h"
#include "one_wire.h"





int main(void){
	
	RCC_Init ();
	
	USART1_Init_for_ONE_WIRE ();
	USART2_Init_for_HC_05 ();
		
	USART1_TX_DMA_Init();
	USART1_RX_DMA_Init();	

	
	while(1)
	{
		measure_temperature ();
		USART2_IRQHandler();
	}	 
}
