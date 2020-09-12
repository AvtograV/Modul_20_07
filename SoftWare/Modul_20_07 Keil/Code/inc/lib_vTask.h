#ifndef _lib_vTask_H
#define _lib_vTask_H 

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lib1Wire.h"
#include "libHc05.h"
#include "libExit.h"

/********************* задача - измерение температуры DS18B20 ( 1-Wire) *********************/
	void vTaskMeasureTemp (void *argument);
	
/*************** задача - измерение температуры (по запросу) DS18B20 ( 1-Wire) ***************/
	void vTaskMeasureTempRequest (void *argument);
	
/************* задача - обработка прерываний (запросов) от ОС Android по USART2 *************/
	void vTaskUSART2_IRQHandlerAndroid (void *argument);
	
/**************** задача - обработка внешнего прерывания на PA9 от iButton *****************/
	void vTaskEXTI9_5_IRQHandler (void *argument);

#endif 			// lib_vTask_H
