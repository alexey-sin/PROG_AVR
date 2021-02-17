#include "globals.h"
#include "serial.h"
#include <stdio.h>
#include <stdlib.h>


unsigned char moteAddress = '2';  // Default value for mote 2. Can also be changed in the command line interface
unsigned char neighbor[2] = {'1','3'}; // Default value for mote 2. Can also be changed in the command line interface
unsigned char networkRole = 'F'; // Default value for forwarder motes. Can also be changed in the command line interface

void command(char *cmd){
	// "put" command only for testing
	if(strncmp(cmd, "put ", 4)==0){
		if(bufFull) USBsend("Buffer is full\r\n");
		else{
			char *msg = &cmd[4];
			PACKET p;
			strcpy(p.payload, msg);
			USBsend(p.payload);
			p.DestAddr = neighbor[1];
			p.SrcAddr = moteAddress;
			p.type = PAY;
			p.seqNo = currentSeqNo;
			currentSeqNo = ++currentSeqNo % 2; // switch seqNo between 0 and 1
			PacketBuffer[pckIn] = p;
			pckIn = ++pckIn % BUFFER_SIZE;

			USBsend("Packet put in buffer\r\n");
			if(pckIn == pckOut) bufFull = 1;
		}
	}
	
	//view the nb. of packets in the buffer
	else if(strncmp(cmd, "buf", 3)==0){
		if(pckIn != pckOut){
		    USBsend("Outgoing packet is: ");
			USBsend(PacketBuffer[pckOut].payload);
			USBsend(NEWLINE);
			
			char s[25];
			
			sprintf(s, "Nb. pck in buf: %d%s",(pckIn-pckOut+BUFFER_SIZE)%BUFFER_SIZE ,NEWLINE);
			USBsend(s);
		}
		else USBsend("No packets in buffer\r\n");
	}
	
	// change mote address
	else if(strncmp(cmd, "chaddr ", 7)==0){
		moteAddress = cmd[7];
		USBsend("Address changed to: ");
		USBputc(moteAddress);
		USBsend(NEWLINE);
	}
	
	// view mote settings
	else if(strncmp(cmd, "moteset", 7)==0){
		USBsend("Mote address: "); USBputc(moteAddress); USBsend(NEWLINE);
		USBsend("Neighbors: "); USBputc(neighbor[0]); USBsend(" and "); USBputc(neighbor[1]); USBsend(NEWLINE);
		USBsend("Network role: "); USBputc(networkRole); USBsend(NEWLINE);
	}
	
	// view nRF settings
	else if(strncmp(cmd, "ISM", 6)==0){
		int i = 0;
		char *s[10];
		
		for(i=0;i<10;i++)
			s[i] = (char*)malloc(35*sizeof(char));
		
		i = 0;
		
		sprintf(s[i], "Center frequency: 0x%x%s", nRFConfig[i++], NEWLINE);
		sprintf(s[i], "Frequency band and power: 0x%x%s", nRFConfig[i++], NEWLINE);
		sprintf(s[i], "Tx and Rx Address width: 0x%x%s", nRFConfig[i++], NEWLINE);
		sprintf(s[i], "RX payload width: 0x%x%s", nRFConfig[i++], NEWLINE);
		sprintf(s[i], "TX payload width: 0x%x%s", nRFConfig[i++], NEWLINE);
		sprintf(s[i], "nRF Adress, byte1: 0x%x%s", nRFConfig[i++], NEWLINE);
		sprintf(s[i], "nRF Adress, byte2: 0x%x%s", nRFConfig[i++], NEWLINE);
		sprintf(s[i], "nRF Adress, byte3: 0x%x%s", nRFConfig[i++], NEWLINE);
		sprintf(s[i], "nRF Adress, byte4: 0x%x%s", nRFConfig[i++], NEWLINE);
		sprintf(s[i], "CRC and Clock: 0x%x%s", nRFConfig[i++], NEWLINE);
		
		for(i=0;i<10;i++)
			USBsend(s[i]);	
	}
	
	// role in the network
	else if(strncmp(cmd, "role ", 5)==0){
		// check for correct input
		if(cmd[5]=='I' || cmd[5]=='F' || cmd[5]=='O'){
			networkRole = cmd[5];
			USBsend("Role changed to: ");
			USBputc(networkRole);
			USBsend(NEWLINE);
		}
		else {
			USBsend("Wrong parameter given. Role not changed\r\n");
		}
	}
	
	// set neighbors of the mote
	else if(strncmp(cmd, "neighbors ", 10)==0){
		neighbor[0] = cmd[10];
		neighbor[1] = cmd[12];
		USBsend("Neighbors set to: "); USBputc(neighbor[0]); USBsend(" and "); USBputc(neighbor[1]); USBsend(NEWLINE);
	}
	
	else{
		USBsend("Wrong command\r\n");
	}
	
	USBsend(">> ");
}

