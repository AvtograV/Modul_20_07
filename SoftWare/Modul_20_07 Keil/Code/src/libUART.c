/******************************** Library ***********************************************/

#include "libUART.h"

/***************************** USART1 (PA9 - TX, PA10 - RX) *****************************/
void Init_USART1_for_1_Wire(void)
{

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	// настройка вывода PA9 (TX1) на режим альтернативной функции с активным выходом
	// биты CNF = 10, биты MODE = X1
	GPIOA->CRH &= (~GPIO_CRH_CNF9_0);
	GPIOA->CRH |= GPIO_CRH_CNF9_1;
	GPIOA->CRH |= GPIO_CRH_MODE9;

	USART1->CR3 |= USART_CR3_HDSEL; // Half duplex mode

	USART1->BRR = FCKL_APB2 / BAUDRATE_USART1;

	USART1->CR1 |= USART_CR1_RXNEIE;
	USART1->CR1 |= USART_CR1_TCIE;

	USART1->CR1 |= USART_CR1_TE;
	USART1->CR1 |= USART_CR1_RE;
	USART1->CR1 |= USART_CR1_UE;
}

/******************************* отправить байт по USART1 *******************************/
void USART1_Send_Char(char chr)
{
	while (!(USART1->SR & USART_SR_TC));
	
	USART1->DR = chr;
}

/****************************** отправить строку по USART1 ******************************/
void USART1_Send_String(char *str)
{
	uint8_t i = 0;
	while (str[i])
		USART1_Send_Char(str[i++]);
}

/*************************** изменить скорость USART1 (1-WIRE) **************************/
void change_speed_USART1(uint32_t set_speed)
{
	USART1->CR1 &= (uint32_t) ~(USART_CR1_TE);
	USART1->CR1 &= (uint32_t) ~(USART_CR1_RE);

	USART1->BRR = FCKL_APB2 / set_speed;

	USART1->CR1 |= (uint32_t)(USART_CR1_TE);
	USART1->CR1 |= (uint32_t)(USART_CR1_RE);
}





/****************************** USART2 (PA2 - TX, PA3 - RX) *****************************/
void Init_USART2_for_HC_05(void)
{

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	GPIOA->CRL |= GPIO_CRL_CNF2_1;
	GPIOA->CRL |= GPIO_CRL_MODE2;

	GPIOA->CRL |= GPIO_CRL_CNF3_0;
	GPIOA->CRL &= ~GPIO_CRL_MODE3;

	USART2->BRR = FCKL_APB1 / BAUDRATE_USART2;

	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RE;
	USART2->CR1 |= USART_CR1_UE;

	USART2->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART2_IRQn);
}


/******************************* отправить байт по USART2 *******************************/
void USART2_Send_Char(char chr)
{

	while (!(USART2->SR & USART_SR_TC))
		;
	USART2->DR = chr;
}


/****************************** отправить строку по USART2 ******************************/
void USART2_Send_String(char *str)
{

	uint8_t i = 0;

	while (str[i])
		USART2_Send_Char(str[i++]);
}
