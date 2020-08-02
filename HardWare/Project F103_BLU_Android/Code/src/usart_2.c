#include "stm32f10x.h"
#include "usart_2.h"


/**************************************** PA2 - TX, PA3 - RX ****************************************/
void USART2_Init_for_HC_05 (void){
	
	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
	
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	GPIOA -> CRL |= GPIO_CRL_CNF2_1;
	GPIOA -> CRL |= GPIO_CRL_MODE2;
	
	GPIOA -> CRL |= GPIO_CRL_CNF3_0;
	GPIOA -> CRL &= ~GPIO_CRL_MODE3;
	
	USART2  -> BRR = FCKL_APB1 / BAUDRATE_USART2;
	
	USART2 -> CR1 |= USART_CR1_TE;
	USART2 -> CR1 |= USART_CR1_RE;
	USART2 -> CR1 |= USART_CR1_UE;
	
	USART2 -> CR1 |= USART_CR1_RXNEIE;	
	NVIC_EnableIRQ(USART2_IRQn);	
}


/************************************** отправить байт по USART2 **************************************/
void USART2_Send_Char (char chr){
	
	while (!(USART2 -> SR & USART_SR_TC));
	USART2 -> DR = chr;	
}


/************************************* отправить строку по USART2 *************************************/
void USART2_Send_String (char* str){
	
	uint8_t i = 0;
	
	while(str[i])
	USART2_Send_Char (str[i++]);	
}
