#ifndef _LibADC_H
#define _LibADC_H 

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "libUART.h"


/***************************************** PA1 **********************************************/
void Init_ADC1_MQ135 (void);

/************** измерить некое кол-во раз (number_of_measurements_MQ) MQ-135 ***************/
uint16_t measure_MQ_135 (uint16_t number_of_measurements);

/*********************************** измерить CO2 (MQ-135) **********************************/	
uint16_t MQ135_measure_request(void);

/** функция нахождения среднего арифметического числа от всего количества измерений MQ-135 **/
uint16_t arithmetic_mean_number(uint8_t number_of_measur);


/* 	2059
		2561
		2039
		2071
		2063
		2053
		2048
		2022
		2016	*/

#endif 			// LibADC_H
