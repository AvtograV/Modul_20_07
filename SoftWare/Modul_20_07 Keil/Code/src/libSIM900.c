#include "libSIM900.h"


/********************** pin 0 - TX, pin 1 - RX (to Arduino board) **********************/
/******************* USART2 (PA2 - TX, PA3 - RX) (HC-05 and SIM-900) *******************/


char sms_number_phone [] = "+79676211100";
char text_sms [size_text_sms] = {0};

extern char buffer_RX_USART2 [size_buffer_reseive_USART2];


/******************* check SIM900 and send his status to USART *******************/
		void check_sim900 (void) {

		USART2_Send_String("AT");												// send a SIM900 command
		USART2_Send_Char(0xD);													// carriage return ("Enter")		
		
		vTaskDelay(300);
			
		USART2_IRQHandler();														// get string from SIM900
			
			if (buffer_RX_USART2[5] == 'O' & buffer_RX_USART2[6] == 'K') {
			
		// если OK, что-то делаем
			}
			else {
				// или ошибка
			}		
	}


/******************************* on/off module SIM900 ******************************/
void sim900_on (void) {
	
		GPIOA -> BSRR |= GPIO_BSRR_BS3;
		vTaskDelay(1000);
		GPIOA -> BSRR |= GPIO_BSRR_BR3;
		vTaskDelay(5000);
		check_sim900 ();
}


/****************************** send SMS with any text *****************************/
uint8_t send_sms (char* num_phone, char* send_text_sms) {

	USART2_Send_String("AT");												// send a SIM900 command
	USART2_Send_Char(0xD);													// carriage return ("Enter")	
		
	vTaskDelay(300);
			
	USART2_IRQHandler();														// get string from SIM900
			
	if (buffer_RX_USART2[5] == 'O' & buffer_RX_USART2[6] == 'K') {				
				
		USART2_Send_String("AT+CMGS=\"");
		USART2_Send_String(num_phone);
		USART2_Send_Char('\"');
		USART2_Send_Char(0xD);

		vTaskDelay(300);
				
		USART2_IRQHandler();													// get string from SIM900
				
		if (buffer_RX_USART2[34] == '>') {
					
			USART2_Send_String(send_text_sms);
			vTaskDelay(1);
			USART2_Send_Char(0x1A);
					
			return SMS_OK;
		}
		
		else {
			return SMS_ERROR;
		}
	}
	return 0;
}
