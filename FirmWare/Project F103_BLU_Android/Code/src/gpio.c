#include "stm32f10x.h"


void GPIO_Init (void){
	
	// PA9 - TX, PA10 - RX
	
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;                       		// enable clock for port A
	
	GPIOA -> CRH |= GPIO_CRH_MODE10_1;															// output mode, max speed 10 MHz.
	GPIOA -> CRH &= ~GPIO_CRH_CNF10;
	
	GPIOA -> BSRR |= GPIO_BSRR_BS10;
}
