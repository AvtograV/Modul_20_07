#include "lib_vTask.h"

const uint8_t number_of_measurements_MQ = 100;									// кол-во измерений (выборка) для MQ

extern uint8_t flag_EXTI_PB10;

extern char ROM_1[];
extern char ROM_7[];

/********** task - calling various functions without time delays **********/
void vTaskFunCallWithoutDelay(void *argument) {
	while(1) {
		
		getString_USART2();																							 // listen constantly
		getString_USART3();																							 // USART2 and USART3
		
		autoStaticTemp(ROM_7, 27);		
	}
}


/********** задача - вызов различных функций через определённый промежуток времени **********/
void vTaskByTimeFunctionCall (void *argument) {
	while(1) {
		
		if_smoke_in_room(measure_MQ_135(number_of_measurements_MQ));

		if_coolant_temp_low (temp_measure_request(ROM_1));		
		if_coolant_temp_low (temp_measure_request(ROM_7));
		
		vTaskDelay(_10_minutes);
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
