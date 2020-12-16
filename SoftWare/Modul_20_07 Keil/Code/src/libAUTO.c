#include "libAUTO.h"

const uint8_t delta_temp = 2;

extern uint8_t FLAG_AUTO;

int SetTempToWinterHouse = 0;
uint8_t FLAG_DELAY_FORSED_VENT = 0;

void autoStaticTemp (char* ROM, uint8_t temp) {
	if (FLAG_AUTO == 1) {
		if (temp_measure_request(ROM) >= (temp + delta_temp) &&
			FLAG_DELAY_FORSED_VENT == 0) {
				
			GPIOB -> BSRR |= GPIO_BSRR_BS12;
			FLAG_DELAY_FORSED_VENT = 1;
		}
		else if (temp_measure_request(ROM) <= (temp - delta_temp) &&
			FLAG_DELAY_FORSED_VENT == 1) {
				GPIOB -> BSRR |= GPIO_BSRR_BR12;
				FLAG_DELAY_FORSED_VENT = 0;
		}
	}
}
