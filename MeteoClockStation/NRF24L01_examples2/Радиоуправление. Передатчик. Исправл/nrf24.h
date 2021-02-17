/*
* ----------------------------------------------------------------------------
* РІР‚СљTHE COFFEEWARE LICENSEРІР‚Сњ (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* This library is based on this library:
*   https://github.com/aaronds/arduino-nrf24l01
* Which is based on this library:
*   http://www.tinkerer.eu/AVRLib/nRF24L01
* -----------------------------------------------------------------------------
*/
#ifndef NRF24
#define NRF24

#include "nRF24L01.h"
#include <stdint.h>

#define LOW 0
#define HIGH 1

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

/* adjustment functions */
void nrf24_init(void);
void nrf24_rx_address(unsigned char* adr);
void nrf24_tx_address(unsigned char* adr);
void nrf24_config(unsigned char channel, unsigned char pay_length);

/* state check functions */
unsigned char nrf24_dataReady(void);
unsigned char nrf24_isSending(void);
unsigned char nrf24_getStatus(void);
unsigned char nrf24_rxFifoEmpty(void);

/* core TX / RX functions */
void nrf24_send(unsigned char* value);
void nrf24_getData(unsigned char* data);

/* use in dynamic length mode */
unsigned char nrf24_payloadLength(void);

/* post transmission analysis */
unsigned char nrf24_lastMessageStatus(void);
unsigned char nrf24_retransmissionCount(void);

/* Returns the payload length */
unsigned char nrf24_payload_length(void);

/* power management */
void nrf24_powerUpRx(void);
void nrf24_powerUpTx(void);
void nrf24_powerDown(void);

/* low level interface ... */
unsigned char spi_transfer(unsigned char tx);
void nrf24_transmitSync(unsigned char* dataout, unsigned char len);
void nrf24_transferSync(unsigned char* dataout, unsigned char* datain, unsigned char len);
void nrf24_configRegister(unsigned char reg, unsigned char value);
void nrf24_readRegister(unsigned char reg, unsigned char* value, unsigned char len);
void nrf24_writeRegister(unsigned char reg, unsigned char* value, unsigned char len);

/* -------------------------------------------------------------------------- */
/* You should implement the platform spesific functions in your code */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* In this function you should do the following things:
 *    - Set MISO pin input
 *    - Set MOSI pin output
 *    - Set SCK pin output
 *    - Set CSN pin output
 *    - Set CE pin output     */
/* -------------------------------------------------------------------------- */
//extern void nrf24_setupPins();

/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_ce_digitalWrite(unsigned char state);

/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_csn_digitalWrite(unsigned char state);

/* -------------------------------------------------------------------------- */
/* nrf24 SCK pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_sck_digitalWrite(unsigned char state);

/* -------------------------------------------------------------------------- */
/* nrf24 MOSI pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_mosi_digitalWrite(unsigned char state);

/* -------------------------------------------------------------------------- */
/* nrf24 MISO pin read function */
/* - returns: Non-zero if the pin is high */
/* -------------------------------------------------------------------------- */
extern unsigned char nrf24_miso_digitalRead(void);

#endif
