#include "stm32f10x.h"
#include "hc_05.h"
#include "usart_2.h"
#include "one_wire.h"

/********************** принять байт от HC-05 по USART1 **********************/
void USART2_IRQHandler(void) {
	if (USART2 -> SR & USART_SR_RXNE) {
			if (USART2 -> DR == 'A') {
					USART2_Send_String("D10 ON");
					USART2_Send_Char(0xD);
					USART2_Send_Char(0xA);
				}
			else if (USART2 -> DR == 'a') {
					USART2_Send_String("D11 ON");
					USART2_Send_Char(0xD);
					USART2_Send_Char(0xA);
				}
			else if (USART2 -> DR == 'B') {
					USART2_Send_String("D12 ON");
					USART2_Send_Char(0xD);
					USART2_Send_Char(0xA);
				}
				else if (USART2 -> DR == 'r') {					// запрос температуры сразу после подключения по Bluetooth
					temp_measur_request ();
				}
		}
}

/******************* отправить что-то по USART2 для HC-05 *******************/
void USART2_Send_Command_HC_05 (void) {		
	for (int i = 0; i < 2; i++) {
		USART2_Send_String("temp " );
		
		char buffer_integer[] = {0};
		utoa_cycle_sub(i, buffer_integer);
		USART2_Send_String (buffer_integer);
		
		USART2_Send_Char(0xD);									// возврат каретки (carriage return, CR) — 0x0D, '\r'
		USART2_Send_Char(0xA);									// перевод на строку вниз(line feed, LF) — 0x0A, '\n'
	}	
}
