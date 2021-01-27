#include "libSIM900.h"


extern char ROM_1[];
extern char ROM_7[];
	
extern char t_buffer_char[5];															// массив для символьного значения температуры
extern char str_MQ_135[5];																// массив для символьных значений с MQ	

extern const uint8_t number_of_measurements_MQ;						// кол-во измерений (выборка) для MQ

uint8_t FLAG_SIM900_ALERT = NO_VALUE;

/******************************* commands from sim900 (USART3) ******************************/
void execute_commands_from_sim900(char* str) {
	
	if (contains (str, "LIGHT ON")) {
			GPIOB -> BSRR |= GPIO_BSRR_BS12;
		// delete read messages
			USART3_Send_String("AT+CMGDA=\"DEL READ\"\r\n");
	}	
	else if (contains (str, "LIGHT OFF")) {
			GPIOB -> BSRR |= GPIO_BSRR_BR12;
			USART3_Send_String("AT+CMGDA=\"DEL READ\"\r\n");
	}
	
	else if (contains (str, "RING")) {															// incom call
		vTaskDelay(1000);
		
		USART3_Send_String("ATH0\r\n");																// phone is busy
		vTaskDelay(1000);
		
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");
		
		vTaskDelay(500);
		
		USART3_Send_String(utoa_cycle_sub_4
										(arithmetic_mean_number(number_of_measurements_MQ), str_MQ_135));
		USART3_Send_String("\r\n");
		
		// USART3_Send_String( utoa_cycle_sub_2(temp_measure_request(ROM_1),	t_buffer_char));
		// USART3_Send_String("\r\n");
		// USART3_Send_String( utoa_cycle_sub_2(temp_measure_request(ROM_7), t_buffer_char));
		// USART3_Send_String("\r\n");		
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	else if (contains (str, "\x3E\x20\r\n")) {							// 0x20 - "space" charecter, x3E - '<'
	
	}		
	else if (contains (str, "+CMTI: \"SM\",")) {
			USART3_Send_String("AT+CMGL=\"REC UNREAD\"\r\n");
	}
	
	// if the user has confirmed receipt of the notification
	else if (contains (str, "Smoke")) {		
		FLAG_SIM900_ALERT = SMOKE_ALERT_RESEIVED;		
	}
	
	// message from the user - ARMED STATUS
	else if (contains (str, "Null")) {
		
		FLAG_SIM900_ALERT = NO_VALUE;
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		USART3_Send_String("ARMED STATUS\r\n");		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
}
