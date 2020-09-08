#include "lib_vTask.h"


/********************* задача - измерение температуры DS18B20 ( 1-Wire) *********************/
	void vTaskMeasureTemp (void *argument) {
		while(1) {
			measure_temperature();
		}
	}

	
/*************** задача - измерение температуры (по запросу) DS18B20 ( 1-Wire) ***************/
	void vTaskMeasureTempRequest (void *argument) {
		while(1) {
			temp_measure_request();
		}
	}
	
	
	/************* задача - обработка прерываний (запросов) от ОС Android по USART2 *************/
	void vTaskUSART2_IRQHandlerAndroid (void *argument) {
		while(1) {
			USART2_IRQHandler();
		}
	}
	
	
	/******************** задача - получение серийного номера ключа iButton *********************/
	void vTaskGetSerialNumberKeyI_Button (void *argument) {
		while(1) {
			i_Button();
		}
	}
