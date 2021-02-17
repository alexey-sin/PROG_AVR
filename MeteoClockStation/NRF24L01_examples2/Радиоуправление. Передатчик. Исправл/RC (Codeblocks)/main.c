/////////////////////////////////////////////////////////////////////
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "LPH8731-3C.h"
#include "Graph.h"
#include "bits_macros.h"
#include "nrf24.h"


/////////////////////////////////////////////////////////////////////
const unsigned char mux[][3] = {  //Применяется для переключения входа АЦП
   {0,0,0},{0,0,1},{0,1,0},
   {0,1,1},{1,0,0},{1,0,1},
   {1,1,0},{1,1,1}
};

unsigned char control[4];  //Массив, содержащий значения, соответствующие положению ручек переменных резисторов
unsigned char rx_array[8];  //Массив, в который записываются данные для передачи
unsigned char tx_array[8];  //Аналогично предыдущему, но для приема
unsigned char rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};  //Адрес устройства, прием
unsigned char tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};  //Аналогчино предыдущему, передача
unsigned int button_push[16] = {1021, 339, 509, 256, 696, 294, 413, 229, 837, 317, 460, 242, 606, 276, 380, 217};  //Численное значение АЦП для различных комбинаций кнопок


volatile unsigned char counter_rx, action;  //Отвечают за обновление данных на ЖК
volatile unsigned char water, water_old, water_set;  //"Водные" переменные. В первую записывается текущее состояние (0 - 1), вторая отвечает за определение изменения. Третья - счетчик.
volatile unsigned char button;  //Используется для хранения текущего состояния кнопок
volatile unsigned int battery_mod, battery_ctrl, signal_level;  //Ответственные за отображение уровня батарей и сигнала
volatile int ds18temp;  //Температура
char string[6];  //Нужна для вывода температуры на ЖК

/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////


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

void get_control(void)
{
   unsigned char a, b, c[3];
   for (a = 0;a < 4;a++)
   {
      for (b = 0;b < 3;b++) c[b] = adc_read(a, 1);
      control[a] = (c[0] + c[1] + c[2])/3;
   }
}

void get_button(void)
{
   volatile unsigned char a, i;
   volatile unsigned int c, e[3];

   for (i = 0;i < 3;i++)
   {
      e[i] = adc_read(4, 0);
   }
   c = (e[0] + e[1] + e[2])/3;
   for (a = 0;a < 16;a++)
   {
      if (c > 1000) button = 0;
      else if ((c > (button_push[a] - 10))&&(c < (button_push[a] + 10))) button = a;
   }
}

unsigned char get_battery_control(void)
{
   volatile unsigned char a;
   battery_ctrl = adc_read(7, 0);
   //
   if (battery_ctrl < 204) a = 0;
   else if (battery_ctrl < 223) a = 1;
   else if (battery_ctrl < 235) a = 2;
   else a = 3;
   //
   return a;
}

unsigned char get_battery_model(void)
{
   return rx_array[0];
}

void actions_switch(void)
{

      switch(action)
      {
	 case 1:
	    battery_model(get_battery_model());
	    //
	    action = 2;
	    break;
	 case 3:
	    battery_control(get_battery_control());
	    //
	    action = 4;
	    break;
	 case 5:
	    if (signal_level < 4) signal(2);
	    else if (signal_level < 10) signal(1);
	    else if (signal_level > 10) signal(0);
	    //
	    action = 6;
	    break;
	 case 7:
	    ds18temp = rx_array[3];
	    ds18temp = ds18temp << 8;
	    ds18temp |= rx_array[2];
	    ds18temp = ds18temp >> 4;
	    itoa(ds18temp, string, 10);
	    LCD_FillRect(38, 68, 30, 10, WHITE);
	    LCD_Puts_Shadow (string, 38, 68, RED, 2, 1, 0);
	    //
	    action = 0;
	    break;
      }
}

void init(void)
{
   PORTD = 0x00;
   DDRD = 0xFF;

   ADMUX = 0b00000000;
   ADCSRA = 0b10000110;

   TCCR0 |= ((1 << CS02)|(0 << CS01)|(1 << CS00));
   TIMSK |= (1 << TOIE0);

   LCD_init();
   graph_init();

   nrf24_init();

   _delay_ms(2000);

   nrf24_config(1, 8); //1 канал, 8 бит посылка

   nrf24_tx_address(tx_address);  // Set the TX address
   nrf24_rx_address(rx_address);  // Set the RX address

   sei();

}
///////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////
int main(void)
{
   init();
   while(1)
   {
      get_control();
      get_button();

      unsigned char i;
      for (i = 0;i < 4;i++) tx_array[i] = control[i];
      tx_array[4] = button;

      nrf24_powerUpTx();

      nrf24_send(tx_array);
      while(nrf24_isSending())
	  ;;

      signal_level = nrf24_retransmissionCount();
      nrf24_powerUpRx();

      _delay_ms(30);

      if (BitIsClear(NRF_PIN, NRF_IRQ))
      {
	 if(nrf24_dataReady()) nrf24_getData(rx_array);
      }

      water = rx_array[4];
      button_show(button, water);

      if (water == 0)
      {
	 if ((water == 0)&&(water_old == 1))
	 {
	    graph_init();
	    water_set = 0;
	    action = 1;
	 }
	 actions_switch();
      }else{
	 if (water_set == 0) graph_warning();
	 water_set = 1;
      }
      //

      water_old = water;
   }
}
///////////////////////////////////////////////////////////////

ISR(TIMER0_OVF_vect)
{
   if (counter_rx == 60)
   {
      action++;
      counter_rx = 0;
   }
   else counter_rx++;
}
