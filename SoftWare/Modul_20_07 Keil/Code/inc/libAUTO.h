#ifndef _LIBAUTO_H
#define _LIBAUTO_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lib1Wire.h"
#include "libUART.h"


#define half_second											500
#define one_second											1000
#define _10_minutes											600000


/*******************************************************************************************/
void autoStaticTemp (char* ROM, uint8_t temp);
/*******************************************************************************************/
void if_smoke_in_room (uint16_t current_smoke);
/********************************************************************************************/
void if_coolant_temp_low (uint8_t current_temp);

/************* функция преобразование числового значения в символьное (2 знака) *************/
char* utoa_cycle_sub_2(uint16_t value, char *buffer);

/************* функция преобразование числового значения в символьное (4 знака) *************/
char *utoa_cycle_sub_4(uint16_t value, char *buffer);

#endif 								// LIBAUTO_H
