#ifndef _LibADC_H
#define _LibADC_H 

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "libUART.h"


/*********************************** PA1 ***************************************/
void Init_ADC1_MQ135 (void);

/************ измерить CO2 (MQ-135) и отправить в приложение Android ***********/	
	void MQ135_measure_request(void);

#endif 			// LibADC_H
