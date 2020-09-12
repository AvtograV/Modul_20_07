#ifndef _LibEXTI_H
#define _LibEXTI_H 

#include "stm32f10x.h"
#include "libUART.h"
#include "lib1Wire.h"
 
 
 /*************************************************************************************************/
 void InitEXTI_9(void);

 /************************ обработчик внешнего прерывания от линии EXTI9 **************************/																
 void EXTI9_5_IRQHandler(void);

#endif 			// LibEXTI_H
