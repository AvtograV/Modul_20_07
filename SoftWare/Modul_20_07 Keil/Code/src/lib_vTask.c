#include "lib_vTask.h"

extern uint8_t flag_EXTI_PB10;
extern uint8_t t_integer_current;
const uint16_t valueDelay = 1000;
const uint8_t number_of_measurements_MQ = 100;									// кол-во измерений (выборка) для MQ


	/************* задача - обработка прерываний (запросов) от ОС Android по USART2 *************/
	void vTaskUSART2_IRQHandlerAndroid (void *argument) {
		while(1) {
			USART2_IRQHandler();
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
			temp_measure_request();
			
			measure_and_send_result_MQ_135(number_of_measurements_MQ);
	}
}
