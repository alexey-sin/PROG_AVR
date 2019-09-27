///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 												Author:  Skyer                       						 //
// 											Website: www.smartep.ru               							 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <util/delay.h>
	#include "ds18b20.h"
//=============================================================================================================
	#define owi_low()							DS18B20_DDR |= (1<<DS18B20_DS)
	#define owi_rel()							DS18B20_DDR &= ~(1<<DS18B20_DS)
	#define owi_state()							(DS18B20_PIN & (1<<DS18B20_DS))
//=============================================================================================================
void owi_write(uint8_t data)
{
	uint8_t n;

	cli();
	for(n = 0; n < 8; ++n)
	{
		if(data & 1)
		{
			owi_low();
			_delay_us(6);
			owi_rel();
			_delay_us(64);

		}
		else
		{
			owi_low();
			_delay_us(60);
			owi_rel();
			_delay_us(10);
		}
		
		data >>= 1;
	}
	sei();
}
//=============================================================================================================
uint8_t owi_read()
{
	uint8_t data = 0, n;

	cli();
	for(n = 0; n < 8; ++n)
	{
		data >>= 1;
		owi_low();
		_delay_us(6);
		owi_rel();
		_delay_us(9);
		if(owi_state())
			data |= 0x80;
		_delay_us(55);
	}
	sei();
	return data;
}
//=============================================================================================================
uint8_t owi_reset()
{
	uint8_t success = 0;

	cli();
	owi_low();
	_delay_us(480);
	owi_rel();
	_delay_us(70);
	if(!owi_state())
		success = 1;
	_delay_us(410);
	sei();
	return success;
}
//=============================================================================================================
uint8_t owi_crc(uint8_t *data, uint8_t len)
{
	uint8_t crc = 0, n, c;

	while(len--)
	{
		c = *(data++);
		for(n = 8; n; --n)
		{
			if( (crc ^ c) & 1 )
			{
				crc ^= 0x18;
				crc >>= 1;
				crc |= 0x80;
			}
			else
			{
				crc >>= 1;
			}
			c >>= 1;
		}
	}
	return crc;
}
//=============================================================================================================
uint8_t DS18B20_Start()
{
	uint8_t success = 0;

	if(owi_reset())
	{
		owi_write(DS18B20_SKIP_ROM);
		owi_write(DS18B20_CONVERT_T);
		success = 1;
	}
	return success;
}
//=============================================================================================================
int DS18B20_Read()
{
	int result = DS18B20_ERROR;
	uint8_t pad[8], crc, n;

	if(owi_reset())
	{
		owi_write(DS18B20_SKIP_ROM);
		owi_write(DS18B20_R_SCRATCHPAD);
		for(n = 0; n < 8; ++n)
			pad[n] = owi_read();
		crc = owi_read();

		if(crc == owi_crc(pad, sizeof(pad)))
			result = ((int)pad[0] | ((int)pad[1] << 8));
	}

	return result;
}
