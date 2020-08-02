#ifndef _HC_05_H
#define _HC_05_H

#include "usart_1.h"
#include "stm32f10x.h"


/********************** принять байт от HC-05 по USART1 **********************/
void USART2_IRQHandler(void);

/******************* отправить что-то по USART1 для HC-05 *******************/
void USART2_Send_Command_HC_05 (void);

#endif 			// HC_05_H
