#include "serial.h"
#include "nrfbits.h"
#include "globals.h"


_FWDT(FWDTEN_OFF);  // Disable watchdog timer
_FOSCSEL(FNOSC_PRIPLL);  // Clock source selection
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF  & POSCMD_HS); // Disable clock switching and monitor & pin function is clock & HS oscillator

short BTreq = 0; // no request sent yet 

int main ( void ){ 
	// PLL settings for Fcy = 22,118 MHz / 4
	_PLLDIV = 0;
	_PLLPRE = 0;
	_PLLPOST = 0;

	// Watch-Dog-Timer switch off
	RCONbits.SWDTEN=0;

	// Wait for PLL to stabalize
	while(OSCCONbits.LOCK!=1);

	// Initialization
	InitUART2();
	InitUART1();
    	InitPort();
	InitSPI();
	InitNRF();
	
	while(1){
		
		if(AM){
			BTconnectTime = 0;
			if(DR){
				USBsend("Receive packet: ");
				//read nRF
				RXPacket(3); // RTS expected
				debug(tempPacket);
				switch(tempPacket.type){//Check header
					case RTS:
						if (tempPacket.DestAddr==moteAddress){
							receive();
						}
						else Pause(BACKOFF);
						break;
					case CTS:
						if (tempPacket.DestAddr==moteAddress && !tempPacket.bufFull){
							send();
							//USBsend("Sent payload.\r\n ");
						}
						else Pause(BACKOFF);
						break;
					default: 
					
						break;
				}
			}
		}
		
		
		else{
			if(pckIn != pckOut||bufFull){ // There are packets in the buffer
				if(networkRole == 'O'){
					if(BTconnect) send2mobile();
				}
					
				else{
					// send procedure
					tempPacket = PacketBuffer[pckOut];
					sendHeader(RTS);
					TimeOut(BACKOFF); // wait for CTS
				}
			}
		}
		
		if(!bufFull && networkRole == 'I' && BTconnect && !BTreq) {
			BTsend("REQ");
			BTreq = 1; // request is sent
			
		}
	}//end of while(1)
}//end of main


