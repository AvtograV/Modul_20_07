#include "main.h"




int main(void){	
	
	RCC_Init();
	
	Init_USART1_for_1_Wire();
	Init_USART2_for_HC_05();
	Init_DMA1_USART1_TX();
	Init_DMA1_USART1_RX();
	
	
	xTaskCreate (vTaskMeasureTemp, "измерение температуры DS18B20 ( 1-Wire)", 32, NULL, 1, NULL);
	xTaskCreate (vTaskUSART2_IRQHandlerAndroid, "обработка прерываний (запросов) от ОС Android по USART2", 32, NULL, 1, NULL);	
	xTaskCreate (vTaskGetSerialNumberKeyI_Button, "i-Button - получение серийного номера ключа", 32, NULL, 1, NULL);
	
	vTaskStartScheduler();
	
	while(1) {}
}
