#include "spi.h"

void SPI_init(void)
{
  ST_DDR = (1<<SCK_pin) | (1<<MOSI_pin) | (1<<CS_pin) | (1<<DC_pin);
  sbi(ST_PORT, CS_pin);  
  #ifdef RESET_HW
    ST_DDR |= 1 << RESET_HW_pin;
    sbi(ST_PORT, RESET_HW_pin);
  #endif
}

uint8_t SPI_byte(uint8_t byte)
{
  for(uint8_t counter = 8; counter; counter--)
    {
      if (byte & MSBit)
        sbi(ST_PORT, MOSI_pin);
      else
        cbi(ST_PORT, MOSI_pin);
      byte <<= 1;
      sbi(ST_PORT, SCK_pin); 
      cbi(ST_PORT, SCK_pin); 
    }
}