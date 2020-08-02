#include "stm32f10x.h"
#include "usart_1.h"


/**************************************** PA9 - TX, PA10 - RX ****************************************/
void USART1_Init_for_ONE_WIRE (void)	{
	
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	
	// настройка вывода PA9 (TX1) на режим альтернативной функции с активным выходом
	// биты CNF = 10, биты MODE = X1
	GPIOA -> CRH &= (~GPIO_CRH_CNF9_0);
	GPIOA -> CRH |= GPIO_CRH_CNF9_1;
	GPIOA -> CRH |= GPIO_CRH_MODE9;
	
	USART1 -> CR3 |= USART_CR3_HDSEL;																		// Half duplex mode
	
	USART1 -> BRR = FCKL_APB2 / BAUDRATE_USART1;
	
	USART1 -> CR1 |= USART_CR1_RXNEIE;
	USART1 -> CR1 |= USART_CR1_TCIE;
	
	USART1 -> CR1 |= USART_CR1_TE;
	USART1 -> CR1 |= USART_CR1_RE;
	USART1 -> CR1 |= USART_CR1_UE;
}


/************************************* временная функция задержки *************************************/
void delay (uint32_t milles) {
		uint32_t mil = 0;
	while (mil < milles)
		mil++;
}


/************************************** отправить байт по USART1 **************************************/
void USART1_Send_Char (char chr){	
	while (!(USART1 -> SR & USART_SR_TC));
	USART1 -> DR = chr;	
}


/************************************* отправить строку по USART1 *************************************/
void USART1_Send_String (char* str){	
	uint8_t i = 0;	
	while(str[i])
	USART1_Send_Char (str[i++]);	
}

/********************************** изменить скорость USART1 (1-WIRE) *********************************/
void change_speed_USART1 (uint32_t set_speed) {
	USART1 -> CR1 &= (uint32_t)~(USART_CR1_TE);
	USART1 -> CR1 &= (uint32_t)~(USART_CR1_RE);
	
	USART1 -> BRR = FCKL_APB2 / set_speed;
	
	USART1 -> CR1 |= (uint32_t)(USART_CR1_TE);
	USART1 -> CR1 |= (uint32_t)(USART_CR1_RE);
}
