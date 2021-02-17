#include <htc.h>
#include "delay.h"
#include "nRF905.h"

/*
****************************************************************************************
* Instruction set for the nRF905 SPI interface; refer to page 16 of nRF905 datasheet
* The available commands to be used on the SPI interface is shown below. Whenever
* CSN is set low the interface expects an instruction. Every new instruction must be
* started by a high to low transition on CSN.
* A read or a write operation may operate on a single byte or on a set of succeeding
* bytes from a given start address defined by the instruction.
****************************************************************************************
*/
#define WRC	0x00	//Write configuration register command
#define RRC	0x10	//Read configuration register command
#define WTP	0x20	//Wrte TX Payload command
#define RTP	0x21	//Read TX Payload command
#define WTA	0x22	//Write TX Address command
#define RTA	0x23	//Read TX address command
#define RRP	0x24	//Read RX Payload command
#define CC	0x80	//Channel configuration

#define OUTPUT			0
#define INPUT			1
#define HIGH			1
#define LOW				0

typedef struct _RFconfig
{
	INT8U n;
	INT8U buf[10];	//RF-CONFIG REGISTER (refer to page 20 of nRF905 datasheet)
}RFconfig;

/*
* Initialization of the Register Contents at byte# 0 - 9 as stated on page 20 of nRF905 datasheet
*/
const RFconfig RxTxconfig =
{
	10,
	0x01,			//byte# 0 : set the channel # to 1
	0x0e,			//byte# 1 : 0bxx[AUTO_RETRAN<5>][RX_RED_PWR<4>][PA_PWR<3:2>][HFREQ_PLL<1>][CH_NO[8]<0>]
					//			0b00001100 => 	AUTO_RETRAN=0; 	No retransmission of data packet
					//							RX_RED_PWR=0; 	Normal operation
					//							PA_PWR=11;		+10dBm output power
					//							HFREQ_PLL=1;	chip operating in 868 MHz band
					//							CH_NO[8]=0;		9th bit of CH_NO is zero
	0x44,			//byte# 2 :	TX_AFW[2:0]=100 => 4 byte TX address field width
 					//			RX_AFW[2:0]=100 => 4 byte RX address field width	
	0x20,			//byte# 3 : RX_payload width of 32 bytes
	0x20,			//byte# 4 : TX_payload width of 32 bytes
	0xcc,			//byte# 5 :	RX_ADDRESS byte 0
	0xcc,			//byte# 6 : RX_ADDRESS byte 1
	0xcc,			//byte# 7 : RX_ADDRESS byte 2
	0xcc,			//byte# 8 : RX_ADDRESS byte 3; thus the RX ADDRESS is configured to 0xcccccccc in this example
					//remarks : The default value is 0xE7E7E7E7
	0x58			//byte# 9 : Set the CRC_MODE, CRC_EN, XOF[2:0], UP_CLK_EN, UP_CLK_FREQ[1:0] bit values
					//			In this configuration, we are doing CRC enable for 8 CRC check bit, 16MHz crystal freq, 
					//			and output clock disable
};

/*
**********************************************************************************************************
void rfSetChannel(void)
{
	static INT8U tmp;

	RACSN = LOW;
	spiWrite(RRC|0x01);			//read the configuration address starting address 1
	tmp = spiRead()&0xfc;		//clear the relevant HFREQ_PLL and CH_NO[8] bits
	RACSN = HIGH;

	RACSN = LOW;
	spiWrite(WRC);				//write the configuration register starting from address 0
	spiWrite((INT8U)CHANNEL);	//write bit[7:0] of the channel, leaving CH_NO[8] in the second register
	spiWrite(tmp|(HFREQ<<1)|((CHANNEL>>8)&0x01));
	RACSN = HIGH;
}
**********************************************************************************************************
*/


/* declaration of Tx and Rx buffers */
INT8U TxBuf[32];
INT8U RxBuf[32];

/*
ShockBurst TX routine, send the whole paylod buffer in 32 bytes to a designated receiving node (TX-address)
*/
void rfTxPacket(void)
{
	INT8U i;

	PWR 		= HIGH;
	TXEN		= HIGH;
	TRX_CE		= LOW;

	RACSN = LOW;			//chip select
	spiWrite(WTA);			//write 4 bytes TX-ADDRESS
	for(i=0; i<4; i++)
	{
		spiWrite(RxTxconfig.buf[i+5]);
	}
	RACSN = HIGH;

	for(i=0;i<200;i++);

	RACSN = LOW;			//chip select
	spiWrite(WTP);
	for (i=0;i<32;i++)
	{	
		spiWrite(TxBuf[i]);	//write payload data here
	}
	RACSN = HIGH;			//chip de-select			

	TRX_CE = HIGH;		//radio enabled
	DelayUs(20);		
	TRX_CE = LOW;		//Since no auto-transmit in this example
							//TRX_CE set low for standby mode
}


void rfRxPacket(void)
{
	INT8U i;

	TRX_CE = LOW;		//switch to standby mode
	RACSN = LOW;		//chip select
	spiWrite(RRP);		//read payload command
	for(i=0;i<32;i++)
		RxBuf[i] = spiRead();
	RACSN = LOW;
	while(DR||AM);
	TRX_CE=HIGH;
}


void rfConfig905(void)
{
	INT8U i;

	PWR 	= HIGH;
	TRX_CE 	= LOW;

	RACSN = LOW;					//chip enable
	spiWrite(WRC);					//repeat SPI write
	for(i=0;i<RxTxconfig.n;i++)
	{
		spiWrite(RxTxconfig.buf[i]);
	}
	RACSN = HIGH;					//chip disable
}

void rfSetRxMode(void)
{
	/* Set Receive mode */
	PWR			= HIGH;
	TXEN		= LOW;
	TRX_CE		= HIGH;
	/* After 650us nRF905 is monitoring the air for incoming communication */
	DelayUs(200);
	DelayUs(200);
	DelayUs(200);
	DelayUs(50);	
}

void rfInitIO(void)
{
	RACSN			= HIGH;			//de-select the RF module to start with
	TRIS_RACSN  	= OUTPUT;
	SPI_SCK			= LOW;			//clock line low to start with
	TRIS_SPI_SCK  	= OUTPUT;
	TRIS_SPI_MOSI 	= OUTPUT;
	TRIS_SPI_MISO 	= INPUT;

	TRIS_PWR		= OUTPUT;
	TRIS_TRX_CE 	= OUTPUT;
	TRIS_TXEN 		= OUTPUT;

	TRIS_CD			= INPUT;
	TRIS_AM			= INPUT;
	TRIS_DR			= INPUT;
}

void spiWrite(INT8U dat)
{
	/* software SPI, send MSB first */
	static INT8U i, c;

	c = dat;
	for(i=0;i<8;i++)
	{
	if((c&0x80)==0x80)
		SPI_MOSI = HIGH;
	else
		SPI_MOSI = LOW;

	SPI_SCK = HIGH;
	c=c<<1;
	SPI_SCK = LOW;
	}
}

INT8U spiRead(void)
{
	/* software SPI read, MSB read first */
	static INT8U i, dat;

	for(i=0;i<8;i++)
	{
		dat = dat<<1;
		SPI_SCK = HIGH;
		if(SPI_MISO)
			dat = dat+1;
		SPI_SCK = LOW;
	}
	
	return dat;
}

void rfSetPwrOff(void)
{
	PWR = LOW;
}
