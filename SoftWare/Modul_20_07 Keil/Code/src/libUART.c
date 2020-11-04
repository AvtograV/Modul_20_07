#include "libUART.h"

extern uint8_t t_integer_current;															// переменная для сохранения текущего значения температуры
extern uint16_t number_of_measurements_MQ;										// кол-во измерений (выборка) для MQ

extern char ROM_7[];

char buffer_RX_USART2 [size_buffer_reseive_USART2];
char* P_buf_RX = &buffer_RX_USART2[0];												// P_buf_RX указатель на buffer_RX_USART2[0]

uint8_t num_bit_RX_USART2 = 0;

uint8_t FLAG_SIM900_STATUS = 0;
uint8_t FLAG_HC05_STATUS = 0;
uint8_t FLAG_GETSTRING_STATUS = 0;


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

/************* узнать - содержит ли строка определённую последовательность **************/
uint8_t contains (char* str, char* sequence) {
	
	uint8_t i_seq, i_str, seq_lenght, num_of_coincidences;				// num_of_coincidences - число(кол-во) совпадений символов
		i_seq = i_str = seq_lenght = num_of_coincidences = 0;
	
	while (sequence[seq_lenght] != 0x00)													// find out the lenght of the "sequence"
		seq_lenght++;
	
	while (sequence[i_seq] != str[i_str]													// find out what "str[i_str]" matches start with...
		&& i_str < size_buffer_reseive_USART2)
		i_str++;
	
	while (sequence[i_seq] == str[i_str] && i_seq < seq_lenght) {
			i_seq++;
			i_str++;
			num_of_coincidences++;		
	}
	return seq_lenght == num_of_coincidences;
}


/************************** SIM900 interrupt handler ***********************************/
void USART2_IRQHandler (void) {
	if (USART2 -> SR & USART_SR_RXNE)	{
		
	buffer_RX_USART2[num_bit_RX_USART2++] = USART2 -> DR;										// записать каждый полученный бит в строку

	if (buffer_RX_USART2[num_bit_RX_USART2 - 1] == '\r') {									// окончание строки
			FLAG_GETSTRING_STATUS = 1;
   }
  }
}


/***************************** получить строку по USART2 *******************************/
void getString_USART2 (void) {
	while (FLAG_GETSTRING_STATUS != 0) {
		
		if (contains (buffer_RX_USART2, "OPEN")) {												// open the lock (on solenoid coil - 1)
				GPIOB -> BSRR |= GPIO_BSRR_BS12;																		
				USART2_Send_String("D12 ON\r\n");
			
				USART2_Send_String(buffer_RX_USART2);
			}
		else if (contains (buffer_RX_USART2, "CLOSE")) {									// close the lock (on solenoid coil - 0)
				GPIOB -> BSRR |= GPIO_BSRR_BR12;
				USART2_Send_String("D12 OFF\r\n");
			
				USART2_Send_String(buffer_RX_USART2);
		}
		else if (contains (buffer_RX_USART2, "REQUEST")) {								// requist temp and СО2, after conected Bluetooth
				t_integer_current = 255;
				temp_measure_request(ROM_7);
				measure_and_send_result_MQ_135(number_of_measurements_MQ);
			
				USART2_Send_String(buffer_RX_USART2);
		}
		
			// Set FLAG_SIM900_STATUS
		else if (contains (buffer_RX_USART2, "ERROR")) {
				FLAG_SIM900_STATUS = 0;
			
			USART2_Send_String(buffer_RX_USART2);
		}
		else if (contains (buffer_RX_USART2, "OK")) 	{											// requist to SIM-900 (send "AT") or after sending SMS
				FLAG_SIM900_STATUS = 1;
			
			USART2_Send_String(buffer_RX_USART2);
		}
			
		else if (contains (buffer_RX_USART2, "RING")) {											// incom call
				FLAG_SIM900_STATUS = 3;
			
			USART2_Send_String(buffer_RX_USART2);
		}
			
		else if (contains (buffer_RX_USART2, "+CMTI: \"SM\",1")) {
			USART2_Send_String("AT+CMGL=\"REC UNREAD\"\r\n");
				
			USART2_Send_String(buffer_RX_USART2);
		}

		num_bit_RX_USART2 = FLAG_GETSTRING_STATUS = 0;
		
		for (uint8_t i = 0; i < size_buffer_reseive_USART2; i++) {					// clear buffer_RX_USART2
			*(P_buf_RX + i) = 0x00;
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
