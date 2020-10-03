#ifndef _LibADC_H
#define _LibADC_H 

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "libUART.h"


/***************************************** PA1 **********************************************/
void Init_ADC1_MQ135 (void);

/**** измерить некое кол-во раз (number_of_measurements) MQ-135 и отправить по USART *****/
void measure_and_send_result_MQ_135(uint16_t number_of_measurements);

#endif 			// LibADC_H
