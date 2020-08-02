#include "rcc.h"
#include "stm32f10x.h"


/*******************************************************************************************************/
void RCC_Init (void){
	
	RCC -> CR |= ((uint32_t)RCC_CR_HSEON); 												// Enable HSE
	while (!(RCC->CR & RCC_CR_HSERDY));														// Ready start HSE		
	
	FLASH -> ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;					// Cloclk Flash memory
	
	RCC -> CFGR |= RCC_CFGR_HPRE_DIV1;														// AHB = SYSCLK / 1
	RCC -> CFGR |= RCC_CFGR_PPRE1_DIV2;														// APB1 = HCLK / 2
	RCC -> CFGR |= RCC_CFGR_PPRE2_DIV1;														// APB2 = HCLK / 1
	
	RCC -> CFGR &= ~RCC_CFGR_PLLMULL;               							// clear PLLMULL bits
	RCC -> CFGR &= ~RCC_CFGR_PLLSRC;															// clearn PLLSRC bits
	RCC -> CFGR &= ~RCC_CFGR_PLLXTPRE;														// clearn PLLXTPRE bits
	
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE;															// source HSE
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE;														// source HSE/1 = 8 MHz
	RCC->CFGR |= RCC_CFGR_PLLMULL9;																// PLL x9: clock = 8 MHz * 9 = 72 MHz
	
	RCC -> CR |= RCC_CR_PLLON;                      							// enable PLL (включаем множитель частоты)
	while((RCC -> CR & RCC_CR_PLLRDY) == 0) {}      							// wait till PLL is ready
	
	RCC -> CFGR &= ~RCC_CFGR_SW;                   							 	// clear SW bits
  RCC -> CFGR |= RCC_CFGR_SW_PLL;                 							// select source SYSCLK = PLL
	while((RCC -> CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1) {} 			// wait till PLL is used	
}


/*******************************************************************************************************/
void MCO_out (void){																						// Master Clock Output (PA8)
	
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;                       	// enable clock for port A
	
	GPIOA -> CRH &= ~GPIO_CRH_CNF8_0;															// setting out alternative push-pull for PA8
	GPIOA -> CRH |= GPIO_CRH_CNF8_1;
	GPIOA -> CRH |= (GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1);				// gpio speed 50 MHz
	
	RCC -> CFGR |= RCC_CFGR_MCO_SYSCLK;														// select source clokc SYSCLK	
}
