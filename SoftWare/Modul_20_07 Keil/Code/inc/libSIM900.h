#ifndef _LibSIM900_H
#define _LibSIM900_H 

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "libUART.h"

#define SMS_OK												1
#define SMS_ERROR											0

#define size_text_sms									50																		// max size text sms


/********************************* send SMS with any text ********************************/
uint8_t send_sms (char* num_phone, char* send_text_sms);

/******************************* answering an incoming call ******************************/
uint8_t incomCall (void);

#endif 			// LibSIM900_H
