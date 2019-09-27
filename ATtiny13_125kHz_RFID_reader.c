/*
		125kHz RFID reader. 

		Target: ATtiny13 at 9.6MHz internal RC oscillator.

		- Input pin :  PB1 (RFID signal).
		- Output pin:  PB0 генерация 125кГц
		- Output pin:  PB2 (Buzzer) Sounds when an RFID tag is been read successfully.
		- Output pin:  PB4 (TxD) Transmits the RFID tag serial number (2400 bps 8n1).
                            This pin is TTL logic (0 or +5V). Do not connect it direct to RS232 port. Use a voltage shifter such as MAX232. 		
		
		
		Frimware v1.00 was written by Vassilis Serasidis on 18 August 2012. 
		http://www.serasidis.gr
		avrsite@yahoo.gr, info@serasidis.gr

		IDE: AVRstudio 6
		
		This code is distributed under GNU GPL V3.
		Commercial use of this project can be done after the permission of the author (Vassilis Serasidis).
		
		
		There is no warranty for this project. Use it with your own responsibility.
*/



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PWM_PIN1  PB3
#define TXD		  PB4
#define TIME1     18
#define TRUE      0
#define FALSE     1 
volatile unsigned char counter1 = 0;
volatile unsigned char receivedBits = 0,startBitCounter = 0; 
unsigned char dataBuffer[8];// = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88}; //8 bytes * 8 bits = 64 bits received data length from RFID tag.
unsigned char parity,colParity;

void hex2Ascii (void);
void delay_time(unsigned long counter);
void sendTxD (unsigned char txd);
void init_PWM (void);

int main (void)
{
	DDRB |= ((1<<PB0) | (1<<PB2) | (1 << PWM_PIN1) |(1<<TXD));
	DDRB &= ~(1<<PB1); 
	//PB0,PB2,PB3,PB4 - как выходы
	//PB1 - как вход (сигнал от ридера)
	
  	init_PWM();

	unsigned char wrongParity,k = 0,m,n,temp = 0,temp2,p,r;
	unsigned char i=0;

	sei();                   //  Enable global interrupts
 
	startBitCounter = 1;
 
 
   for (;;)
   {   		
		
		for(n=0;n<2;n++) //просто цикл на 2 прохода
		{
			while(counter1 == 255); //если counter1 переполнился, ждем пока не придет любое изменение фронта от ридера
			// здесь видимо висим все время пока нет сигналов от ридера
			i=0;

			for(k=0;k<4;k++)  //просто цикл на 4 прохода
			{
				while(i<250)  //ждем пока не появится '1' на PB1 в течении некоторого времени
				{
					if(bit_is_set(PINB,PWM_PIN1))
						i++;
					else
						i=0;
				}
			}
		
			while((bit_is_set(PINB,PWM_PIN1))); //ждем пока на  PB1 не появится  '0'
			// здесь мы после первого импульса, на PB1 '0'.

			parity = 0;
			for(i=0;i<5;i++) // There is 1 byte (8-bit) for the customer ID (bits 10-13 and bits 15-18)  and 4 bytes (32-bit) for the  RFID tag serial number (bits 20-24, 26-29, 31-34 and 36-39).
			{	             // That is (4 bits + 1 parity bit)= 5 bits. 5 bits * 10 
				dataBuffer[i] = 0;
			
				for(m=0;m<2;m++) // 2 times is needed to complete 1 byte (2 * 4 bits)
				{
				
					for(k=0;k<4;k++) // 4 bits of data for each RFID number.
					{
						dataBuffer[i] <<= 1;
				
						if((bit_is_set(PINB,PWM_PIN1))) //Save the 4-bit part of the 32-bit Tag's serial number.
							dataBuffer[i] |= (1<<0);
						else
							dataBuffer[i] &= ~(1<<0);

						receivedBits++;	
						_delay_us(TIME1); //задержка 18мкс
					}
					parity >>= 1;
					if((bit_is_set(PINB,PWM_PIN1)))
						parity |= (1<<7);           //The fifth bit is the parity of the previous 4 bits. Is been stored to the 'parity' variable.
					_delay_us(TIME1); //задержка 18мкс
				}			
			}
		}



		colParity = 0;
		for(k=0;k<4;k++) // Read the 4 bits of column parity (bits 60-64).
		{
			colParity <<= 1;
				
			if((bit_is_set(PINB,PWM_PIN1))) //Save the 4-bit part of the 32-bit Tag's serial number.
				colParity |= (1<<0);
			else
				colParity &= ~(1<<0);
			_delay_us(TIME1); //задержка 18мкс
		}
		
		cli(); //Clear interrupts and check the parity of the collected 32 bits from the serial number (4 bytes length).
		//TCCR0B &= ~(1<<CS00);
		
		wrongParity  = FALSE; //We give the value of FALSE to the 'wrongParity' variable.
		p = 1;
		r = 4;
		for(m=1;m<5;m++) //Check for parity only the 4 bytes that contain the Tag's serial number.
		{
			for(i=0;i<2;i++)
			{
				temp = 0;
				for(n=(r+0);n<(r+4);n++) //Check the parity of the first 4 bits of the Tag's serial number.
				{						
					if(bit_is_set(dataBuffer[m],n)) 
						temp ^= 1;
				}
				//temp %= 2; //Find of the 4-bit number has odd or even number of 'Ones'.
				temp2 =(parity & p); //Compare the calculated parity with the stored one from the 'parity' variable.
				
				if(temp2 != 0)
					temp2 = 1;
				p <<= 1;	

				//sendTxD(0x30 + temp);
				//sendTxD(0x30 + temp2);
				//sendTxD(' ');

				if(temp != temp2) //If the calculated parity is not the same with the stored one,
				{
					wrongParity = TRUE; //mark 'wrongParity' variable as TRUE because the parity is wrong and
					break;              //terminate the 'for' loop.
				}

				r ^= 0b00000100;
			}

		}		
		
		p = 0b00000001;
		for(m=0;m<4;m++)
		{
			temp = 0;
			for(i=0;i<5;i++)
			{
				if(!(dataBuffer[i] & p))  	
					temp ^= 1;
				p <<= 4;
				if(!(dataBuffer[i] & p))  
					temp ^= 1;	
				p >>= 4;				
			}
			temp2 =(colParity & p); //Compare the calculated parity with the stored one from the 'parity' variable.
			
			if(temp2 != 0)
				temp2 = 1;
			p <<= 1;
			
			if(temp != temp2) //If the calculated parity is not the same with the stored one,
			{
				wrongParity = TRUE; //mark 'wrongParity' variable as TRUE because the parity is wrong and
				break;              //terminate the 'for' loop.
			}
		}				

			if(wrongParity == FALSE) //If the parity calculation gives correct parity then
				hex2Ascii();         //go to send the Tag's serial number as ASCII trough TxD pin.


			for(m=0;m<8;m++)
				dataBuffer[m] = 0;  //Clear data buffer.
			//TCCR0B |= (1<<CS00);
			sei();			

   }
}

//==============================================================================
// Interrupt from INT0 pin.         
//==============================================================================
ISR(PCINT0_vect) //здесь видимо ошибка = должно быть PCINT1_vect и тогда прерывание по входу от ридера на любое изменение уровня
{
	if((counter1 > 90)&&(bit_is_set(PINB,PB1)))
		PORTB |= (1<<PWM_PIN1);
	else
	if((counter1 > 90)&&(bit_is_clear(PINB,PB1)))
		PORTB &= ~(1<<PWM_PIN1);
		

	counter1 = 0;
}

//==============================================================================
//          
//==============================================================================
ISR(TIM0_OVF_vect)
{//здесь каждый такт 125кГц (через каждые 8мкс)
	if(counter1 < 255)	//255 * 8 = 2,04мс
		counter1++;
	else
		PORTB |= (1<<PWM_PIN1);
}


//===============================================================================
// PWM function. Is set to produce the 125kHz square wave.
//===============================================================================
 
void init_PWM (void)
{
	TCCR0A |= (1<<WGM00 | 1<<WGM01 | 1<<COM0A0 | 1<<COM0B0);  // Configure timer 1 for CTC mode
	TCCR0B |= (1<<WGM02 | 1<<CS00 | 1<<FOC0A | 1<<FOC0B);
	//а получается что выбран режим fastPWM - быстрый ШИМ - на выходе формируется пила с 
	//резким передним фронтом и наклонным задним
	TIMSK0 |= (1<<TOIE0);	//включить прерывание по таймеру 0
	PCMSK  |= (1<<PCINT1);	//включить прерывания по PCINT1 (вход от ридера)
	GIMSK  |= (1<<PCIE);	//разрешить прерывания по выводам PCINT5...0

	OCR0A   = 38;           // Set CTC compare value
	OCR0B   = 38;           // Set CTC compare value
	// получается что влючены на генерацию выходного сигнала оба выхода - А и В
	//но при конфигурировании выход OC0B отключен и включен 
} 
//===================================================================
// Convert a Hex number into ASCII.
//===================================================================
void hex2Ascii (void)
{
	uint32_t temp = 0;
	unsigned char i,buffer[12];


	//sendTxD(' '); //send a 'space' character.

	for(i=1;i<5;i++) //Leave the byte 0 (8-bit customer ID) and get the next 32-bit serial number of the Tag.
	{
		temp <<= 8;
		temp |= dataBuffer[i];
	}

	ultoa(temp,buffer,10); //Convert the 32-bit number 'temp' into ASCII string 'buffer'.

	//PORTB |= (1<<BUZZER);
	for(i=0;i<strlen(buffer);i++)
		sendTxD(buffer[i]);

	sendTxD(0x0d);
	_delay_ms(1000);
	//PORTB &= ~(1<<BUZZER);
}

//===================================================================
// Software based Serial Transmittion.
// Transmits one character each time it runs.
// (2400 bps at 9.6MHz internal RC oscillator).
//===================================================================
void sendTxD (unsigned char txd)
{
	unsigned char bitCounter = 0;  

	PORTB &= ~(1 << TXD);
		_delay_us(405);
	for(bitCounter=8; bitCounter>0; bitCounter--)
	{
		if (bit_is_clear(txd, 0))
			PORTB &= ~(1 << TXD);
		else
			PORTB |= (1 << TXD);

		_delay_us(405);
		txd >>= 1;
	}
	PORTB |= (1 << TXD);
	_delay_us(405);
}


