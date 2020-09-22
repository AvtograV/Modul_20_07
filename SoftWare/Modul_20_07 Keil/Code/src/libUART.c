#include "libUART.h"

extern uint8_t t_integer_current;								// переменная для сохранения текущего значения температуры


/******************* USART1 (PA9 (Single Wire (Half-Duplex) (DS18B20 *******************/
void Init_USART1_DS18B20(void) {

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

/************************** отправить байт по USART1 (DS18B20) *************************/
void USART1_Send_Char(char chr)
{
	while (!(USART1->SR & USART_SR_TC));
	
	USART1->DR = chr;
}

/************************ отправить строку по USART1 (DS18B20) *************************/
void USART1_Send_String(char *str)
{
	uint8_t i = 0;
	while (str[i])
		USART1_Send_Char(str[i++]);
}

/************************** изменить скорость USART1 (DS18B20) *************************/
void change_speed_USART1(uint32_t set_speed)
{
	USART1->CR1 &= (uint32_t) ~(USART_CR1_TE);
	USART1->CR1 &= (uint32_t) ~(USART_CR1_RE);

	USART1->BRR = FCKL_APB2 / set_speed;

	USART1->CR1 |= (uint32_t)(USART_CR1_TE);
	USART1->CR1 |= (uint32_t)(USART_CR1_RE);
}





/************************* USART2 (PA2 - TX, PA3 - RX) (HC-05) *************************/
void Init_USART2_HC_05(void)
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


/************************** отправить байт по USART2 (HC-05) ***************************/
void USART2_Send_Char(char chr)
{

	while (!(USART2->SR & USART_SR_TC))
		;
	USART2->DR = chr;
}


/************************* отправить строку по USART2 (HC-05) **************************/
void USART2_Send_String(char *str)
{

	uint8_t i = 0;

	while (str[i])
		USART2_Send_Char(str[i++]);
}


/********************** принять байт от HC-05 по USART2 **********************/
void USART2_IRQHandler(void) {
	if (USART2 -> SR & USART_SR_RXNE) {
			if (USART2 -> DR == 'a') {
				GPIOB -> BSRR |= GPIO_BSRR_BS12;									// open the lock (on solenoid coil - 1)
					USART2_Send_String("D12 ON");
					USART2_Send_Char(0xD);
					USART2_Send_Char(0xA);
				}
			else if (USART2 -> DR == 'A') {
					USART2_Send_String("D12 OFF");
				GPIOB -> BSRR |= GPIO_BSRR_BR12;									// close the lock (on solenoid coil - 0)
					USART2_Send_Char(0xD);
					USART2_Send_Char(0xA);
				}
			else if (USART2 -> DR == 'B') {
					USART2_Send_String("D12 ON");
					USART2_Send_Char(0xD);
					USART2_Send_Char(0xA);
				}
			else if (USART2 -> DR == 'r') {					// запрос температуры сразу после подключения по Bluetooth
					
				t_integer_current = 0;
				temp_measure_request();
				}
		}
}


/********************** USART3 (PB10 (Single Wire (Half-Duplex) ************************/
void Init_USART3_iButton(void) {

	RCC -> APB1ENR |= RCC_APB1ENR_USART3EN;
	RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN;

	// настройка вывода PB10 на режим альтернативной функции с активным выходом
	// биты CNF = 10, биты MODE = X1
	GPIOB -> CRH &= (~GPIO_CRH_CNF10_0);
	GPIOB -> CRH |= GPIO_CRH_CNF10_1;
	GPIOB -> CRH |= GPIO_CRH_MODE10;

	USART3 -> CR3 |= USART_CR3_HDSEL;												// Half duplex mode

	USART3 -> BRR = FCKL_APB1 / BAUDRATE_USART3;

	USART3 -> CR1 |= USART_CR1_RXNEIE;
	USART3 -> CR1 |= USART_CR1_TCIE;

	USART3 -> CR1 |= USART_CR1_TE;
	USART3 -> CR1 |= USART_CR1_RE;
	USART3 -> CR1 |= USART_CR1_UE;
}


/****************************** отправить байт по USART3 *******************************/
void USART3_Send_Char(char chr)
{
	while (!(USART3 -> SR & USART_SR_TC));
	
	USART3 -> DR = chr;
}

/***************************** отправить строку по USART3 ******************************/
void USART3_Send_String(char *str)
{
	uint8_t i = 0;
	while (str[i])
		USART3_Send_Char(str[i++]);
}

/************************* изменить скорость USART3 (iButton) **************************/
void change_speed_USART3(uint32_t set_speed)
{
	USART3 -> CR1 &= (uint32_t) ~(USART_CR1_TE);
	USART3 -> CR1 &= (uint32_t) ~(USART_CR1_RE);

	USART3 -> BRR = FCKL_APB1 / set_speed;

	USART3 -> CR1 |= (uint32_t)(USART_CR1_TE);
	USART3 -> CR1 |= (uint32_t)(USART_CR1_RE);
}
