#include <p33FJ256GP710.h>

#define BUFFER_SIZE 10
#define PAYLOAD_SIZE 29
#define NEWLINE "\r\n"
#define TIMEOUT 100 //wait for ACK in the send
#define BT_TIMEOUT 7000 //ACK timeout for the BT
#define BT_CONNECT_TIMEOUT 10000
#define BACKOFF 20
#define RTS 8
#define CTS 4
#define ACK 2
#define PAY 1


typedef struct{
	unsigned char DestAddr;
	unsigned char SrcAddr;
	unsigned int type		:4;
	unsigned int seqNo		:1;
	unsigned int bufFull	:1;
	unsigned int reserved	:2; // Not used yet
	unsigned char payload[PAYLOAD_SIZE];
} PACKET;

extern PACKET PacketBuffer[BUFFER_SIZE];
extern int pckIn; // Buffer in pointer
extern int pckOut; // Buffer out pointer
extern PACKET tempPacket;
extern int bufFull;
extern short BTack;
extern short BTreq;
extern unsigned short BTconnectTime;
extern short BTconnect;
extern short currentSeqNo;

extern unsigned char nRFConfig[10];
extern unsigned char moteAddress;
extern unsigned char neighbor[2];
extern unsigned char networkRole;