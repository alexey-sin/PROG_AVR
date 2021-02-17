// Port config for nRF905 ( pin function)
#define TX_EN  LATDbits.LATD1 //TX_EN=1 TX mode, TX_EN=0 RX mode
#define TRX_CE LATDbits.LATD2 //Enables chip for receive and transmit
#define PWR_UP LATDbits.LATD3 //Power up chip
#define CD     PORTDbits.RD5 //Carrier Detect 
#define AM     PORTDbits.RD6 //Address Match
#define DR     PORTDbits.RD7 //Receive and transmit Data Ready
#define CSN    LATDbits.LATD4 //SPI enable, active low
#define TX_EN_TRIS  TRISDbits.TRISD1 
#define TRX_CE_TRIS TRISDbits.TRISD2
#define PWR_UP_TRIS TRISDbits.TRISD3
#define CD_TRIS     TRISDbits.TRISD5
#define AM_TRIS     TRISDbits.TRISD6
#define DR_TRIS     TRISDbits.TRISD7
#define CSN_TRIS    TRISDbits.TRISD4
