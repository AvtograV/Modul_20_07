#include "libHc05.h"

uint8_t FLAG_AUTO = 0;

extern char ROM_1[];
extern char ROM_7[];

extern char t_buffer_char[5];																					// массив для символьного значения температуры
extern char str_MQ_135[5];																						// массив для символьных значений с MQ


void getCommands_HC_05 (char* str) {

	if (contains (str, "VENT_IN_BEE_ON")) {
		GPIOB -> BSRR |= GPIO_BSRR_BS12;
			if (GPIOB -> ODR & GPIO_ODR_ODR12)
				USART2_Send_String("VENT_IN_BEE_ON\r\n");
			else
				USART2_Send_String("ERROR_VENT\r\n");
		}

	else if (contains (str, "VENT_IN_BEE_OFF")) {
		GPIOB -> BSRR |= GPIO_BSRR_BR12;
			if (~(GPIOB -> ODR & GPIO_ODR_ODR12))
				USART2_Send_String("VENT_IN_BEE_OFF\r\n");
			else
				USART2_Send_String("ERROR_VENT\r\n");
		}
	
		
	else if (contains (str, "VENT_OUT_BEE_ON")) {
		GPIOB -> BSRR |= GPIO_BSRR_BS13;																		
			if (GPIOB -> ODR & GPIO_ODR_ODR13)
				USART2_Send_String("VENT_OUT_BEE_ON\r\n");
			else
				USART2_Send_String("ERROR_VENT\r\n");
		}
	
	else if (contains (str, "VENT_OUT_BEE_OFF")) {
		GPIOB -> BSRR |= GPIO_BSRR_BR13;
			if (~(GPIOB -> ODR & GPIO_ODR_ODR13))
				USART2_Send_String("VENT_OUT_BEE_OFF\r\n");
			else
				USART2_Send_String("ERROR_VENT\r\n");
		}		
	
		
// activation of the automatic mode of maintaining the set temperature
	else if (contains (str, "AUTO_ON")) {
		FLAG_AUTO = 1;
			USART2_Send_String("AUTO_ON\r\n");
	}		
	else if (contains (str, "AUTO_OFF")) {
		FLAG_AUTO = 0;
			USART2_Send_String("AUTO_OFF\r\n");
	}		
	
	
	else if (contains (str, "REQUEST")) {											// REQUEST, after connected Bluetooth
				
		if (GPIOB -> ODR & GPIO_ODR_ODR12)
			USART2_Send_String("FORCED_ON\r\n");
				
		if (GPIOB -> ODR & GPIO_ODR_ODR13)
			USART2_Send_String("EXHAUST_ON\r\n");
				
		if (FLAG_AUTO == 1)
			USART2_Send_String("AUTO_ON\r\n");
				
		
			utoa_cycle_sub_2(temp_measure_request(ROM_1), t_buffer_char);
			USART2_Send_String("temp right ");
			USART2_Send_String(t_buffer_char);
			USART2_Send_Char('\r'); 																				// возврат каретки (carriage return, CR) — 0x0D, '\r'
			USART2_Send_Char('\n'); 																				// перевод на строку вниз(line feed, LF) — 0x0A, '\n'

			utoa_cycle_sub_2(temp_measure_request(ROM_7), t_buffer_char);
			USART2_Send_String("temp left ");
			USART2_Send_String(t_buffer_char);
			USART2_Send_Char('\r');
			USART2_Send_Char('\n');
	}					
}
