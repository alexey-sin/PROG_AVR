/*
*******************************************************************************************************************
*									Demonstration for Nordic nRF905 module
* Description:  This program demonstrates the API interface between Microchip PIC16F690 at 3.0V & nRF905_MOD.
*				The PCB employed was nRF_CB_Eval_1a.
*				This program simply sets the RF module to receive mode, output characters received
*				from RF to PC via Silabs UART<->USB bridge. Data is visualized by program such as 
*				Docklight (www.docklight.de) or HyperTerminal
*				nRF905 working frequency in 868MHz configured in 	rfConfig905()
* File:			..\nRF905_Rx\nRF905RX_main.c
* Software:		HiTech PICC Lite compiler version 9.60 under MPLAB IDE v7.50
* Programmer: 	John Leung, TechToys Co. (www.TechToys.com.hk)
* Hardware:		nRF_CB_Eval_1a, at Vdd=3.0V powered by USB 5V, regualted by Silabs CP2102 USB<->UART bridge
* Date:			28th April 2007
* Version:		1.0
* Programmer:	John Leung
*
* 												All Rights Reserved
*
*******************************************************************************************************************
*/

#include <stdio.h>
#include <htc.h>
#include "usart.h"
#include "nRF905.h"


/*
********************************************************************************************************************
*											Configuration Bit Section
*
* Configuration bit setting for 16F690 : Internal OSC with no clock out, watchdog disabled, etc.
* Please consult \\HI-TECH Software\PICC\lite\9.60\include\pic16f685.h for meaning of the arguments.
********************************************************************************************************************
*/
__CONFIG(INTIO&WDTDIS&PWRTDIS&MCLREN&UNPROTECT&BORDIS&IESODIS&FCMDIS);

/*
********************************************************************************************************************
*													MAIN
* This program takes care of the Rx side, printing the whole 32 bytes from Rx PayLoad
* Printing is via Silabs CP2102 USB<->UART bridge. 
* Baud rate is 9600bps. Launch Docklight or HyperTerminal to see the package received
********************************************************************************************************************
*/
void main(void)
{
	unsigned char i;

	ANSEL  = 0x00;
	ANSELH = 0x00;	//all digital operation
	SCS = 1;		//internal oscillator is used for system clock and 
					//accept the deafulat clock speed of 4MHz defined by
					//IRCF<2:0>=110
	INTCON=0;		// purpose of disabling the interrupts.
	rfInitIO();
	rfConfig905();
	rfSetRxMode();
	init_comms();	// set up the USART - settings defined in usart.h

	printf("\rnRF905 Receiver Demo:\n");

	for(;;)
	{
		if(DR==1) {
			rfRxPacket();
			printf("received packet in main()\n");
			for(i=0;i<32;i++)
			printf("Character: %c \n", RxBuf[i]);
		}
	}
}
