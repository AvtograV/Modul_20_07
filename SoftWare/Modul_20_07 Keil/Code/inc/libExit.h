#ifndef _LibEXTI_H
#define _LibEXTI_H 

#include "stm32f10x.h"
#include "libUART.h"
#include "lib1Wire.h"
 
 
 /****************************** PB10 for iButoon *************************************/
	void InitEXTI_PB10 (void);

 /**************** обработчик внешнего прерывания от линии EXTI_PB10 *****************/																
	void EXTI15_10_IRQHandler(void);

#endif 			// LibEXTI_H
