#include <spi.h>
#include <uart.h>
#include <string.h>
#include "nrfcfg.h"
#include "nrfbits.h"
#include <p33FJ256GP710.h>
#include "globals.h"


unsigned char nRFConfig[10];
unsigned char *nRFAddress = "MOTE";

PACKET PacketBuffer[BUFFER_SIZE];
int pckIn = 0; // Buffer in pointer
int pckOut = 0; // Buffer out pointer
PACKET tempPacket;
int bufFull = 0;
short currentSeqNo = 0;

void InitPort(void){
	// Port I/O configuration: 0=output, 1=input
	TX_EN_TRIS  = 0;  //TX_EN
	TRX_CE_TRIS = 0;  //TRX_CE
	PWR_UP_TRIS = 0;  //PWR_UP
	CD_TRIS     = 1;  //CD
	AM_TRIS     = 1;  //AM
	DR_TRIS     = 1;  //DR
	CSN_TRIS    = 0;  //CSN

	// nRF initial modes
	TX_EN = 0;  // Rx mode
	TRX_CE = 0; // Standby mode
	PWR_UP = 0; // Power down
	CSN = 0;    // Chip Select Not = enabled
}

//Init SPI1
void InitSPI(void){
	OpenSPI1(0x013F,0x0000,0x8000);
	DisableIntSPI1;
}

//SPI Write Read Function
unsigned char Spi1WriteRead(unsigned char in){
	unsigned char out;
	SPI1STATbits.SPIROV = 0; // Reset overflow bit
	SPI1BUF = in;//Write character to SPI buffer
	while(!SPI1STATbits.SPIRBF);// Wait for character
	out = SPI1BUF;//Read and discard data to avoid overflow 
	return out;
}



//Init NRF905=RF - Configuration � Register Description
void InitNRF(void){
	int i;
	nRFConfig[0] = CH_NO_BYTE; //Sets center frequency together with HFREQ_PLL
	nRFConfig[1] = PA_PWR__10dBm|HFREQ_PLL_433MHz|CH_NO_BIT8|RX_RED_PWR; //Output power,  Band 433 or 868/915 MHz
	nRFConfig[2] = TX_AFW_4BYTE|RX_AFW_4BYTE; //Tx and Rx Address width
	nRFConfig[3] = RX_PW_3BYTE; //RX payload width
	nRFConfig[4] = TX_PW_3BYTE; //Tx payload width
	nRFConfig[5] = nRFAddress[0]; 
	nRFConfig[6] = nRFAddress[1];
	nRFConfig[7] = nRFAddress[2];
	nRFConfig[8] = nRFAddress[3];
	nRFConfig[9] = CRC16_EN|XOF_16MHz; //CRC check. Crystal oscillator frequency. 
	
	PWR_UP = 1; 
	CSN = 1;
	CSN = 0; 
	Spi1WriteRead(WC); //Write nRF configuration
	for(i=0;i<10;i++)
		Spi1WriteRead(nRFConfig[i]);
	CSN = 1;
	
	CSN = 0;
	Spi1WriteRead(WTA); //Write TX Address
	for(i=0;i<4;i++)
		Spi1WriteRead(nRFAddress[i]);
	CSN = 1;
	
	TRX_CE = 1;
}

// Sending debug information to the USB interface
void debug(PACKET p){
	USBsend("Source: "); USBputc(p.SrcAddr);
	USBsend(" Destination: "); USBputc(p.DestAddr);
	if(p.type==RTS)
		USBsend(" Type:RTS\r\n");
	else if(p.type==CTS)
		USBsend(" Type:CTS\r\n");
	else if(p.type==ACK)
		USBsend(" Type:ACK\r\n");
	else if(p.type==PAY)
		USBsend(" Type:PAY\r\n");
	else USBsend(" Type:Unknown\r\n");
	
}

void RXPacket(int bytes2read){
	int i;
	char SpiRxBuf[bytes2read];
	TRX_CE = 0;
	CSN = 0;
	Spi1WriteRead(RRP); //Read RX Payload
	for (i=0;i<bytes2read;i++)
		SpiRxBuf[i] = Spi1WriteRead(0);
	CSN = 1;
	TRX_CE = 1;
	PACKET *p = (PACKET *)SpiRxBuf;
	tempPacket = p[0];
}


void TXPacket(char* packet,int bytes2send){
	
	TRX_CE = 0;
	TX_EN = 1;
	int i;
	CSN = 0;
	Spi1WriteRead(WTP); //Write TX Payload
	for(i=0;i<bytes2send;i++)
		Spi1WriteRead(packet[i]); 
	CSN = 1;

	TRX_CE = 1; // Start transmitting
	while(!DR); // Wait for transmission to finish

	TX_EN = 0;
}

void setNRFpw(char RXpw, char TXpw){
	int i;
	nRFConfig[3] = RXpw; //RX payload width
	nRFConfig[4] = TXpw; //Tx payload width
	CSN = 0; 
	Spi1WriteRead(WC3); //Write nRF configuration
	for(i=3;i<5;i++)
		Spi1WriteRead(nRFConfig[i]);
	CSN = 1;
}

void sendHeader(int type){
	tempPacket.type = type;
	if(type != RTS){
		tempPacket.DestAddr = tempPacket.SrcAddr; //Switch RX and TX address
		tempPacket.SrcAddr = moteAddress;
		if(type==CTS){
			tempPacket.bufFull = bufFull;
		}
	}
	TXPacket((char *)&tempPacket, 3);
	if(type==ACK) USBsend("ACK ");
	else if(type==CTS) USBsend("CTS ");
	else if(type==RTS) USBsend("RTS ");
	USBsend("header sent\r\n");
}


// Put packet in main buffer
int putInBuffer(PACKET pck){	
	// Find out who sent the packet and reconfigure addresses
	PACKET p = pck;
	
	USBsend("To put in buf: ");
	debug(p);
	
	if(p.SrcAddr == neighbor[0]){
		p.DestAddr = neighbor[1];
		USBsend("New dest addr: ");
		debug(p);
	}
		
	else if(p.SrcAddr == neighbor[1]){
		p.DestAddr = neighbor[0];
		USBsend("New dest addr: ");
		debug(p);
	}
	
	else{
		USBsend("Packet does not match neighbors\r\n");
		return;
	}
	p.SrcAddr = moteAddress;
	p.type = PAY;
	USBsend("Put in buf: ");
	debug(p);
	
	DisableIntU2RX;//Disable USB
	PacketBuffer[pckIn] = p; // Put the packet in the buffer
	pckIn = ++pckIn % BUFFER_SIZE;
	
	if(pckIn == pckOut)
		bufFull = 1;
	EnableIntU2RX;//Enable USB put command

}

void receive(void){
	while(CD); 
	sendHeader(CTS);//send CTS
	setNRFpw(RX_PW_32BYTE, TX_PW_3BYTE);
	
	if(!TimeOut(TIMEOUT)){
		RXPacket(32); // Data packet expected
		debug(tempPacket);
		if(tempPacket.type == PAY && tempPacket.DestAddr == moteAddress){
			if(tempPacket.seqNo == currentSeqNo){//if seqNo is correct, if not then discard the Pck(the Pck is not put in the buffer)
				putInBuffer(tempPacket);
				currentSeqNo = ++currentSeqNo % 2; // switch seqNo between 0 and 1
			}
			sendHeader(ACK);
		}
	}
	setNRFpw(RX_PW_3BYTE, TX_PW_3BYTE); // Reset Payload width to receive other packets
}

void send(void){
	setNRFpw(RX_PW_3BYTE, TX_PW_32BYTE);
	tempPacket = PacketBuffer[pckOut];
	while(CD);
	TXPacket((char *)&tempPacket, 32);
	setNRFpw(RX_PW_3BYTE, TX_PW_3BYTE);

	if(!TimeOut(TIMEOUT)){
		RXPacket(3); // ACK packet expected
		debug(tempPacket);
		if(tempPacket.type==ACK && tempPacket.DestAddr==moteAddress){
			pckOut = ++pckOut % BUFFER_SIZE; // Remove packet from buffer
			bufFull = 0;
			USBsend("Payload packet was sent\r\n");
			
		}
	}
	else USBsend("ACK timeout.\r\n "); /** Handle timeout **/
}

void send2mobile(void){
	PACKET p = PacketBuffer[pckOut];

	int i;
	char s[30];
	for(i=0;i<29;i++){
		s[i] = p.payload[i];
		BTputc(s[i]);
	}
	s[29] = 0;
		
	if(!BTtimeOut(BT_TIMEOUT)){
		pckOut = ++pckOut % BUFFER_SIZE; // Remove packet from buffer
		bufFull = 0;
		BTack = 0;
	}
	else {
		USBsend("BT ACK timeout\r\n");
	}
}



