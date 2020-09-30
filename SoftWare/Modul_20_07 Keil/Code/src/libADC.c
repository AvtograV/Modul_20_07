#include "libADC.h"


uint16_t MQ135_res = 0;
char MQ135_buffer[] = {0};


const uint16_t  pow10Table16[]=
{
    10000ul,
    1000ul,
    100ul,
    10ul,
    1ul
};

static char *utoa_cycle_sub(uint16_t value, char *buffer) {
   if (value == 0)  {
      buffer[0] = '0';
      buffer[1] = 0;
      return buffer;
   }
	 
   char *ptr = buffer;
   uint8_t i = 0;
	 
   do {
      uint16_t pow10 = pow10Table16[i++];
      uint8_t count = 0;
		 
      while(value >= pow10) {
         count ++;
         value -= pow10;
      }			
     *ptr++ = count + '0';
   }
	 
		while(i < 5);
			*ptr = 0;	 
																					// удаляем ведущие нули
   while(buffer[0] == '0') ++buffer;
   return buffer;
}


/****************************** PA1 **********************************/
void Init_ADC1_MQ135 (void) {
	
  RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN ; 										// тактирования порта A
	RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN; 										// тактирование модуля ADC1

	GPIOA -> CRL &= ~ (GPIO_CRL_MODE1 | GPIO_CRL_CNF1); 			// PA1 - аналоговый вход
	
	RCC -> CFGR |= RCC_CFGR_ADCPRE_DIV6;											// PCLK2 divided by 6
	
	ADC1 -> CR2 |= ADC_CR2_CAL; 															// запуск калибровки	
	while (!(ADC1 -> CR2 & ADC_CR2_CAL)); 										// ожидание окончания калибровки
	
	ADC1 -> CR2 |= ADC_CR2_ADON; 															// разрешить АЦП
	
	ADC1 -> CR2 &= ~ADC_CR2_CONT;															// режим одиночного преобразования
	
	ADC1 -> CR2 |= ADC_CR2_EXTSEL; 														// определить внешнеее событие для запуска преобразования - SWSTART
	
	ADC1 -> CR2 |= ADC_CR2_EXTTRIG; 													// разрешение преобразования по внешнему событию 
	
	ADC1 -> SMPR2 |= ADC_SMPR2_SMP1; 													// время выборки 239.5 cycles
	
	ADC1 -> SQR1 &= ~ADC_SQR1_L;															// количество каналов для преобразования 
																														// в регулярной группе ADC1 (0000) - один канал
	
	ADC1 -> SQR3 |= ADC_SQR3_SQ1_0;														// (PA1) - номер канала (первый канал) первого
																														//  преобразования регулярной группы ADC1
}

/************ измерить CO2 (MQ-135) и отправить в приложение Android ***********/
	
	void MQ135_measure_request(void) {
		
				ADC1 -> CR2 |= ADC_CR2_SWSTART;								// запуск преобразований
        while (!(ADC1 -> SR & ADC_SR_EOC));						// ждем пока преобразование завершится

				MQ135_res = ADC1 -> DR;
		
				utoa_cycle_sub(MQ135_res, MQ135_buffer);
				
				USART2_Send_String("MQ135 ");
				USART2_Send_String(MQ135_buffer);
				USART2_Send_Char(0xD); 																								// возврат каретки (carriage return, CR) — 0x0D, '\r'
				USART2_Send_Char(0xA); 																								// перевод на строку вниз(line feed, LF) — 0x0A, '\n'
}
