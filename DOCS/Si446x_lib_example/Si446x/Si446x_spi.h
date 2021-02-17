/*
 * Project: Si4463 Radio Library for AVR and Arduino
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2017 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/si4463-radio-library-avr-arduino/
 */

#ifndef SI446X_SPI_H_
#define SI446X_SPI_H_

#include <avr/io.h>

void spi_init(void);

inline void spi_transfer_nr(uint8_t data)
{
	SPDR = data;
	loop_until_bit_is_set(SPSR, SPIF);
}

inline uint8_t spi_transfer(uint8_t data)
{
	SPDR = data;
	loop_until_bit_is_set(SPSR, SPIF);
	return SPDR;
}

#endif /* SI446X_SPI_H_ */
