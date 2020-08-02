#ifndef _ONE_WIRE_H
#define _ONE_WIRE_H


#include "dma.h"
#include "usart_1.h"


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

uint8_t OW_Send (uint8_t sendReset, char *command, uint8_t cLen, uint8_t *data, uint8_t dLen, uint8_t readStart);

void measure_temperature (void);

void temp_measur_request (void);

char *utoa_cycle_sub(uint16_t value, char *buffer);

#endif 								// ONE_WIRE_H
