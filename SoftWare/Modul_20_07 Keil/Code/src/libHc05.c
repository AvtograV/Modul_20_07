#include "libHc05.h"

extern uint8_t t_integer_current;								// переменная для сохранения текущего значения температуры

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
					
				t_integer_current = 0;
				temp_measure_request();
				}
		}
}
