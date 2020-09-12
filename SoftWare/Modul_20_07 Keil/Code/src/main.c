#include "main.h"


int main(void){	
	
	RCC_Init();	
	InitEXTI_9();
	GPIO_Init();
	
//	Init_USART1_for_1_Wire();
//	Init_USART2_for_HC_05();
	
//	Init_DMA1_USART1_TX();
//	Init_DMA1_USART1_RX();
	
	EXTI9_5_IRQHandler();
	
//	xTaskCreate (vTaskMeasureTemp, "измерение температуры DS18B20 ( 1-Wire)", 32, NULL, 1, NULL);
//	xTaskCreate (vTaskUSART2_IRQHandlerAndroid, "обработка прерываний (запросов) от ОС Android по USART2", 32, NULL, 1, NULL);
//	xTaskCreate (vTaskEXTI9_5_IRQHandler, "обработка EXTI (внешнего прерывания) от iButton", 32, NULL, 1, NULL);
	
//	vTaskStartScheduler();
	
  while (1) { }
}
