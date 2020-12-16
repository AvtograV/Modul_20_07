#include "libHc05.h"

uint8_t FLAG_AUTO = 0;
uint8_t FLAG_HC05_STATUS = 0;


void getCommands_HC_05 (char* str) {

	if (contains (str, "FORCED_VENT_ON")) {
		GPIOB -> BSRR |= GPIO_BSRR_BS12;
			if (GPIOB -> ODR & GPIO_ODR_ODR12)
				USART2_Send_String("FORCED_ON\r\n");
		}			
	else if (contains (str, "FORCED_VENT_OFF")) {
		GPIOB -> BSRR |= GPIO_BSRR_BR12;
			if (~(GPIOB -> ODR & GPIO_ODR_ODR12))
				USART2_Send_String("FORCED_OFF\r\n");
		}		
	else if (contains (str, "EXHAUST_VENT_ON")) {
		GPIOB -> BSRR |= GPIO_BSRR_BS13;																		
			if (GPIOB -> ODR & GPIO_ODR_ODR13)
				USART2_Send_String("EXHAUST_ON\r\n");
		}		
	else if (contains (str, "EXHAUST_VENT_OFF")) {
		GPIOB -> BSRR |= GPIO_BSRR_BR13;
			if (~(GPIOB -> ODR & GPIO_ODR_ODR13))
				USART2_Send_String("EXHAUST_OFF\r\n");
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
				
			FLAG_HC05_STATUS = 1;																	// (FLAG_HC05_STATUS == 1) - temp_measure_request(ROM_1)
	}																													// or temp_measure_request(ROM_7) in lib_vTask.c
}
