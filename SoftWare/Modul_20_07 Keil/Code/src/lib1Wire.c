#include "lib1Wire.h"

const uint16_t openLockTime = 5000;																				// время открытия замка (соленоид)
const uint16_t pow10Table2_16[] = {10ul, 1ul};

const char ROM_1[] = {0x28, 0xff, 0x07, 0x21, 0x33, 0x17, 0x04, 0x3f};		// 0x28 - family code (DS18B20)			0x3f - CRC
//const char ROM_2[] = {0x28, 0xff, 0xCA, 0x81, 0x33, 0x17, 0x04, 0x58};		// 0xff, 0xCA, 0x81, 0x33, 0x17, 0x04 - ROM CODE (in reverse order)
//const char ROM_3[] = {0x28, 0xff, 0xFB, 0x1F, 0x33, 0x17, 0x04, 0xE5};
//const char ROM_4[] = {0x28, 0xff, 0x9D, 0x8C, 0x32, 0x17, 0x03, 0x29};
//const char ROM_5[] = {0x28, 0xff, 0xE9, 0x8B, 0x32, 0x17, 0x03, 0xDD};
//const char ROM_6[] = {0x28, 0xff, 0xF8, 0x8B, 0x32, 0x17, 0x03, 0x6C};
const char ROM_7[] = {0x28, 0xff, 0xA8, 0xCA, 0x32, 0x17, 0x03, 0x96};
//const char ROM_8[] = {0x28, 0xff, 0x9D, 0xE3, 0x80, 0x17, 0x05, 0x0D};		// sealed sensor

int64_t i_button_serial_num = 0; 																					// полученный серийный номер ключа i-button
int16_t Tx16 = 0; 																												// результат измерения DS18B20 - двухбайтовое целое со знаком,
																																					// содержащее температуру в градусах, умноженную на 16 (получено с DS18B20)																																					
extern uint8_t buf_DS18B20_USART1_DMA1_tx[8];
extern uint8_t buf_DS18B20_USART1_DMA1_rx[8];

extern uint8_t buf_iButton_USART3_DMA1_tx[8];
extern uint8_t buf_iButton_USART3_DMA1_rx[8];

/************* функция преобразование числового значения в символьное (2 знака) *************/
char *utoa_cycle_sub(uint16_t value, char *buffer)
{
	if (value == 0)
	{
		buffer[0] = '0';
		buffer[1] = 0;
		return buffer;
	}

	char *ptr = buffer;
	uint8_t i = 0;

	do
	{
		uint16_t pow10 = pow10Table2_16[i++];
		uint8_t count = 0;

		while (value >= pow10)
		{
			count++;
			value -= pow10;
		}

		*ptr++ = count + '0';

	} while (i < 2);

	*ptr = 0;

	while (buffer[0] == '0') // удаляем ведущие нули
		++buffer;

	return buffer;
}

/********************************************************************************************/
// функция преобразует один байт в восемь, для передачи через USART
// ow_byte - байт, который надо преобразовать
// ow_bits - ссылка на буфер, размером не менее 8 байт
/********************************************************************************************/
void OW_toBits(uint8_t ow_byte, uint8_t *ow_bits)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if (ow_byte & 0x01)
		{					 				 // если текущий бит в байте ==1 то
			*ow_bits = OW_1; // заменяем на число которое при передаче по USART для 1wire будет единицей т. е. 0xFF
		}
		else
		{
			*ow_bits = OW_0; // тоже самое только для 0
		}
		ow_bits++;
		ow_byte = ow_byte >> 1; // сдвигаем обработанный бит
	}
}

/********************************************************************************************/
// обратное преобразование - из того, что получено через USART опять собирается байт
// ow_bits - ссылка на буфер, размером не менее 8 байт
/********************************************************************************************/
uint8_t OW_toByte(uint8_t *ow_bits)
{
	uint8_t ow_byte, i;
	ow_byte = 0;
	for (i = 0; i < 8; i++)
	{
		ow_byte = ow_byte >> 1;
		if (*ow_bits == OW_R_1)
		{					 				 // если пришло  по USART 0xFF то это у нас пришла 1ца
			ow_byte |= 0x80; // устанавливаем в 1 старший бит
		}
		ow_bits++; // передвигаемся к следующему байту который является либо 0 = 0x00 или 1 = 0xFF
	}
	return ow_byte; // возвращаем полученный байт
}

/********************************************************************************************/
// осуществляет сброс и проверку на наличие устройств на шине
/********************************************************************************************/
uint8_t OW_Reset(uint8_t num_usart) {

	uint8_t ow_presence;
	
	if (num_usart == usart1_DS18B20) {								// USART1

		change_speed_USART1(9600);
		USART1_Send_Char(0xf0);
		
		while (!(USART1 -> SR & USART_SR_TC))	{}
		
		ow_presence = USART1 -> DR;
		change_speed_USART1(115200);
	}
	else if (num_usart == 3) {												// USART3

		change_speed_USART3(9600);
		USART3_Send_Char(0xf0);
		
		while (!(USART3->SR & USART_SR_TC))	{}
		
		ow_presence = USART3->DR;
		change_speed_USART3(115200);
	}
	
	if (ow_presence != 0xf0)
	{
		return OW_OK;
	}
	return OW_NO_DEVICE;
}

/******************************** приём - передача по 1-wire ********************************/
//	процедура общения с шиной 1-wire
/********************************************************************************************/

uint8_t OW_Send(	   // ниже указанны аргументы функции OW_Send ()	
	uint8_t sendReset, // sendReset - посылать RESET в начале общения
										 // OW_SEND_RESET или OW_NO_RESET
	uint8_t numUsart,	 // указать какой USART используется
	char *command,	   // command - массив байт, отсылаемых в шину.
										 // если нужно чтение - отправляем OW_READ_SLOTH
	uint8_t cLen,	   	 // cLen - длина буфера команд, столько байт отошлется в шину
	uint8_t *data,	   // data - если требуется чтение, то ссылка на буфер для чтения
	uint8_t dLen,	   	 // dLen - длина буфера для чтения. Прочитается не более этой длины
										 // можно указать OW_NO_READ, тогда можно не задавать data и dLen
	uint8_t readStart) // readStart - с какого символа передачи начинать чтение, нумеруются с 0
{

	if (sendReset == OW_SEND_RESET)	{
		if (OW_Reset(numUsart) == OW_NO_DEVICE)	{
			
			return OW_NO_DEVICE;
		}
	}

	while (cLen > 0) {
		
		if (numUsart == usart1_DS18B20) {
			OW_toBits(*command, buf_DS18B20_USART1_DMA1_tx);
		}
		else if (numUsart == usart3_iButton) {
			OW_toBits(*command, buf_iButton_USART3_DMA1_tx);
		}
		
	command++;
	cLen--;

		if (numUsart == usart1_DS18B20) {
			Exchange_DMA1_USART1();										// обмен DMA1 - USART1 - DMA1
		}
		else if (numUsart == usart3_iButton) {
			Exchange_DMA1_USART3();										// обмен DMA1 - USART3 - DMA1
		}
		

		if (readStart == 0 && dLen > 0)	{
			
			if (numUsart == usart1_DS18B20) {
				*data = OW_toByte(buf_DS18B20_USART1_DMA1_rx);
			}
			else if (numUsart == usart3_iButton) {
				*data = OW_toByte(buf_iButton_USART3_DMA1_rx);
			}
			data++;
			dLen--;
		}
		else
		{
			if (readStart != OW_NO_READ)
			{
				readStart--;
			}
		}
	}
	return OW_OK;
}


/********* temperature measurement with a specific sensor *********/
uint8_t temp_measure_request(char *ROM_DS18B20) {
		
		OW_Send(OW_SEND_RESET, usart1_DS18B20, "\x55", 1, 0, 0, OW_NO_READ);														// MATCH ROM
		OW_Send(OW_NO_RESET, usart1_DS18B20, ROM_DS18B20, 8, 0, 0, OW_NO_READ);	
		OW_Send(OW_NO_RESET, usart1_DS18B20, "\x44", 1, 0, 0, OW_NO_READ); 															// CONVERT T	
	
		vTaskDelay(1000);
	
		OW_Send(OW_SEND_RESET, usart1_DS18B20, "\x55", 1, 0, 0, OW_NO_READ);
		OW_Send(OW_NO_RESET, usart1_DS18B20, ROM_DS18B20, 8, 0, 0, OW_NO_READ);
		OW_Send(OW_NO_RESET, usart1_DS18B20, "\xbe\xff\xff", 3, (uint8_t *)&Tx16, 2, 1);								// READ SCRATCHPAD
	
		// разделить полученное значение на 16
		uint8_t t_integer_new = Tx16 >> 4;
	
	return t_integer_new;
}


/**************************** система контроля доступа IBUTTON ******************************/	
void i_Button(void)	{
	
	OW_Send(OW_SEND_RESET, usart3_iButton, "\x33\xff\xff\xff\xff\xff\xff\xff\xff", 9, (uint8_t *)&i_button_serial_num, 6, 2);
	
	if (i_button_serial_num == Key_iButton_1 || i_button_serial_num == Key_iButton_2) {
	
		GPIOB -> BSRR |= GPIO_BSRR_BS12;									// open the lock (on solenoid coil - 1)
		vTaskDelay(openLockTime);
		GPIOB -> BSRR |= GPIO_BSRR_BR12;									// close the lock (on solenoid coil - 0)
		
		i_button_serial_num = 0;
	 }
}


/*********************************** прочитать ROM DS18B20 **********************************/
void read_ROM_DS18B20(void) {
 OW_Send(OW_SEND_RESET, usart1_DS18B20, "\x33\xff\xff\xff\xff\xff\xff\xff\xff", 9, (uint8_t *)&i_button_serial_num, 8, 0);		
}
