#include "main.h"


int main(void){
	
	RCC_Init();
	InitEXTI_PB10();
	GPIO_Init();
	
	Init_ADC1_MQ135();
	
	Init_USART1_DS18B20();
	Init_USART2_HC05_and_SIM900();
	Init_USART3_iButton();
	
	Init_DMA1_USART1_TX();																	// DS18B20
	Init_DMA1_USART1_RX();
	Init_DMA1_USART3_TX();																	// iButton
	Init_DMA1_USART3_RX();
	

	xTaskCreate (vTaskFunCallWithoutDelay, "calling various functions without time delays", 32, NULL, 1, NULL);
	xTaskCreate(vTaskByTimeFunctionCall, "вызов функций через определённый промежуток времени", 32, NULL, 1, NULL);
	xTaskCreate (vTaskEXTI10_IRQHandler, "обработка EXTI (внешнего прерывания) от iButton", 32, NULL, 1, NULL);
	
	vTaskStartScheduler();
	
  while (1) { }
}
