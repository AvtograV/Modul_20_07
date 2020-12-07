#include "lib_vTask.h"

const uint8_t number_of_measurements_MQ = 100;									// кол-во измерений (выборка) для MQ

extern uint8_t flag_EXTI_PB10;
extern uint8_t t_integer_current;

extern char ROM_1[];
extern char ROM_7[];

extern uint8_t FLAG_HC05_STATUS;
extern uint8_t t_integer_current;

	/******************************* задача - USART2_IRQHandler ********************************/
	void vTaskUSART2_IRQHandler(void *argument) {
		while(1) {
			getString_USART2();			
			incomCall(send_parameters_to_511100);
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

	
	/*** задача - вызов различных функций через определённый промежуток времени или по запросу ***/
	void vTaskPeriodicFunctionCall (void *argument) {
		while(1) {
			if (FLAG_HC05_STATUS == 1){
				t_integer_current = 255;
				temp_measure_request(ROM_1, "temp right ");
				FLAG_HC05_STATUS++;
			} 
			if (FLAG_HC05_STATUS == 2) {
				t_integer_current = 255;
				temp_measure_request(ROM_7, "temp left ");
				FLAG_HC05_STATUS = 0;
			}
			
			temp_measure_request(ROM_1, "temp right ");
			temp_measure_request(ROM_7, "temp left ");
			measure_and_send_result_MQ_135(number_of_measurements_MQ);
			vTaskDelay(10000);
	}
}
