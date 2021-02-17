/* File spi.h */
#ifndef SPI_H
#define SPI_H

#define ST_DDR                DDRB
#define ST_PORT               PORTB
#define ST_PIN                PINB
#define SCK_pin               PB5
#define MOSI_pin              PB3
#define CS_pin                PB2
#define DC_pin                PB1
#define RESET_HW
#ifdef RESET_HW
  #define RESET_HW_pin        PB0
  #define RESET_HW_DELAY      10
#endif

#define cbi(_byte, _bit)        _byte &= ~(1<<_bit)
#define sbi(_byte, _bit)        _byte |=  (1<<_bit)

#define MSBit                 0x80
#define LSBit                 0x01

void SPI_init(void);
uint8_t SPI_byte(uint8_t byte);


#endif /* SPI_H */