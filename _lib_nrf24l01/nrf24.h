#ifndef NRF24
#define NRF24

#include <avr/io.h>
#include "../../_libs_macros/bits_macros.h"
#include <inttypes.h>
#include <stdint.h>
#include <nrf24_config.h>

/**
ниже описанные define должны лежать в файле nrf24_config.h в src проекта
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
*/


/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD       0x1C
#define FEATURE     0x1D

/* Bit Mnemonics */

/* Feature*/
#define EN_ACK_PAY 1

/* configuratio nregister */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0

/* enable auto acknowledgment */
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0

/* enable rx addresses */
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0

/* setup of address width */
#define AW          0 /* 2 bits */

/* setup of auto re-transmission */
#define ARD         4 /* 4 bits */
#define ARC         0 /* 4 bits */

/* RF setup register */
#define PLL_LOCK    4
#define RF_DR_H     3
#define RF_DR_L     5
#define RF_PWR      1 /* 2 bits */   

/* general status register */
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1 /* 3 bits */
#define TX_FULL     0

/* transmit observe register */
#define PLOS_CNT    4 /* 4 bits */
#define ARC_CNT     0 /* 4 bits */

/* fifo status */
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0

/* dynamic length */
#define DPL_P0      0
#define DPL_P1      1
#define DPL_P2      2
#define DPL_P3      3
#define DPL_P4      4
#define DPL_P5      5

/* Instruction Mnemonics */
#define R_REGISTER    0x00 /* last 4 bits will indicate reg. address */
#define W_REGISTER    0x20 /* last 4 bits will indicate reg. address */
#define REGISTER_MASK 0x1F
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define ACTIVATE      0x50 
#define R_RX_PL_WID   0x60
#define W_ACK_PAYLOAD 0xA8 // + 3 последние бита - адрес куда передавать
#define NOP           0xFF




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

/* использовать в режиме динамической длины */
unsigned char nrf24_payloadLength(void);

/* анализ после передачи */
unsigned char nrf24_lastMessageStatus(void);
unsigned char nrf24_retransmissionCount(void);

/* Возвращает длину */
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

#endif


