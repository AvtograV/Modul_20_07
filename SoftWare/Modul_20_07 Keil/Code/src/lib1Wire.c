#include "lib1Wire.h"


extern uint8_t ow_t_buf[8]; 			// буфер для передачи по 1-wire
extern uint8_t ow_r_buf[8]; 			// буфер для приёма по 1-wire

uint8_t t_integer_current = 0; 		// переменная для сохранения текущего значения температуры

char t_buffer_char[] = {0};				// массив для символьного значения температуры

const uint16_t pow10Table2_16[] =
	{
		10ul,
		1ul};
	

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

/******************************************************************************/
// функция преобразует один байт в восемь, для передачи через USART
// ow_byte - байт, который надо преобразовать
// ow_bits - ссылка на буфер, размером не менее 8 байт
/******************************************************************************/
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

/******************************************************************************/
// обратное преобразование - из того, что получено через USART опять собирается байт
// ow_bits - ссылка на буфер, размером не менее 8 байт
/******************************************************************************/
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

/******************************************************************************/
// осуществляет сброс и проверку на наличие устройств на шине
/******************************************************************************/
uint8_t OW_Reset()
{

	uint8_t ow_presence;

	change_speed_USART1(9600);

	USART1_Send_Char(0xf0);

	while (!(USART1->SR & USART_SR_TC))
	{
	}
	ow_presence = USART1->DR;

	change_speed_USART1(115200);

	if (ow_presence != 0xf0)
	{
		return OW_OK;
	}
	return OW_NO_DEVICE;
}

/************************* приём - передача по 1-wire *************************/
//	процедура общения с шиной 1-wire
/******************************************************************************/

uint8_t OW_Send(	   // ниже указанны аргументы функции OW_Send ()
	uint8_t sendReset, // sendReset - посылать RESET в начале общения
										 // OW_SEND_RESET или OW_NO_RESET
	char *command,	   // command - массив байт, отсылаемых в шину.
										 // если нужно чтение - отправляем OW_READ_SLOTH
	uint8_t cLen,	   	 // cLen - длина буфера команд, столько байт отошлется в шину
	uint8_t *data,	   // data - если требуется чтение, то ссылка на буфер для чтения
	uint8_t dLen,	   	 // dLen - длина буфера для чтения. Прочитается не более этой длины
										 // можно указать OW_NO_READ, тогда можно не задавать data и dLen
	uint8_t readStart) // readStart - с какого символа передачи начинать чтение, нумеруются с 0
{

	if (sendReset == OW_SEND_RESET)
	{
		if (OW_Reset() == OW_NO_DEVICE)
		{
			return OW_NO_DEVICE;
		}
	}

	while (cLen > 0)
	{
		OW_toBits(*command, ow_t_buf);
		command++;
		cLen--;

		Exchange_DMA1_USART1();

		if (readStart == 0 && dLen > 0)
		{
			*data = OW_toByte(ow_r_buf);
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

/************ измерить температуру и отправить в приложение Android ************/
void measure_temperature(void)
{

	int16_t Tx16 = 0; // результат измерения - двухбайтовое целое со знаком,
										// содержащее температуру в градусах, умноженную на 16

	OW_Send(OW_SEND_RESET, "\xcc\x44", 2, 0, 0, OW_NO_READ); // SKIP ROM, CONVERT T
	
	vTaskDelay(1000);

	OW_Send(OW_SEND_RESET, "\xcc\xbe\xff\xff", 4, (uint8_t *)&Tx16, 2, 2); // SKIP ROM, READ SCRATCHPAD
	
	// разделить полученное значение на 16
	uint8_t t_integer_new = Tx16 >> 4;
	
	// проверить - если значение температуры изменилось - отправить новое значение
	// если не изменилось - не отправлять
	if (t_integer_current != t_integer_new)
	{
		// поменять текущее значение на новое
		t_integer_current = t_integer_new;
		// преобразовать из цифровых в символьные значения
		utoa_cycle_sub(t_integer_new, t_buffer_char);

		USART2_Send_String("temp ");
		USART2_Send_String(t_buffer_char);

		USART2_Send_Char(0xD); // возврат каретки (carriage return, CR) — 0x0D, '\r'
		USART2_Send_Char(0xA); // перевод на строку вниз(line feed, LF) — 0x0A, '\n'
	}
}

/********************* измерение температуры по запросу ***********************/
void temp_measure_request(void)
{

	int16_t Tx16 = 0; // результат измерения - двухбайтовое целое со знаком,
										// содержащее температуру в градусах, умноженную на 16

	OW_Send(OW_SEND_RESET, "\xcc\x44", 2, 0, 0, OW_NO_READ); // SKIP ROM, CONVERT T
	
	vTaskDelay(1000);
	
	OW_Send(OW_SEND_RESET, "\xcc\xbe\xff\xff", 4, (uint8_t *)&Tx16, 2, 2); // SKIP ROM, READ SCRATCHPAD

	uint8_t t_integer_new = Tx16 >> 4;

	utoa_cycle_sub(t_integer_new, t_buffer_char);

	USART2_Send_String("Temp ");
	USART2_Send_String(t_buffer_char);

	USART2_Send_Char(0xD); // возврат каретки (carriage return, CR) — 0x0D, '\r'
	USART2_Send_Char(0xA); // перевод на строку вниз(line feed, LF) — 0x0A, '\n'
}

/********************* система контроля доступа IBUTTON ***********************/
void i_Button(void)	{

	int8_t i_button_serial_num = 0; 	// полученный серийный номер ключа i-button
	
	// read ROM [33h] или [0Fh]
	// math ROM [55h]
	// skip ROM [CCh]
	// searсh ROM [F0h]

	OW_Send(OW_SEND_RESET, "\33", 1, (uint8_t *)&i_button_serial_num, 2, 2);
	
	vTaskDelay(10000);

	utoa_cycle_sub(i_button_serial_num, t_buffer_char);

	USART2_Send_String("Серийный номер iButton ");
	USART2_Send_String(t_buffer_char);

	USART2_Send_Char(0xD); // возврат каретки (carriage return, CR) — 0x0D, '\r'
	USART2_Send_Char(0xA); // перевод на строку вниз(line feed, LF) — 0x0A, '\n'
}
