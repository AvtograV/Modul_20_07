#ifndef _LIBAUTO_H
#define _LIBAUTO_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lib1Wire.h"


void autoStaticTemp (char* ROM, uint8_t temp);


#endif 								// LIBAUTO_H
