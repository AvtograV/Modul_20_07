#include "lib_vTask.h"

extern uint8_t flag_EXTI9;
const uint32_t valueDelay = 1;


	/************* задача - обработка прерываний (запросов) от ОС Android по USART2 *************/
	void vTaskUSART2_IRQHandlerAndroid (void *argument) {
		while(1) {
			USART2_IRQHandler();
		}
	}
	
	
	/**************** задача - обработка внешнего прерывания на PA9 от iButton *****************/
	void vTaskEXTI9_5_IRQHandler (void *argument) {
		for (;;) {
       if (flag_EXTI9) {
				 vTaskDelay(10);
				 i_Button();				
				 flag_EXTI9 = 0;
			}
		}
	}

	/********** задача - вызов различных функций через определённый промежуток времени **********/
	void vTaskPeriodicFunctionCall (void *argument) {
		while(1) {
			temp_measure_request();
			vTaskDelay(valueDelay);
		}
	}
