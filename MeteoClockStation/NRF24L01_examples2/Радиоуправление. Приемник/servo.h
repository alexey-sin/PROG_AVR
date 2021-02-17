#define CYCLE 5000

#define SERVO_DDR DDRC
#define SERVO_PORT PORTC
#define SERVO_PIN0 0
#define SERVO_PIN1 1
#define SERVO_PIN2 2
#define SERVO_PIN3 3
     
volatile unsigned int angle[4] = {1500, 1500, 1500, 1020};
volatile unsigned char clk = 0;

void servo_init(void);
void set_angle(unsigned char channel, int value);
void set_angle_byte(unsigned char channel, unsigned char value);

void port_init(void)
{
   SERVO_PORT &= ~((1 << SERVO_PIN0)|(1 << SERVO_PIN1)|(1 << SERVO_PIN2)|(1 << SERVO_PIN3));
   SERVO_DDR |= ((1 << SERVO_PIN0)|(1 << SERVO_PIN1)|(1 << SERVO_PIN2)|(1 << SERVO_PIN3));
}
     
void timer1_init(void) 
{
   OCR1A = 20000;
   TCCR1A = 0;
   TCCR1B |= (1 << WGM12);
   //
   TCNT1 = 0;
   TCCR1B |= (1 << CS11);
   TIMSK |= (1 << OCIE1A);
}
 
void servo_init(void)
{
   port_init();
   sei();
   timer1_init();
}

void set_angle(unsigned char channel, int value)
{
   angle[channel] = value;
}

void set_angle_byte(unsigned char channel, unsigned char value)
{
   int tmp;
   tmp = value;
   tmp *= 3.9;
   tmp += 1000;
   if (tmp < 1000) tmp = 1000;
   else if (tmp > 2000) tmp = 2000;
   set_angle(channel, tmp); 
}
     
ISR(TIMER1_COMPA_vect) 
{
   switch(clk)
   {
      case 0:
	 SERVO_PORT |= (1 << SERVO_PIN0);
	 OCR1A = angle[0];
	 break;
      case 1: 
	 SERVO_PORT &= ~(1 << SERVO_PIN0);
	 OCR1A = CYCLE - angle[0];
	 break;
      case 2:
	 SERVO_PORT |= (1 << SERVO_PIN1);
	 OCR1A = angle[1];
	 break;
      case 3:
	 SERVO_PORT &= ~(1 << SERVO_PIN1);
	 OCR1A = CYCLE - angle[1];
	 break;
      case 4: 
	 SERVO_PORT |= (1 << SERVO_PIN2);
	 OCR1A = angle[2];
	 break;
      case 5: 
	 SERVO_PORT &= ~(1 << SERVO_PIN2);
	 OCR1A = CYCLE - angle[2];
	 break;
      case 6: 
	 SERVO_PORT |= (1 << SERVO_PIN3);
	 OCR1A = angle[3];
	 break;
      case 7: 
	 SERVO_PORT &= ~(1 << SERVO_PIN3);
	 OCR1A = CYCLE - angle[3];
	 break;
      default:
	 break;
   }
   clk++;
   if (clk == 9) clk = 0;
}