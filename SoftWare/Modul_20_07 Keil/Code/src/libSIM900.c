#include "libSIM900.h"

char buffer_messeger[5];

extern char t_buffer_char[5];															// массив для символьного значения температуры
extern char str_MQ_135[5];																// массив для символьных значений с MQ	

extern char ROM_1[];
extern char ROM_7[];

extern const uint8_t number_of_measurements_MQ;						// кол-во измерений (выборка) для MQ

extern uint16_t threshold_smoke;
extern uint16_t critical_value_smoke;

uint8_t FLAG_SIM900_ALERT = NO_VALUE;

/******************************* commands from sim900 (USART3) ******************************/
void execute_commands_from_sim900(char* str) {
	
	if (contains (str, "LIGHT ON")) {
			GPIOB -> BSRR |= GPIO_BSRR_BS12;
		// delete read messages
			USART3_Send_String("AT+CMGDA=\"DEL READ\"\r\n");
	}	
	else if (contains (str, "LIGHT OFF")) {
			GPIOB -> BSRR |= GPIO_BSRR_BR12;
			USART3_Send_String("AT+CMGDA=\"DEL READ\"\r\n");
	}
	
	// call-request of some current parameters
	else if (contains (str, "RING")) {
		vTaskDelay(1000);
		
		USART3_Send_String("ATH0\r\n");																// phone is busy
		vTaskDelay(1000);
		
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");
		
		vTaskDelay(500);
		
		USART3_Send_String(utoa_cycle_sub_4
										(arithmetic_mean_number(number_of_measurements_MQ), str_MQ_135));
		USART3_Send_String("\r\n");
		
		// USART3_Send_String( utoa_cycle_sub_2(temp_measure_request(ROM_1),	t_buffer_char));
		// USART3_Send_String("\r\n");
		// USART3_Send_String( utoa_cycle_sub_2(temp_measure_request(ROM_7), t_buffer_char));
		// USART3_Send_String("\r\n");		
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// 0x20 - "space" charecter, x3E - '<'
	else if (contains (str, "\x3E\x20\r\n")) { }	
	// message reseived = read message
	else if (contains (str, "+CMTI: \"SM\",")) {
			USART3_Send_String("AT+CMGL=\"REC UNREAD\"\r\n");
	}
	
	
	// if the user has confirmed receipt of the notification
	else if (contains (str, "GET SMOKE")) {		
		FLAG_SIM900_ALERT = SMOKE_ALERT_RESEIVED;		
	}
	
	// set the status "guard"
	else if (contains (str, "ARMED STATUS")) {
		
		FLAG_SIM900_ALERT = NO_VALUE;
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		USART3_Send_String("ARMED STATUS\r\n");		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// request for current values 
	else if (contains (str, "GET SMOKE LEVELS")) {
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);
		
		USART3_Send_String("Threshold smoke ");
		USART3_Send_String(utoa_cycle_sub_4(threshold_smoke, buffer_messeger));
		USART3_Send_String("\r\n");
		USART3_Send_String("Critical value smoke ");
		USART3_Send_String(utoa_cycle_sub_4(critical_value_smoke, buffer_messeger));
		USART3_Send_String("\r\n");
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// increase the variable "threshold_smoke" by 25
	else if (contains (str, "THRESHOLD +25")) {
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		
		USART3_Send_String("Current value \"threshold_smoke\" ");
		USART3_Send_String(utoa_cycle_sub_4(threshold_smoke += 25, buffer_messeger));
		USART3_Send_String("\r\n");
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// increase the variable "threshold_smoke" by 50
	else if (contains (str, "THRESHOLD +50")) {
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		
		USART3_Send_String("Current value \"threshold_smoke\" ");
		USART3_Send_String(utoa_cycle_sub_4(threshold_smoke += 50, buffer_messeger));
		USART3_Send_String("\r\n");
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// decrease the variable "threshold_smoke" by 25
	else if (contains (str, "THRESHOLD -25")) {
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		
		USART3_Send_String("Current value \"threshold_smoke\" ");
		USART3_Send_String(utoa_cycle_sub_4(threshold_smoke -= 25, buffer_messeger));
		USART3_Send_String("\r\n");
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// decrease the variable "threshold_smoke" by 50
	else if (contains (str, "THRESHOLD -50")) {
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		
		USART3_Send_String("Current value \"threshold_smoke\" ");
		USART3_Send_String(utoa_cycle_sub_4(threshold_smoke -= 50, buffer_messeger));
		USART3_Send_String("\r\n");
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// increase the variable "critical_value_smoke" by 25
	else if (contains (str, "CRITICAL SMOKE +25")) {
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		
		USART3_Send_String("Current value \"critical_value_smoke\" ");
		USART3_Send_String(utoa_cycle_sub_4(critical_value_smoke += 25, buffer_messeger));
		USART3_Send_String("\r\n");
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// increase the variable "critical_value_smoke" by 50
	else if (contains (str, "CRITICAL SMOKE +50")) {
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		
		USART3_Send_String("Current value \"critical_value_smoke\" ");
		USART3_Send_String(utoa_cycle_sub_4(critical_value_smoke += 50, buffer_messeger));
		USART3_Send_String("\r\n");
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// decrease the variable "critical_value_smoke" by 25
	else if (contains (str, "CRITICAL SMOKE -25")) {
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		
		USART3_Send_String("Current value \"critical_value_smoke\" ");
		USART3_Send_String(utoa_cycle_sub_4(critical_value_smoke -= 25, buffer_messeger));
		USART3_Send_String("\r\n");
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
	
	// decrease the variable "critical_value_smoke" by 50
	else if (contains (str, "CRITICAL SMOKE -50")) {
		
		vTaskDelay(1000);
		USART3_Send_String("AT+CMGS=\"+79676211100\"\r\n");		
		vTaskDelay(500);		
		
		USART3_Send_String("Current value \"critical_value_smoke\" ");
		USART3_Send_String(utoa_cycle_sub_4(critical_value_smoke -= 50, buffer_messeger));
		USART3_Send_String("\r\n");
		
		vTaskDelay(1);
		USART3_Send_Char(0x1a);																				// sub
	}
}
