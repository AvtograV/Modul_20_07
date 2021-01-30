#include "libAUTO.h"

const uint8_t delta_temp = 2;

uint16_t threshold_smoke = 700;
uint16_t critical_value_smoke = 750;

extern uint8_t FLAG_AUTO;
extern uint8_t FLAG_SIM900_ALERT;

char t_buffer_char[5];																					// массив для символьного значения температуры
char str_MQ_135[5];																							// массив для символьных значений с MQ

int SetTempToWinterHouse = 0;

uint8_t FLAG_DELAY_FORSED_VENT = 0;

const uint16_t pow10Table4_16[] = {1000ul, 100ul, 10ul, 1ul};
const uint16_t pow10Table2_16[] = {10ul, 1ul};


/************* функция преобразование числового значения в символьное (2 знака) *************/
char* utoa_cycle_sub_2(uint16_t value, char *buffer) {
	if (value == 0)
	{
		buffer[0] = '0';
		buffer[1] = 0;
		return buffer;
	}

	char *ptr = buffer;
	uint8_t i = 0;

	do
	{
		uint16_t pow10 = pow10Table2_16[i++];
		uint8_t count = 0;

		while (value >= pow10)
		{
			count++;
			value -= pow10;
		}

		*ptr++ = count + '0';

	} while (i < 2);

	*ptr = 0;

	while (buffer[0] == '0') // удаляем ведущие нули
		++buffer;

	return buffer;
}


/************* функция преобразование числового значения в символьное (4 знака) *************/
char* utoa_cycle_sub_4(uint16_t value, char *buffer) {
   if (value == 0)  {
				buffer[0] = '0';
				buffer[1] = 0;
			return buffer;
   }
	 
   char *ptr = buffer;
	 
  uint8_t i = 0;
	 
   do { uint16_t pow10 = pow10Table4_16[i++];
		 
  uint8_t count = 0;
		 
		while(value >= pow10) {
			count ++;
			value -= pow10;
		}
	 
     *ptr++ = count + '0';
  }
	 
		while(i < 4);
			*ptr = 0;
	 
			// удаляем ведущие нули
   while(buffer[0] == '0') ++buffer;
   return buffer;
}


/********************************************************************************************/
void autoStaticTemp (char* ROM, uint8_t temp) {
	if (FLAG_AUTO == 1) {
		if (temp_measure_request(ROM) >= (temp + delta_temp) &&
			FLAG_DELAY_FORSED_VENT == 0) {
				
			GPIOB -> BSRR |= GPIO_BSRR_BS12;
			FLAG_DELAY_FORSED_VENT = 1;
		}
		else if (temp_measure_request(ROM) <= (temp - delta_temp) &&
			FLAG_DELAY_FORSED_VENT == 1) {
				GPIOB -> BSRR |= GPIO_BSRR_BR12;
				FLAG_DELAY_FORSED_VENT = 0;
		}
	}
}

/********************************************************************************************/
void if_smoke_in_room (uint16_t current_smoke) {
	if (current_smoke >= threshold_smoke &&															// increased gas content
			FLAG_SIM900_ALERT == NO_VALUE) {
		
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");
		vTaskDelay(half_second);

		USART3_Send_String("ATTENTION\r\nSMOKE VALUE ");
		USART3_Send_String(utoa_cycle_sub_4(current_smoke, str_MQ_135));
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);
	}
	else if (current_smoke >= critical_value_smoke &&										// dangerous gas pollution
			FLAG_SIM900_ALERT == NO_VALUE) {
				
			USART3_Send_String("ATD\"+79676211100\"\r\n");									// phone call
	}
}

/********************************************************************************************/
void if_coolant_temp_low (uint8_t current_temp) {}
