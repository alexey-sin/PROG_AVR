#include "nrf24.h"

unsigned char payload_len;

/* init the hardware pins */
void nrf24_init(){
    //setup pins
    ClrBit(NRF_DDR, NRF_IRQ); //IRQ input
    SetBit(NRF_DDR, NRF_CE); // CE output
    SetBit(NRF_DDR, NRF_CSN); // CSN output
    SetBit(NRF_DDR, NRF_SCK); // SCK output
    SetBit(NRF_DDR, NRF_MOSI); // MOSI output
    ClrBit(NRF_DDR, NRF_MISO); // MISO input  


	ClrBit(NRF_PORT, NRF_CE);
    SetBit(NRF_PORT, NRF_CSN);
}

/* configure the module */
void nrf24_config(unsigned char channel, unsigned char pay_length){
    /* Use static payload length ... */
    payload_len = pay_length;

    // Set RF channel
    nrf24_configRegister(RF_CH,channel);	// 0 - 126

    // Set length of incoming payload 
    nrf24_configRegister(RX_PW_P0, payload_len); // Auto-ACK pipe ...
    nrf24_configRegister(RX_PW_P1, 0x00); // Data payload pipe
    nrf24_configRegister(RX_PW_P2, 0x00); // Pipe not used 
    nrf24_configRegister(RX_PW_P3, 0x00); // Pipe not used 
    nrf24_configRegister(RX_PW_P4, 0x00); // Pipe not used 
    nrf24_configRegister(RX_PW_P5, 0x00); // Pipe not used 

    // 250 Kbps, TX gain: 0dbm
    nrf24_configRegister(RF_SETUP, (1 << RF_DR_L)|(0 << RF_DR_H)|((0x03)<<RF_PWR));

    // CRC enable, 1 byte CRC length
    nrf24_configRegister(CONFIG, nrf24_CONFIG);

    // Not Auto Acknowledgment
    nrf24_configRegister(EN_AA, (0<<ENAA_P0)|(0<<ENAA_P1)|(0<<ENAA_P2)|(0<<ENAA_P3)|(0<<ENAA_P4)|(0<<ENAA_P5));

    // Enable RX addresses
    nrf24_configRegister(EN_RXADDR, (1<<ERX_P0)|(0<<ERX_P1)|(0<<ERX_P2)|(0<<ERX_P3)|(0<<ERX_P4)|(0<<ERX_P5));

    // Auto retransmit delay: 1000 us and Up to 15 retransmit trials
    nrf24_configRegister(SETUP_RETR, (0x04 << ARD)|(0x0F << ARC));
    // nrf24_configRegister(SETUP_RETR, (0 << ARD)|(0 << ARC));

    // Dynamic length configurations: No dynamic length
    nrf24_configRegister(DYNPD, (0<<DPL_P0)|(0<<DPL_P1)|(0<<DPL_P2)|(0<<DPL_P3)|(0<<DPL_P4)|(0<<DPL_P5));

    // Start listening
    nrf24_powerUpRx();
}

/* Set the RX address */
void nrf24_rx_address(unsigned char* adr){
    ClrBit(NRF_PORT, NRF_CE);
    nrf24_writeRegister(RX_ADDR_P1, adr, nrf24_ADDR_LEN);
    SetBit(NRF_PORT, NRF_CE);
}

/* Returns the payload length */
unsigned char nrf24_payload_length(void){
    return payload_len;
}

/* Set the TX address */
void nrf24_tx_address(unsigned char* adr){
    /* RX_ADDR_P0 должен быть установлен на адрес отправителя для автоматического подтверждения работы. */
    nrf24_writeRegister(RX_ADDR_P0, adr, nrf24_ADDR_LEN);
    nrf24_writeRegister(TX_ADDR, adr, nrf24_ADDR_LEN);
}

/* Проверяет, доступны ли данные для чтения */
/* Возвращает 1, если данные готовы ... */
unsigned char nrf24_dataReady(void){
    // See note in getData() function - just checking RX_DR isn't good enough
    unsigned char status = nrf24_getStatus();

    // We can short circuit on RX_DR, but if it's not set, we still need
    // to check the FIFO for any pending packets
    if(status & (1 << RX_DR)) return 1;

    return !nrf24_rxFifoEmpty();
}

/* Checks if receive FIFO is empty or not */
unsigned char nrf24_rxFifoEmpty(void){
    unsigned char fifoStatus;

    nrf24_readRegister(FIFO_STATUS, &fifoStatus, 1);
    
    return (fifoStatus & (1 << RX_EMPTY));
}

/* Returns the length of data waiting in the RX fifo */
unsigned char nrf24_payloadLength(void){
    unsigned char status;
    ClrBit(NRF_PORT, NRF_CSN);
    spi_transfer(R_RX_PL_WID);
    status = spi_transfer(0x00);
    return status;
}

/* Reads payload bytes into data array */
void nrf24_getData(unsigned char* data){
    /* Pull down chip select */
    ClrBit(NRF_PORT, NRF_CSN);                               

    /* Send cmd to read rx payload */
    spi_transfer(R_RX_PAYLOAD);
    
    /* Read payload */
    nrf24_transferSync(data, data, payload_len);
    
    /* Pull up chip select */
    SetBit(NRF_PORT, NRF_CSN);

    /* Reset status register */
    nrf24_configRegister(STATUS, (1<<RX_DR));   
}

/* Returns the number of retransmissions occured for the last message */
unsigned char nrf24_retransmissionCount(void){
    unsigned char rv;
    nrf24_readRegister(OBSERVE_TX, &rv, 1);
    rv = rv & 0x0F;
    return rv;
}

// Отправляет пакет данных на адрес по умолчанию. Обязательно отправьте правильное
// количество байтов, настроенное как полезная нагрузка на получателе.
void nrf24_send(unsigned char* value){    
    /* Go to Standby-I first */
    ClrBit(NRF_PORT, NRF_CE);
     
    //В режим передачи
    nrf24_powerUpTx();
	
    // Очищаем TX fifo
	ClrBit(NRF_PORT, NRF_CSN);           
	spi_transfer(FLUSH_TX);     
	SetBit(NRF_PORT, NRF_CSN);                    

    /* Pull down chip select */
    ClrBit(NRF_PORT, NRF_CSN);

    /* Write cmd to write payload */
    spi_transfer(W_TX_PAYLOAD);

    /* Write payload */
    nrf24_transmitSync(value, payload_len);   

    /* Поднимем выбор чипа */
    SetBit(NRF_PORT, NRF_CSN);

    /* Старт передачи */
    SetBit(NRF_PORT, NRF_CE);    
}

unsigned char nrf24_isSending(void){
    unsigned char status;

    /* чтение текущего статуса */
    status = nrf24_getStatus();
                
    /* если отправка успешна (TX_DS) или превышен максимум повторов (MAX_RT). */
    if((status & ((1 << TX_DS)|(1 << MAX_RT)))) return 0; /* false */

    return 1; /* true */

}

unsigned char nrf24_getStatus(void){
    unsigned char rv;
    ClrBit(NRF_PORT, NRF_CSN);
    rv = spi_transfer(NOP);
    SetBit(NRF_PORT, NRF_CSN);
    return rv;
}

unsigned char nrf24_lastMessageStatus(void){
    unsigned char rv;

    rv = nrf24_getStatus();

    /* Transmission went OK */
    if((rv & ((1 << TX_DS)))) return NRF24_TRANSMISSON_OK;
    /* Maximum retransmission count is reached */
    /* Last message probably went missing ... */
    else if((rv & ((1 << MAX_RT)))) return NRF24_MESSAGE_LOST;
    /* Probably still sending ... */
    else return 0xFF;
}

void nrf24_powerUpRx(void){     
    ClrBit(NRF_PORT, NRF_CSN);
    spi_transfer(FLUSH_RX);
    SetBit(NRF_PORT, NRF_CSN);

    nrf24_configRegister(STATUS, (1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT)); 

    ClrBit(NRF_PORT, NRF_CE);   
    nrf24_configRegister(CONFIG, nrf24_CONFIG|((1<<PWR_UP)|(1<<PRIM_RX)));    
    SetBit(NRF_PORT, NRF_CE);
}

void nrf24_powerUpTx(void){
    nrf24_configRegister(STATUS, (1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT)); 
    nrf24_configRegister(CONFIG, nrf24_CONFIG|((1<<PWR_UP)|(0<<PRIM_RX)));
}

void nrf24_powerDown(void){
    ClrBit(NRF_PORT, NRF_CE);
    nrf24_configRegister(CONFIG,nrf24_CONFIG);
}

/* software spi routine */
unsigned char spi_transfer(unsigned char tx){
    unsigned char i = 0;
    unsigned char rx = 0;    

    ClrBit(NRF_PORT, NRF_SCK);

    for(i = 0;i < 8;i++){
        if(tx & (1 << (7 - i))) SetBit(NRF_PORT, NRF_MOSI);            
        else ClrBit(NRF_PORT, NRF_MOSI);

        SetBit(NRF_PORT, NRF_SCK);        

        rx = rx << 1;
        if(BitIsSet(NRF_PIN, NRF_MISO)) rx |= 0x01;

        ClrBit(NRF_PORT, NRF_SCK);                
    }

    return rx;
}

/* посылка и прием множества байтов через SPI */
void nrf24_transferSync(unsigned char* dataout, unsigned char* datain, unsigned char len){
    unsigned char i;

    for(i = 0;i < len;i++) datain[i] = spi_transfer(dataout[i]);

}

/* посылка множества байтов через SPI */
void nrf24_transmitSync(unsigned char* dataout, unsigned char len){
    unsigned char i;
    
    for(i = 0;i < len;i++) spi_transfer(dataout[i]);

}

/* Clocks only one byte into the given nrf24 register */
void nrf24_configRegister(unsigned char reg, unsigned char value){
    ClrBit(NRF_PORT, NRF_CSN);
    spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
    spi_transfer(value);
    SetBit(NRF_PORT, NRF_CSN);
}

/* Чтение одиночного регистра из nrf24 */
void nrf24_readRegister(unsigned char reg, unsigned char* value, unsigned char len){
    ClrBit(NRF_PORT, NRF_CSN);
    spi_transfer(R_REGISTER | (REGISTER_MASK & reg));
    nrf24_transferSync(value,value,len);
    SetBit(NRF_PORT, NRF_CSN);
}

/* Запись в одиночный регистр nrf24 */
void nrf24_writeRegister(unsigned char reg, unsigned char* value, unsigned char len){
    ClrBit(NRF_PORT, NRF_CSN);
    spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
    nrf24_transmitSync(value,len);
    SetBit(NRF_PORT, NRF_CSN);
}


