// 				Author:  Skyer                            			   //
// 				Website: www.smartep.ru                  			   //
/////////////////////////////////////////////////////////////////////////
	#include <avr/interrupt.h>
	#include <avr/io.h>
	#include <util/delay.h>
	#include <avr/pgmspace.h>
	#include <string.h>
//=============================================================================================================
	#include "lib/ds18b20.h"
//=============================================================================================================
	#define	SEG_PORT 							PORTC
	#define	SEG_DDR  							DDRC
	#define	DIG_PORT 							PORTB
	#define	DIG_DDR  							DDRB
	#define	Digit1  							PB0
	#define	Digit2  							PB1
	#define	Digit3  							PB2
	#define	Digit4  							PB3
	#define DISPLAY_DELAY						4
	#define SYMBOL_POINT						0b10000000
	#define SYMBOL_MINUS						0b01000000
//=============================================================================================================
	uint16_t 	ms = 0;
	uint8_t 	sec = 0, min = 0, hour = 0;
	uint32_t 	second_count = 0;
	uint8_t 	refresh = 0;
	uint8_t 	dig[4] = {0, 0, 0, 0};
	int16_t 	DS18B20_Temperature = DS18B20_ERROR;
	uint8_t 	DS18B20_Is_Converted = 0;
	uint32_t 	DS18B20_Update_Time = 0;
//=============================================================================================================
	// Массив для декодирования цифры в код числа для 7-сегментного индикатора
	uint8_t digits[10] = {
		//hgfedcba
		0b00111111,
		0b00000110,
		0b01011011,
		0b01001111,
		0b01100110,
		0b01101101,
		0b01111101,
		0b00000111,
		0b01111111,
		0b01101111,
	};
//=============================================================================================================
ISR(TIMER2_COMP_vect)
{
	uint8_t sreg_save = SREG;
	ms++;
	if(ms == 1000)
	{
		second_count++;
		ms = 0;
		sec++;
		if (sec == 60)
		{
			sec = 0;
			min++;
			if (min == 60)
			{
				min = 0;
				hour++;
				if (hour == 24)
					hour = 0;
			}
		}
	}
	SREG = sreg_save;
}
//=============================================================================================================
void DS18B20_Update()
{
	if(DS18B20_Is_Converted)
		DS18B20_Temperature = DS18B20_Read();
	else
		DS18B20_Temperature = DS18B20_ERROR;
	DS18B20_Is_Converted = DS18B20_Start();
	DS18B20_Update_Time = second_count + DS18B20_REFRESH_TIME;
	refresh = 1;
}
//=============================================================================================================
int main (void)
{
	// Запрещаем прерывания
	cli();
	// Настраиваем порты ввода-вывода
	SEG_DDR = 0xFF;
	SEG_PORT = 0x00;
	DIG_DDR = (1 << Digit1)|(1 << Digit2)|(1 << Digit3)|(1 << Digit4);
	DIG_PORT &= ~((1 << Digit1)|(1 << Digit2)|(1 << Digit3)|(1 << Digit4));
	// Настраиваем таймеры
	// Разрешение прерывания таймера 2
	TIMSK = (1 << OCIE2);
	// Настраиваем таймер 2 на подсчет секунд
	// Предделитель на 64
	TCCR2 = (1 << WGM21)|(0 << WGM20)|(1 << CS22)|(0 << CS21)|(0 << CS20);
	OCR2 = F_CPU / 64 / 1000;
	// Обнуляем счетчик таймера 2
	TCNT2 = 0;
	// Запуск первого измерения температуры
	DS18B20_Is_Converted = DS18B20_Start();
	_delay_ms(1000);
	DS18B20_Update();
	// Разрешаем прерывания
	sei();
	// Основная программа
	while (1)
	{
		if(second_count >= DS18B20_Update_Time)
			DS18B20_Update();
		// Преобразование температуры в отдельные цифры для вывода
		if (refresh)
		{
			if (DS18B20_Temperature != DS18B20_ERROR)
			{
				uint8_t tls = (DS18B20_Temperature & 0xFF);
				uint8_t tms = (DS18B20_Temperature >> 8);
				if (tms & 0xF8)
				{
					tls = (~tls)+1;
					tms = ~tms;
					dig[0] = SYMBOL_MINUS;
				} else
					dig[0] = 0;
				uint8_t temp = (tms  <<  4) + (tls >> 4);
				uint8_t frac = tls & 0x0F;
				frac = (frac << 1) + (frac << 3); 		// * 10
				frac = frac >> 4; 						// *0.0625 or /16
				dig[3] = digits[frac];
				dig[2] = digits[temp % 10] + SYMBOL_POINT;
				dig[1] = digits[temp % 100 / 10];
			}
			else
				memset(dig, SYMBOL_MINUS, sizeof(dig));
			refresh = 0;
		}
		DIG_PORT = (1 << Digit1);
		SEG_PORT = dig[0];
		_delay_ms(DISPLAY_DELAY);
		DIG_PORT = (1 << Digit2);
		SEG_PORT = dig[1];
		_delay_ms(DISPLAY_DELAY);
		DIG_PORT = (1 << Digit3);
		SEG_PORT = dig[2];
		_delay_ms(DISPLAY_DELAY);
		DIG_PORT = (1 << Digit4);
		SEG_PORT = dig[3];
		_delay_ms(DISPLAY_DELAY);
	}
}
