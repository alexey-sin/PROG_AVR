/*
*******************************************************************************************************************
*									Demonstration for Nordic nRF905 module
* Description:  This program demonstrates the API interface between Microchip PIC16F690 at 3.0V & nRF905_MOD.
*				The PCB employed was nRF_CB_Eval_1a.
*				This program simply sets the RF module to transmit mode, and send 32 bytes in 868MHz!
* File:			..\nRF905_Tx\nRF905TX_main.c
* Software:		HiTech PICC Lite compiler version 9.60 under MPLAB IDE v7.50
* Programmer: 	John Leung, TechToys Co. (www.TechToys.com.hk)
* Hardware:		nRF_CB_Eval_1a, at Vdd=3.0V powered by USB 5V, internal regulator of Silabs CP2102 USB<->UART bridge
* Date:			28th April 2007
* Version:		1.0
* 												All Rights Reserved
*
*******************************************************************************************************************
*/


#include <htc.h>		//header files for all supported chips includes PIC16F690 (..\include\pic16f685.h)
#include "nRF905.h"		//header file for nRF905 driver
#include "delay.h"		//for software delay function

/*
********************************************************************************************************************
*												Configuration Bit Section
*
* Configuration bit setting for 16F690 : Internal OSC with no clock out, watchdog disabled, etc.
* Please consult \\HI-TECH Software\PICC\lite\9.60\include\pic16f685.h for meaning of the arguments.
********************************************************************************************************************
*/
__CONFIG(INTIO&WDTDIS&PWRTDIS&MCLREN&UNPROTECT&BORDIS&IESODIS&FCMDIS);


/*
********************************************************************************************************************
* 														MAIN
* 
********************************************************************************************************************
*/
void main(void)
{
	unsigned char i,j=0;

	ANSEL  	= 0x00;
	ANSELH 	= 0x00;		//all digital operation
	SCS 	= 1;		//internal oscillator is used for system clock and 
						//accept the deafulat clock speed of 4MHz defined by
						//IRCF<2:0>=110
	rfInitIO();
	rfConfig905();

	for(i=0;i<32;i++) TxBuf[i] = 0x30+(j++);	//load data in TxBuf[i]
	rfTxPacket();
	for(;;)
	{
		;
	}
	
}





