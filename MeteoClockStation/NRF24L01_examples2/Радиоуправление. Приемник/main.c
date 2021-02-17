#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "servo.h"
#include "nrf24.h"
#include "bits_macros.h"
#include "ds18b20.h"

#define WATER_SENS_PORT PORTD
#define WATER_SENS_DDR DDRD
#define WATER_SENS_PIN PIND
#define WATER_SENS0A_BIT 6
#define WATER_SENS0B_BIT 7

#define LOAD_PORT PORTD
#define LOAD_DDR DDRD
#define LOAD_0 2
#define LOAD_1 3
#define LOAD_2 4
#define LOAD_3 5


unsigned char rx_array[8];
unsigned char tx_array[8];
unsigned char rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
unsigned char tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
unsigned char battery_level[2][3] = {{135, 147, 155}, {67, 73, 77}};
volatile unsigned int battery;

unsigned char tx, counter_rx, action, water, load;
unsigned char a1 = 0, a2 = 0;


const unsigned char mux[][3] = {
   {0,0,0},{0,0,1},{0,1,0},
   {0,1,1},{1,0,0},{1,0,1},
   {1,1,0},{1,1,1}
};

unsigned int adc_read(unsigned char ch, unsigned char type)
{
   ADMUX &= ~((1 << MUX2)|(1 << MUX1)|(1 << MUX0));
   ADMUX |= ((mux[ch][0] << MUX2)|(mux[ch][1] << MUX1)|(mux[ch][2] << MUX0));
   ADCSRA |= (1 << ADSC);
   while (BitIsSet(ADCSRA, ADSC))
      ;;
   if (type == 0)
   {
      return ADCW;
   }
   else
   {
      volatile unsigned char a, b;
      volatile unsigned int c;
      a = ADCL;
      b = ADCH;
      c = ((b << 6) + (a >> 2));
      return c;
   }
}

void water_check(void)
{
   if (BitIsClear(WATER_SENS_PIN, WATER_SENS0A_BIT)) water = 1;
   else water = 0;
   //
   tx_array[4] = water;
}

void load_control(unsigned char data)
{
   if (BitIsSet(data, 0)) LOAD_PORT |= (1 << LOAD_0); else LOAD_PORT &= ~(1 << LOAD_0);
   if (BitIsSet(data, 1)) LOAD_PORT |= (1 << LOAD_1); else LOAD_PORT &= ~(1 << LOAD_1);
   if (BitIsSet(data, 2)) LOAD_PORT |= (1 << LOAD_2); else LOAD_PORT &= ~(1 << LOAD_2);
   if (BitIsSet(data, 3)) LOAD_PORT |= (1 << LOAD_3); else LOAD_PORT &= ~(1 << LOAD_3);	
}

unsigned char get_battery(unsigned char n)
{
   volatile unsigned char a, i;
   battery = adc_read(n + 6, 0);
   //
   for (i = 0;i < 2;i++)
   {
      if (battery < battery_level[i][0]) a = 0;
      else if (battery < battery_level[i][1]) a = 1;
      else if (battery < battery_level[i][2]) a = 2;
      else a = 3;   
   }
   //
   return a;
}

void init(void)
{
   ADMUX = 0b00000110;
   ADCSRA = 0b10000110;
   
   WATER_SENS_PORT |= (1 << WATER_SENS0A_BIT);
   WATER_SENS_PORT &= ~(1 << WATER_SENS0B_BIT);
   WATER_SENS_DDR |= (1 << WATER_SENS0B_BIT);
   WATER_SENS_DDR &= ~(1 << WATER_SENS0A_BIT);
   
   LOAD_PORT &= ~((1 << LOAD_0)|(1 << LOAD_1)|(1 << LOAD_2)|(1 << LOAD_3));
   LOAD_DDR |= ((1 << LOAD_0)|(1 << LOAD_1)|(1 << LOAD_2)|(1 << LOAD_3));
   
   TCCR0 |= ((1 << CS02)|(0 << CS01)|(1 << CS00));
   TIMSK |= (1 << TOIE0);
   
   servo_init();
   
   unsigned char i;
   for (i = 0;i < 4;i++)
   {
      set_angle_byte(i, 127);
   }
   
   nrf24_init();
   _delay_ms(2000);
   nrf24_config(1, 8); //1 канал, 8 бит посылка 
   nrf24_tx_address(tx_address);  // Set the TX address
   nrf24_rx_address(rx_address);  // Set the RX address
   
   nrf24_powerUpRx();
  
   
   sei();
}

int main()
{ 
   init();
   //
   while(1)
   {
      tx_array[0] = get_battery(0);
      water_check();  
      
      if (BitIsClear(NRF_PIN, NRF_IRQ))
      {
	 if(nrf24_dataReady())
	 {
	    nrf24_getData(rx_array);
	    unsigned char i;
	    set_angle_byte(0, rx_array[0]);
	    set_angle_byte(1, rx_array[1]);
	    set_angle_byte(2, rx_array[2]);
	    set_angle_byte(3, rx_array[3]);
	       
	    load_control(rx_array[4]);
      
	    nrf24_powerUpTx();
	    for (i = 0;i < 2;i++)
	    {
	       nrf24_send(tx_array);   
	       while(nrf24_isSending())
		  ;;
	    }
	    nrf24_powerUpRx();
	 }
      }
     
     
      switch(action)
      {
	 case 2:
	    w1_find();
	    w1_sendcmd(0xcc);
	    w1_sendcmd(0x44);
	    action++;
	    break;
	 case 4:
	    w1_find();//снова посылаем Presence и Reset
	    w1_sendcmd(0xcc);
	    w1_sendcmd(0xbe);//передать байты ведущему(у 18b20 в первых двух содержится температура)
	    a1 = w1_receive_byte();//читаем два байта с температурой
	    a2 = w1_receive_byte();
	    tx_array[2] = a1;
	    tx_array[3] = a2;
	    action = 0;
	    break;
      }
      
   }
}

ISR(TIMER0_OVF_vect)
{
   if (counter_rx == 60)
   {
      action++;
      counter_rx = 0;
   }
   else
   {
      counter_rx++;
   }
}

