#include "main.h"


int main(void){
	
	RCC_Init();	
	InitEXTI_9();
//	GPIO_Init();
	
	Init_USART1_DS18B20();
	Init_USART2_HC_05();
	Init_USART3_iButton();
	
	Init_DMA1_USART1_TX();																	// DS18B20
	Init_DMA1_USART1_RX();
	Init_DMA1_USART3_TX();																	// iButton
	Init_DMA1_USART3_RX();
	

	xTaskCreate (vTaskUSART2_IRQHandlerAndroid, "обработка прерываний (запросов) от ОС Android по USART2", 32, NULL, 1, NULL);
//	xTaskCreate (vTaskEXTI9_5_IRQHandler, "обработка EXTI (внешнего прерывания) от iButton", 32, NULL, 1, NULL);
	xTaskCreate(vTaskPeriodicFunctionCall, "вызов функций через определённый промежуток времени", 32, NULL, 1, NULL);
	
	vTaskStartScheduler();
	
  while (1) { }
}
