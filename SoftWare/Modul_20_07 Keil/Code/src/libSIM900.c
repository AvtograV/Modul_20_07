#include "libSIM900.h"

/******************* USART2 (PA2 - TX, PA3 - RX) (HC-05 and SIM-900) *******************/

const char number_phone[] 	= "+79025623095";
const char number_phone_2[] = "+79676211100";

char text_sms [size_text_sms];
extern char buffer_RX_USART2 [size_buffer_reseive_USART2];

extern uint8_t FLAG_SIM900_STATUS;

const uint8_t max_num_requests = 5;														// reguest "AT" <-> "OK"


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
uint8_t send_sms (char* num_phone, char* send_text_sms) {	

	FLAG_SIM900_STATUS = 0;
	
	for (uint8_t i = 0; i < max_num_requests; i++) {
				
		USART2_Send_String("AT+CMGS=\"");
		USART2_Send_String(num_phone);
		USART2_Send_Char('\"');
		USART2_Send_Char(0xD);
			
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
uint8_t incomCall (void) {
	
	if (FLAG_SIM900_STATUS == 3) {
		
		FLAG_SIM900_STATUS = 0;
		vTaskDelay(3000);
		USART2_Send_String("ATH0");		
	}	
	return FLAG_SIM900_STATUS;
}
