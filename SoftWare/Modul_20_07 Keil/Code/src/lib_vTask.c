#include "lib_vTask.h"

const uint8_t number_of_measurements_MQ = 100;									// кол-во измерений (выборка) для MQ

extern uint8_t flag_EXTI_PB10;
extern uint8_t t_integer_current;

extern char ROM_7[];

	/******************************* задача - USART2_IRQHandler ********************************/
	void vTaskUSART2_IRQHandler(void *argument) {
		while(1) {
			incomCall(send_parameters_to_511100);
			getString_USART2();
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

	
	/********** задача - вызов различных функций через определённый промежуток времени **********/
	void vTaskPeriodicFunctionCall (void *argument) {
		while(1) {			
			temp_measure_request(ROM_7);
			measure_and_send_result_MQ_135(number_of_measurements_MQ);
			
			vTaskDelay(60000);
	}
}
