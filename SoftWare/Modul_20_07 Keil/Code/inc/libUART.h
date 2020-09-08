#ifndef LIBUART_H
#define LIBUART_H

#include "stm32f10x.h"

#define FCKL_APB2 72000000								// for USART1
#define FCKL_APB1 36000000								// for USART2

#define BAUDRATE_USART1 9600
#define BAUDRATE_USART2 38400

/***************************** USART1 (PA9 - TX, PA10 - RX) *****************************/
void Init_USART1_for_1_Wire(void);
/******************************* отправить байт по USART1 *******************************/
void USART1_Send_Char(char chr);
/****************************** отправить строку по USART1 ******************************/
void USART1_Send_String(char *str);
/*************************** изменить скорость USART1 (1-WIRE) **************************/
void change_speed_USART1(uint32_t set_speed);


/****************************** USART2 (PA2 - TX, PA3 - RX) *****************************/
void Init_USART2_for_HC_05(void);
/******************************* отправить байт по USART2 *******************************/
void USART2_Send_Char(char chr);
/****************************** отправить строку по USART2 ******************************/
void USART2_Send_String(char *str);

#endif // LIBUART_H
