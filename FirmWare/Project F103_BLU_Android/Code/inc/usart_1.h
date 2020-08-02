#ifndef _USART_1_H
#define _USART_1_H

#include "stm32f10x.h"

#define FCKL_APB2													72000000
#define BAUDRATE_USART1										9600

/**************************************** PA9 - TX, PA10 - RX ****************************************/
void USART1_Init_for_ONE_WIRE (void);
/************************************* временная функция задержки *************************************/
void delay (uint32_t milles);
/************************************** отправить байт по USART1 **************************************/
void USART1_Send_Char (char chr);
/************************************* отправить строку по USART1 *************************************/
void USART1_Send_String (char* str);
/********************************** изменить скорость USART1 (1-WIRE) *********************************/
void change_speed_USART1 (uint32_t set_speed);

#endif 			// USART_1_H
