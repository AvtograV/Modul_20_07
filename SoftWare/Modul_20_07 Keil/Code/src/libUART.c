#include "libUART.h"

extern uint8_t t_integer_current;															// переменная для сохранения текущего значения температуры
extern uint16_t number_of_measurements_MQ;										// кол-во измерений (выборка) для MQ

extern char ROM[];
extern char ROM_2[];

char buffer_RX_USART2 [size_buffer_reseive_USART2];
uint8_t num_bit_RX_USART2 = 0;

uint8_t FLAG_SIM900_STATUS = 0;
uint8_t FLAG_HC05_STATUS = 0;


/******************* USART1 (PA9 (Single Wire (Half-Duplex) (DS18B20) *******************/
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





/******************* USART2 (PA2 - TX, PA3 - RX) (HC-05 and SIM-900) *******************/
void Init_USART2_HC05_and_SIM900(void) {

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


/******************** отправить байт по USART2 (HC-05 and SIM900) **********************/
void USART2_Send_Char(char chr) {

	while(!(USART2 -> SR & USART_SR_TC));
	USART2 -> DR = chr;
}


/******************* отправить строку по USART2 (HC-05 and SIM900) *********************/
void USART2_Send_String(char *str) {	
	uint8_t i = 0;

	while (str[i])
		USART2_Send_Char(str[i++]);
}


/***************************************************************************************/
uint8_t contains (char* str, char* sequence) {
	 uint8_t num_of_coincidences = 0;
	 uint8_t i = 0;
	 uint8_t ii = 0;
	 
	while (sequence[i] != 0) {
		while (str[ii] != 0) {
			if(sequence[i] == str[ii]) {				
				num_of_coincidences++;				
			}
			ii++;
		}
		ii = 0;
		i++;
	}	
	return (num_of_coincidences == i);
}


/************************** SIM900 interrupt handler ***********************************/
void USART2_IRQHandler (void) {
	if (USART2 -> SR & USART_SR_RXNE)	{
		
	buffer_RX_USART2[num_bit_RX_USART2] = USART2 -> DR;											// записать каждый полученный бит в строку
  num_bit_RX_USART2++;
 
	if(buffer_RX_USART2[num_bit_RX_USART2 - 1] == '\r') {										// окончание строки

		// Set FLAG_HC05_STATUS
		if (contains (buffer_RX_USART2, "open")) {			
			GPIOB -> BSRR |= GPIO_BSRR_BS12;																		// open the lock (on solenoid coil - 1)		
			USART2_Send_String("D12 ON");
			USART2_Send_Char(0xD);
			USART2_Send_Char(0xA);
		}
		else if (contains (buffer_RX_USART2, "close")) {			
			USART2_Send_String("D12 OFF");
			GPIOB -> BSRR |= GPIO_BSRR_BR12;																		// close the lock (on solenoid coil - 0)
			USART2_Send_Char(0xD);
			USART2_Send_Char(0xA);
		}
		else if (contains (buffer_RX_USART2, "request")) {										// requist temp and СО2, after conected Bluetooth					
			t_integer_current = 255;
			temp_measure_request(ROM);				
			measure_and_send_result_MQ_135(number_of_measurements_MQ);
		}
		
		// Set FLAG_SIM900_STATUS
		else if (contains (buffer_RX_USART2, "ERROR")) {
			FLAG_SIM900_STATUS = 0;
		}
		else if (contains (buffer_RX_USART2, "OK")) {													// requist to SIM-900 (send "AT") or after sending SMS
			FLAG_SIM900_STATUS = 1;
		}
		else if (contains (buffer_RX_USART2, ">")) {													// willingness to send SMS text
			FLAG_SIM900_STATUS = 2;
		}
		else if (contains (buffer_RX_USART2, "RING")) {												// incom call
			FLAG_SIM900_STATUS = 3;
		}
 
		// Clear buffer USART2 RX
		for (uint8_t i = 0; i < size_buffer_reseive_USART2; i++) {
			if (buffer_RX_USART2[i] != 0) {
				buffer_RX_USART2[i] = 0;
			}
		}		
		num_bit_RX_USART2 = 0;
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
