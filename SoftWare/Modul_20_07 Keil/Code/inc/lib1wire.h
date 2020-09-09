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

#define OW_NO_READ    0xff

#define OW_READ_SLOT	0xff

#define OW_0					0x00
#define OW_1					0xff
#define OW_R_1				0xff
#define Key_iButton_1 0x0000010628D6




/************************* приём - передача по 1-wire *************************/
uint8_t OW_Send (uint8_t sendReset, char *command, uint8_t cLen, uint8_t *data, uint8_t dLen, uint8_t readStart);

/*********** измерить температуру и отправить в приложение Android ************/
void measure_temperature (void);

/********************* измерение температуры по запросу ***********************/
void temp_measure_request (void);

/********************* система контроля доступа IBUTTON ***********************/
void i_Button(void);

/******************************************************************************/
char *utoa_cycle_sub(uint16_t value, char *buffer);



#endif 								// LIB1WIRE_H
