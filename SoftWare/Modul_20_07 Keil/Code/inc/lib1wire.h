#ifndef _LIB1WIRE_H
#define _LIB1WIRE_H


#include "libDMA.h"
#include "libUART.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#define OW_SEND_RESET		1
#define OW_NO_RESET			2

#define OW_OK    				1
#define OW_ERROR    		2
#define OW_NO_DEVICE		3

#define OW_NO_READ    	0xff

#define OW_READ_SLOT		0xff

#define OW_0						0x00
#define OW_1						0xff
#define OW_R_1					0xff

#define Key_iButton_1 	0x0000010628D6
#define Key_iButton_2 	0x00000105BA4E

#define usart1_DS18B20	1
#define usart3_iButton	3




/*********** измерить температуру и отправить в приложение Android (в т. ч. по запросу)************/
void temp_measure_request(char *ROM_DS18B20);

/************************************** прочитать ROM DS18B20 *************************************/
void read_ROM_DS18B20(void);

/******************************* система контроля доступа IBUTTON *********************************/
void i_Button(void);


#endif 								// LIB1WIRE_H
