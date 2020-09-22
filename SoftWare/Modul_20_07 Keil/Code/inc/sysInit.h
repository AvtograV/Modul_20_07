#include "stm32f10x.h"
#include "FreeRTOS.h"
 #include "task.h"

#ifndef __SYSINIT_H
#define __SYSINIT_H

/************************************************ Use functions ***************************************************************/
void RCC_Init (void);
/*******************************************************************************************************/
void GPIO_Init (void);

#endif /* __SYSINIT_H */
