#ifndef _lib_vTask_H
#define _lib_vTask_H 

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lib1Wire.h"
#include "libExit.h"
#include "sysInit.h"
#include "libSIM900.h"
#include "libUART.h"


/******************************* задача - USART2_IRQHandler ********************************/
	void vTaskUSART2_IRQHandler(void *argument);
	
/**************** задача - обработка внешнего прерывания на PB10 от iButton *****************/
	void vTaskEXTI10_IRQHandler (void *argument);
	
/********** задача - вызов различных функций через определённый промежуток времени **********/
	void vTaskPeriodicFunctionCall (void *argument);


#endif 			// lib_vTask_H
