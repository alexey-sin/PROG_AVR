#ifndef 	_NRF905_H_
#define 	_NRF905_H_

typedef unsigned char	INT8U;
typedef unsigned int	INT16U;

#define CH_NO			0x01
#define HFREQ_PLL		0
#define PA_PWR			3
#define RX_RED_PWR		0
#define AUTO_RETRAN		0
#define RX_AWF			4
#define TX_AWF			4
#define RX_PW			32
#define TX_PW			32
#define RX_ADDRESS		(0xCCCCCCCC)L
#define UP_CLK_FREQ		0
#define UP_CLK_EN		0
#define XOF				3
#define CRC_EN			1
#define CRC_MODE		0		

/*
***********************************************************************************************
*							Pin definition, HiTech C PICC Lite compiler specific
*							Hardware: nRF_CB_Eval_1a
***********************************************************************************************
*/

#define TRIS_RACSN		TRISC6
#define RACSN			RC6
#define TRIS_SPI_SCK	TRISB6
#define SPI_SCK			RB6
#define TRIS_SPI_MOSI	TRISC7
#define SPI_MOSI		RC7
#define TRIS_SPI_MISO	TRISB4
#define SPI_MISO		RB4

#define TRIS_TXEN		TRISC3
#define TXEN			RC3
#define TRIS_TRX_CE		TRISC4
#define TRX_CE			RC4
#define TRIS_PWR		TRISC5
#define PWR				RC5

#define TRIS_CD			TRISA4
#define CD				RA4
#define TRIS_AM			TRISA5
#define AM				RA5
#define TRIS_DR			TRISA2
#define DR				RA2

extern INT8U TxBuf[32];
extern INT8U RxBuf[32];

/*
***********************************************************************************************
*									FUNCTION PROTOTYPES
***********************************************************************************************
*/
void rfTxPacket(void);
void rfRxPacket(void);
void rfConfig905(void);
void rfSetRxMode(void);
void rfSetChannel(void);


/*
***********************************************************************************************
*									FUNCTION PROTOTYPES
*									 HARDWARE SPECIFIC
***********************************************************************************************
*/
void 	rfInitIO(void);
void 	spiWrite(INT8U dat);
INT8U 	spiRead(void);
void 	rfSetPwrOff(void);

#endif
