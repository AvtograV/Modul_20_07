#include "libSIM900.h"

/******************* USART2 (PA2 - TX, PA3 - RX) (HC-05 and SIM-900) *******************/
const uint8_t max_num_requests = 5;														// reguest "AT" <-> "OK"
uint8_t FLAG_SIM900_STATUS = 0;

//char text_sms [size_text_sms];


/********************** check SIM900 and send his status to USART **********************/
uint8_t check_sim900 (void) {		
		FLAG_SIM900_STATUS = 0;

		USART2_Send_String("AT");																			// send a SIM900 command
		USART2_Send_Char(0xD);																				// carriage return ("Enter")		
		vTaskDelay(500);
			
		return FLAG_SIM900_STATUS;
	}


/********************************* on/off module SIM900 ********************************/
uint8_t sim900_on (void) {
	
		GPIOA -> BSRR |= GPIO_BSRR_BS3;
		vTaskDelay(1000);
		GPIOA -> BSRR |= GPIO_BSRR_BR3;
		vTaskDelay(5000);

		return check_sim900();
}


/******************************** send SMS with any text *******************************/
uint8_t send_sms (char* send_sms_tell, char* send_text_sms) {	

	FLAG_SIM900_STATUS = 0;
	
	for (uint8_t i = 0; i < max_num_requests; i++) {				
		USART2_Send_String(send_sms_tell);			
		vTaskDelay(500);
		
		if (FLAG_SIM900_STATUS == 2) {																// willingness to send SMS text
			i = max_num_requests;
					
			USART2_Send_String(send_text_sms);
			vTaskDelay(1);
			USART2_Send_Char(0x1A);
		}
	}
	return FLAG_SIM900_STATUS;
}


/******************************* answering an incoming call ******************************/
uint8_t incomCall (char* comm_send_sms_to_tell) {
	
	if (FLAG_SIM900_STATUS == 3) {																	// FLAG_SIM900_STATUS = "RING"
		FLAG_SIM900_STATUS = 0;
		vTaskDelay(2000);
		
		USART2_Send_String("ATH0\r\n");																// phone is busy
		vTaskDelay(1000);
		
		USART2_Send_String(comm_send_sms_to_tell);										// send_parameters_to_511100						"AT+CMGS=\"+79025623095\"\r\n"

//		while (!(FLAG_SIM900_STATUS == 2)) {}													// FLAG_SIM900_STATUS = ">\r\n"
																																	// willingness to send SMS text
		FLAG_SIM900_STATUS = 0;
		
		vTaskDelay(500);	
			
		measure_and_send_result_MQ_135(100);
		USART2_Send_String("\r\n");
		
//		temp_measure_request(ROM_1, "temp right ");
//		temp_measure_request(ROM_7, "temp left ");
		
		vTaskDelay(1);	
		USART2_Send_Char(0x1A);																				// sub
	}
	return 1;
}


void getCommands_SIM_900(char* str) {

	if (contains (str, "ERROR")) {
		FLAG_SIM900_STATUS = 0;
	}		
	else if (contains (str, "OK")) 	{												// requist to SIM-900 (send "AT") or after sending SMS
		FLAG_SIM900_STATUS = 1;
	}	
	else if (contains (str, "RING")) {											// incom call
			FLAG_SIM900_STATUS = 3;
	}		
	else if (contains (str, "\x3E\x20\r\n")) {							// 0x20 - "space" charecter, x3E - '<'
			FLAG_SIM900_STATUS = 2;
	}		
	else if (contains (str, "+CMTI: \"SM\",")) {
			USART2_Send_String("AT+CMGL=\"REC UNREAD\"\r\n");
	}	
	else if (contains (str, "LIGHT ON")) {			
			GPIOB -> BSRR |= GPIO_BSRR_BS12;
			USART2_Send_String("AT+CMGDA=\"DEL READ\"\r\n");
	}		
	else if (contains (str, "LIGHT OFF")) {
			GPIOB -> BSRR |= GPIO_BSRR_BR12;
			USART2_Send_String("AT+CMGDA=\"DEL READ\"\r\n");
	}
}
