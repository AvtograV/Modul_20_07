#ifndef _LibSIM900_H
#define _LibSIM900_H 

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "libUART.h"
#include "libAUTO.h"
#include "libADC.h"


#define SMS_OK															1
#define SMS_ERROR														0

#define size_text_sms												50												// max size text sms

#define SMOKE_ALERT_RESEIVED								1
#define NO_VALUE														0

/******************************* commands from sim900 (USART3) ******************************/
void execute_commands_from_sim900(char* str);



// AT+CMGL="ALL"				- get a list of all messager
// AT+CMGD=2,1 					- delete all read massager
// AT+CMGD=2,4 					- delete all messager
// AT+CMGR=4						- read message number 4

// <CR><LF>+CMGL: 7,"REC UNREAD","+79676211100","","21/01/26,16:14:50+40"<CR><LF>SMOKE<CR><LF>

#endif 			// LibSIM900_H
