#ifndef LIBUART_H
#define LIBUART_H

#include "stm32f10x.h"
#include "lib1wire.h"
#include "libADC.h"
#include "libHc05.h"
#include "libSIM900.h"

#define FCKL_APB2 															72000000					// for USART1
#define FCKL_APB1 															36000000					// for USART2 and USART3

#define BAUDRATE_USART1 												9600
#define BAUDRATE_USART2 												38400
#define BAUDRATE_USART3 												38400

#define size_buffer_RX_USART										80

/************* узнать - содержит ли строка определённую последовательность **************/
uint8_t contains (char* str, char* sequence);


/*********************** USART1 (PA9 (Single Wire (Half-Duplex) *************************/
void Init_USART1_DS18B20(void);
/******************************* отправить байт по USART1 *******************************/
void USART1_Send_Char(char chr);
/****************************** отправить строку по USART1 ******************************/
void USART1_Send_String(char *str);
/************************** изменить скорость USART1 (DS18B20) **************************/
void change_speed_USART1(uint32_t set_speed);


/************************* USART2 (PA2 - TX, PA3 - RX) (HC-05) **************************/
void Init_USART2_HC05(void);
/******************************* отправить байт по USART2 *******************************/
void USART2_Send_Char(char chr);
/****************************** отправить строку по USART2 ******************************/
void USART2_Send_String(char *str);
/*************************** принять байт от HC-05 по USART2 ****************************/
void USART2_IRQHandler(void);
/***************************** получить строку по USART2 ********************************/
void getString_USART2 (void);


/*********************** USART3 (PB10 - TX, PB - RX) (SIM - 900) ************************/
void Init_USART3_SIM900(void);
/******************************* отправить байт по USART3 *******************************/
void USART3_Send_Char(char chr);
/****************************** отправить строку по USART3 ******************************/
void USART3_Send_String(char *str);
/*********************** получить строку по USART3 (SIM - 900) **************************/
void getString_USART3 (void);
/************************* изменить скорость USART3 (for iButton) ***********************/
void change_speed_USART3(uint32_t set_speed);
/*********************** USART3 (PB10 (Single Wire (Half-Duplex) ************************/
void Init_USART3_iButton(void);

#endif // LIBUART_H
