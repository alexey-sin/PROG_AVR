#include <timer.h>
#include <p33FJ256GP710.h>
#include "nrfbits.h"
#include "globals.h"

#define CLOCK_FREQ   22118000
#define TIMER_CONST  (CLOCK_FREQ/4000)

unsigned char TimeFlag;
unsigned short mTimerCounter;
unsigned short BTconnectTime = 0;

void Pause(unsigned int time){
	ConfigIntTimer1(T1_INT_ON & T1_INT_PRIOR_1); //  Enable Timer1 Interrupt and Priority to "1"  

	OpenTimer1(T1_ON & // Start timer1
			   T1_GATE_OFF & // Disable gate pin for timer1
			   T1_IDLE_STOP & // Stop timer in idle mode
			   T1_PS_1_1 & // Presceler 1:1
			   T1_SYNC_EXT_OFF & // Disable sync external source
			   T1_SOURCE_INT, TIMER_CONST);

	mTimerCounter = 0; // Timer value is set to 0
	WriteTimer1(0);	// Clear count value at TMR1 register

	while (mTimerCounter < time); // Wait

	ConfigIntTimer1(T1_INT_OFF); // Finished. Interrupts switch off

	CloseTimer1();
}

int TimeOut(int maxTime){
	// Same settings as Pause()
	ConfigIntTimer1(T1_INT_ON & T1_INT_PRIOR_1);
	OpenTimer1(T1_ON & // Start timer1
			   T1_GATE_OFF & // Disable gate pin for timer1
			   T1_IDLE_STOP & // Stop timer in idle mode
			   T1_PS_1_1 & // Prescaler 1:1
			   T1_SYNC_EXT_OFF & // Disable sync external source
			   T1_SOURCE_INT, TIMER_CONST);

	mTimerCounter = 0; // Timer value is set to 0
	WriteTimer1(0);	// Clear count value at TMR1 register
	int status = 1; // return value 1=timeout, 0=DR

	while(mTimerCounter < maxTime){ // Wait
		if(DR){
			status = 0;
			break;
		}
	}
	
	ConfigIntTimer1(T1_INT_OFF); // Finished. Interrupts switch off
	CloseTimer1();
	return status;
}

int BTtimeOut(int maxTime){
	// Same settings as Pause()
	ConfigIntTimer1(T1_INT_ON & T1_INT_PRIOR_1);
	OpenTimer1(T1_ON & // Start timer1
			   T1_GATE_OFF & // Disable gate pin for timer1
			   T1_IDLE_STOP & // Stop timer in idle mode
			   T1_PS_1_1 & // Prescaler 1:1
			   T1_SYNC_EXT_OFF & // Disable sync external source
			   T1_SOURCE_INT, TIMER_CONST);

	mTimerCounter = 0; // Timer value is set to 0
	WriteTimer1(0);	// Clear count value at TMR1 register
	int status = 1; // return value 1=timeout, 0=DR

	while(mTimerCounter < maxTime){ // Wait
		if(BTack){
			status = 0;
			break;
		}
	}
	
	ConfigIntTimer1(T1_INT_OFF); // Finished. Interrupts switch off
	CloseTimer1();
	return status;
}

void InitTimer2(void){
	ConfigIntTimer2(T2_INT_ON & T2_INT_PRIOR_2);
	OpenTimer2(T2_ON & // Start timer2
			   T2_GATE_OFF & // Disable gate pin for timer1
			   T2_IDLE_STOP & // Stop timer in idle mode
			   T2_PS_1_1 & // Prescaler 1:1
			   //T2_SYNC_EXT_OFF & // Disable sync external source
			   T2_SOURCE_INT, TIMER_CONST);
}


void __attribute__((__interrupt__)) _T1Interrupt(void){
	// This interrupt should be called each millisecond
	mTimerCounter++;
	WriteTimer1(0);
	IFS0bits.T1IF = 0; // Clear interrupt flag 
}

// Interrupt timeout to check if the BT transmission has ended
void __attribute__((__interrupt__)) _T2Interrupt(void){
	BTconnectTime++;
	if(BTconnectTime >= BT_CONNECT_TIMEOUT) {//Connection time out. No more packets.The input mote close the connection.
		CloseTimer2();
		BTreq = 0;
		BTconnect = 0; 
		BTconnectTime = 0;
		USBsend("Bluetooth connection is closed due to timeout\r\n");
		
	}
	
	IFS0bits.T2IF = 0; // Clear interrupt flag 
}

