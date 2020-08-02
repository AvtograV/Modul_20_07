#ifndef _USART_2_H
#define _USART_2_H

#include "stm32f10x.h"

#define FCKL_APB1												36000000
#define BAUDRATE_USART2									38400


void USART2_Init_for_HC_05 (void);
/************************************** отправить байт по USART2 **************************************/
void USART2_Send_Char (char chr);
/************************************* отправить строку по USART2 *************************************/
void USART2_Send_String (char* str);


#endif 			// USART_2_H
