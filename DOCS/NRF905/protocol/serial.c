#include "serial.h"
#include "globals.h"

char USBbuffer[40]; //Received data is stored in buffer 
char BTbuffer[30];
short BTack = 0;
short BTcounter = 0;
short BTconnect = 0;

void InitUART2(){ 
   OpenUART2(0x8000,0x8570,5); //configures the UART module for the USB interface (baud rate = 57600)
   EnableIntU2RX; //Enable interrupt
   SetPriorityIntU2RX(UART_RX_INT_PR6); //Priority 6
}

void InitUART1(){    
   OpenUART1(0x8000,0x8570,2); //configures the UART module for the Bluetooth interface (baud rate = 19200)
   EnableIntU1RX; //Enable interrupt
   SetPriorityIntU1RX(UART_RX_INT_PR7); //Priority 7
}

void BTsend(char *ToBeSent){
	putsUART1((unsigned int*)ToBeSent);
	while(BusyUART1()); //wait for transmission to complete
}

void BTputc(char ToBeSent){
	putcUART1((unsigned int)ToBeSent);
	while(BusyUART1()); //wait for transmission to complete
}

void USBsend(char* ToBeSent){
	putsUART2((unsigned int*)ToBeSent);
	while(BusyUART2()); //wait for transmission to complete
}

void USBputc(char ToBeSent){
	putcUART2((unsigned int)ToBeSent);
	while(BusyUART2()); //wait for transmission to complete
}

void BToutput(void){	
	if(BTbuffer[0] == 'A'){
		USBsend("Received ACK from mobile phone\r\n ");
		BTack = 1;
	}
	else if(BTbuffer[0] == 'E'){
		USBsend("Mobile phone closed the connection by END \r\n ");
		BTconnect = 0;
		BTack = 1;
	}
}

void BTinput(void){
	if(BTcounter==28){
		BTcounter = 0;
		PACKET p;
		int i;
		for(i=0;i<29;i++){
			p.payload[i] = BTbuffer[i];
		}
		p.type = PAY;
		p.SrcAddr = moteAddress;
		if(neighbor[0]=='M') p.DestAddr = neighbor[1];
		else p.DestAddr = neighbor[0];
		p.seqNo = currentSeqNo;
		currentSeqNo = ++currentSeqNo % 2; // switch seqNo between 0 and 1		
		PacketBuffer[pckIn] = p; // Put the packet in the buffer
		pckIn = ++pckIn % BUFFER_SIZE; //cirkular buffer
		if(pckIn == pckOut) bufFull = 1;
		BTreq = 0;
		BTconnectTime = 0;
	}
	else BTcounter++;
}

/********* START OF INTERRUPT SERVICE ROUTINES ********/

//Bluetooth interface interrupt
void __attribute__((__interrupt__)) _U1RXInterrupt(void){  //interrupt is called for each byte

	BTbuffer[BTcounter]=ReadUART1();
	
	if(BTconnect){
		if(networkRole=='O'){
			BToutput();
		}
		
		else if(networkRole=='I'){
			BTinput();
		}
		else USBsend("Wrong BT role!\r\n");
	}
	else{
		if(BTbuffer[BTcounter]==10){ // '\n' received
			BTbuffer[BTcounter+1] = 0;
			if(strncmp(BTbuffer, "INIT", 4)==0){ // Check if an init string is received and set connect flag
				BTconnect = 1;
				if(networkRole=='I') InitTimer2();
				BTack = 0;
				USBsend("Init received\r\n");
			}
			BTcounter = 0;
		}
		else BTcounter++;
	}
		
	//clear interrupt flag
	IFS0bits.U1RXIF = 0;

	return;
}

//USB interrupt
void __attribute__((__interrupt__)) _U2RXInterrupt(void){  //interrupt is called for each byte
	static unsigned char IntRcv=0; 
	USBbuffer[IntRcv]=ReadUART2();
	if (USBbuffer[IntRcv]==10) { // '\n' received
		USBbuffer[IntRcv+1]=0;// add \0 as the last char
		command(USBbuffer);
		IntRcv=0;
		}
	else IntRcv++;
	IFS1bits.U2RXIF = 0;//clear interrupt flag
	
	return;
}

/********* END OF INTERRUPT SERVICE ROUTINES ********/
