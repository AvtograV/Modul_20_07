#include "libADC.h"


uint16_t MQ135_res = 0;
char MQ135_buffer[] = {0};
uint32_t sum_of_measurements_MQ;


/****************************************** PA5 *********************************************/
void Init_ADC1_MQ135 (void) {
	
  RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN ; 										// тактирования порта A
	RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN; 										// тактирование модуля ADC1

	GPIOA -> CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5); 				// PA5 - аналоговый вход
	GPIOA -> CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7); 				// PA6 - аналоговый вход
	
	RCC -> CFGR |= RCC_CFGR_ADCPRE_DIV6;											// PCLK2 divided by 6
	
	ADC1 -> CR2 |= ADC_CR2_CAL; 															// запуск калибровки	
	while (!(ADC1 -> CR2 & ADC_CR2_CAL)); 										// ожидание окончания калибровки
	
	ADC1 -> CR2 |= ADC_CR2_ADON; 															// разрешить АЦП	
	ADC1 -> CR2 &= ~ADC_CR2_CONT;															// режим одиночного преобразования	
	ADC1 -> CR2 |= ADC_CR2_EXTSEL; 														// определить внешнеее событие для запуска преобразования - SWSTART	
	ADC1 -> CR2 |= ADC_CR2_EXTTRIG; 													// разрешение преобразования по внешнему событию 	
	ADC1 -> SMPR2 |= ADC_SMPR2_SMP1; 													// время выборки 239.5 cycles
	
	ADC1 -> SQR1 |= ADC_SQR1_L_0;															// количество каналов для преобразования
																														// в регулярной группе ADC1 (0001) - два канала
	
	ADC1 -> SQR3 |= ADC_SQR3_SQ1_0;														// (0101) первое преобразование - 5 канал (PA5) - регулярная группа
	ADC1 -> SQR3 |= ADC_SQR3_SQ1_2;
	
	ADC1 -> SQR3 |= ADC_SQR3_SQ2_0;														// (0111) второе преобразование - 7 канал (PA7) - регулярная группа
	ADC1 -> SQR3 |= ADC_SQR3_SQ2_1;
	ADC1 -> SQR3 |= ADC_SQR3_SQ2_2;
}

/*********************************** измерить CO2 (MQ-135) **********************************/	
uint16_t MQ135_measure_request(void) {
		
	ADC1 -> CR2 |= ADC_CR2_SWSTART;								// запуск преобразований
		while (!(ADC1 -> SR & ADC_SR_EOC));					// ждем пока преобразование завершится
		
	return ADC1 -> DR;
}

/** функция нахождения среднего арифметического числа от всего количества измерений MQ-135 **/
uint16_t arithmetic_mean_number(uint8_t number_of_measur) {
	
	sum_of_measurements_MQ = 0;
	
	if (number_of_measur != 0) {
		for (uint8_t i = 0; i < number_of_measur; i++) {
			
			sum_of_measurements_MQ += MQ135_measure_request();
			
			vTaskDelay(10);
		}
	}	
	return sum_of_measurements_MQ /= number_of_measur;
}


/************** измерить некое кол-во раз (number_of_measurements_MQ) MQ-135 ***************/
uint16_t measure_MQ_135 (uint16_t number_of_measurements) {
	
	MQ135_res = 0;	
	MQ135_res = arithmetic_mean_number(number_of_measurements);
	
	return MQ135_res;
}
