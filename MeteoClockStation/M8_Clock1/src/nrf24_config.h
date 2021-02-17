#ifndef NRF24_CONFIG
#define NRF24_CONFIG

#define nrf24_ADDR_LEN 5
#define nrf24_CONFIG ((1 << EN_CRC)|(0 << CRCO))

#define NRF24_TRANSMISSON_OK 0
#define NRF24_MESSAGE_LOST 1

#define NRF_PORT PORTB
#define NRF_DDR DDRB
#define NRF_PIN PINB
#define NRF_CE 0
#define NRF_CSN 2
#define NRF_SCK 5
#define NRF_MOSI 3 
#define NRF_MISO 4
#define NRF_IRQ 1

#endif

