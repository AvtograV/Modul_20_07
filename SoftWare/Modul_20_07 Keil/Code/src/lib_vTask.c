#include "lib_vTask.h"

const uint8_t number_of_measurements_MQ = 100;									// кол-во измерений (выборка) для MQ

extern uint8_t flag_EXTI_PB10;

extern char ROM_1[];
extern char ROM_7[];

extern uint8_t FLAG_HC05_STATUS;

char t_buffer_char[] = {0};																						// массив для символьного значения температуры

/********** task - calling various functions without time delays **********/
void vTaskFunCallWithoutDelay(void *argument) {
	while(1) {
		getString_USART2();

	// on request
		while (FLAG_HC05_STATUS == 1){																		// if (FLAG_HC05_STATUS == 1) - REQUEST after 
																																			// connecting via bluetooth				
			utoa_cycle_sub(temp_measure_request(ROM_1), t_buffer_char);
			USART2_Send_String("temp right ");
			USART2_Send_String(t_buffer_char);
			USART2_Send_Char('\r'); 																				// возврат каретки (carriage return, CR) — 0x0D, '\r'
			USART2_Send_Char('\n'); 																				// перевод на строку вниз(line feed, LF) — 0x0A, '\n'
				
			utoa_cycle_sub(temp_measure_request(ROM_7), t_buffer_char);
			USART2_Send_String("temp left ");
			USART2_Send_String(t_buffer_char);
			USART2_Send_Char('\r');
			USART2_Send_Char('\n');
				
			FLAG_HC05_STATUS = 0;
		}				
		
		incomCall(send_parameters_to_511100);
		autoStaticTemp(ROM_7, 27);
	}
}

	
/********** задача - вызов различных функций через определённый промежуток времени **********/
void vTaskByTimeFunctionCall (void *argument) {
	while(1) {
			
	vTaskDelay(10000);
			
		utoa_cycle_sub(temp_measure_request(ROM_1), t_buffer_char);
		USART2_Send_String("temp right ");
		USART2_Send_String(t_buffer_char);
		USART2_Send_Char('\r');
		USART2_Send_Char('\n');
				
		utoa_cycle_sub(temp_measure_request(ROM_7), t_buffer_char);
		USART2_Send_String("temp left ");
		USART2_Send_String(t_buffer_char);
		USART2_Send_Char('\r');
		USART2_Send_Char('\n');
		
		measure_and_send_result_MQ_135(number_of_measurements_MQ);
	}
}


/**************** задача - обработка внешнего прерывания на PB10 от iButton *****************/
void vTaskEXTI10_IRQHandler (void *argument) {
	for (;;) {
		if (flag_EXTI_PB10) {
			i_Button();
			flag_EXTI_PB10 = 0;
		}
	}
}
