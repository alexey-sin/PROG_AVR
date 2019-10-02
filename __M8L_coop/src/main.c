#include "main.h"
//===============================================================================
int main(void){  
	// LoadValuesFromEEPROM();
    init();
	_delay_ms(500);
	
    while(1){
		// if(BitIsSet(FLAGS2, FL2_ADC)){
			// CalculateVoltage();
			// ClrBit(FLAGS2, FL2_ADC);
		// }
		// CheckButton();
		// if(BitIsSet(FLAGS1,FL1_BTN)){
			// ExecuteButton();
			// ClrBit(FLAGS1,FL1_BTN);
		// }
		// if(BitIsSet(FLAGS1,FL1_BLC2)){	//тик ~2мс
			// Tick2();
			// ClrBit(FLAGS1,FL1_BLC2);
		// }
		// if(BitIsSet(FLAGS1,FL1_BLC250)){	//тик ~0,25сек
			// Tick250();
			// ClrBit(FLAGS1,FL1_BLC250);
		// }
    }
	// _delay_ms(250);
    return 0;
}
//===============================================================================
void init(void){
	// SetBit(ACSR, ACD);//отключим аналоговый компаратор	
	// // порты
// // пины индикатора PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,  PC0,PC1,  PD3,PD4,PD5,PD6,PD7
	
	// // Port A
	// // DDRA |= (1 << PinPWM_SCK | 1 << PinPWM_MOSI);
	// // DDRA &= ~(1 << PinDS_DQ);
	// // PORTA &= ~(1 << PinDS_DQ | 1 << PinPWM_SCK | 1 << PinPWM_MOSI);
	
	// // // Port B
	// DDRB = (1 << PinB_LCD_3B | 1 << PinB_LCD_2B | 1 << PinB_LCD_2F | 1 << PinB_LCD_2A | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D | 1 << PinB_LCD_3D | 1 << PinB_LCD_1EF);
	// PORTB = (1 << PinB_LCD_3B | 1 << PinB_LCD_2B | 1 << PinB_LCD_2F | 1 << PinB_LCD_2A | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D | 1 << PinB_LCD_3D | 1 << PinB_LCD_1EF);
	
	// // Port C
	// DDRC = (1 << PinC_LCD_2E | 1 << PinC_LCD_2C);
	// PORTC = (1 << PinC_KN_MMM | 1 << PinC_KN_PLS | 1 << PinC_KN_MIN | 1 << PinC_LCD_2E | 1 << PinC_LCD_2C);
	
	// // Port D
	// DDRD = (1 << PinD_RELE | 1 << PinD_LCD_3C | 1 << PinD_LCD_3E | 1 << PinD_LCD_3G | 1 << PinD_LCD_3A | 1 << PinD_LCD_3F);
	// PORTD = (1 << PinD_LCD_3C | 1 << PinD_LCD_3E | 1 << PinD_LCD_3G | 1 << PinD_LCD_3A | 1 << PinD_LCD_3F);
	
    // //1(16бит)таймер - основной тик ~0,25сек
    // TIMSK1 |= (1 << OCIE1A);   // разрешить прерывание по совпадению А 1(16разр) счетчика
    
	// TCNT1H = 0;			// сброс счетных регистров
	// TCNT1L = 0;
	// TCCR1A = 0;
	// TCCR1B = (1 << CS10 | 1 << CS11);	//*64
	// OCR1AH = ((0x0F42 & 0xFF00) >> 8);
	// OCR1AL = (0x0F42 & 0x00FF);
	
	// //2(8бит)таймер - тик ~2мс
	// TIMSK2 |= (1 << TOIE2);   // разрешить прерывание по переполнению 2(8разр) счетчика
	// // запускать будем потом
	// TCNT2 = 0;
	// TCCR2B |= (1 << CS21);   // (1М)1мкс*256*8 = ~2мс (переполнение счетчика)
	
	// //настраиваем АЦП
	// ADMUX = (1 << REFS0 | 1 << REFS1 | 1 << MUX0 | 1 << MUX1 | 1 << MUX2);	//ADC7 и внутренняя опора 1,1в
	// ADCSRA = (1 << ADEN | 1 << ADSC | 1 << ADIE | 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0);	//ADC включён, запуск преобразования, прерывание, прескалер 128 (1МГц / 128 = ~8кГц)
	
	sei();
}
// void Tick2(void){
	// if(BitIsSet(FLAGS1,FL1_BTN_WAIT)){
		// CountMUL_BTN_WAIT--;
		// if(CountMUL_BTN_WAIT == 0){
			// ClrBit(FLAGS1, FL1_BTN_WAIT);
		// }
	// }
	// switch(STATUS){
		// case 1:{
			// if(BitIsSet(FLAGS2, FL2_ADC_OK)) LCDvolume = ADCvolume; else LCDvolume = 200;
			// OutLCD();
			// break;
		// }
		// case 2:{
			// if(BitIsSet(FLAGS1,FL1_LCD_FAIR)) OutLCD(); else ClearLCD();
			// break;
		// }
	// }
// }
// void Tick250(void){
	// if(BitIsSet(FLAGS2, FL2_ADC_OK)){	//если напряжение измерено работаем с реле по уставкам 
		// if(ADCvolume >= SETVAL_HIGH){
			// if(BitIsClr(PIND, PinD_RELE)) SetBit(PORTD, PinD_RELE);
			// ClrBit(FLAGS2, FL2_RELE_DELAY);
		// }
		// if(BitIsSet(FLAGS2, FL2_RELE_DELAY)){
			// RELEcntDelay--;
			// if(RELEcntDelay == 0){
				// ClrBit(PORTD, PinD_RELE);
				// ClrBit(FLAGS2, FL2_RELE_DELAY);
			// }
		// }else{
			// if(ADCvolume <= SETVAL_LOW){
				// RELEcntDelay = SETVAL_DELAY * 4;
				// SetBit(FLAGS2, FL2_RELE_DELAY);
			// }
		// }
	// }
	// switch(STATUS){
		// case 0: break;
		// case 1:{
			// CountMUL_LCD++;
			// if(CountMUL_LCD == MUL_LCD_WAIT_SLEEP){
				// STATUS = 0;
				// CountMUL_LCD = 0;
				// ClearLCD();
			// }
			// break;
		// }
		// case 2:{
			// CountMUL_LCD++;
			// if(CountMUL_LCD == MUL_MENU_EXIT){
				// STATUS = 1;
				// CountMUL_LCD = 0;
				// UpdateValuesFromEEPROM();
			// }
			// if(BitIsSet(FLAGS1,FL1_LCD_FAIR)) ClrBit(FLAGS1,FL1_LCD_FAIR); else SetBit(FLAGS1,FL1_LCD_FAIR);
			// break;
		// }
	// }
// }
// void CalculateVoltage(void){
	// uint16_t res;
	// double dtmp;
	
	// res = (ADCL | (ADCH << 8));
	
	// if(ADCcntBuff == 0){
		// ADCcntBuff = ADC_MAX_CNT_BUFF;
		
		// dtmp = ADCbuffer * ADC_COEFFICIENT;	//17.603 1023 10 64
		// ADCvolume = (uint8_t)dtmp;
		// ADCbuffer = 0;
		// if(ADCvolume > 100 && ADCvolume < (ADC_MAX_VOLTAGE * 10)) SetBit(FLAGS2, FL2_ADC_OK);
		// else{
			// ADCvolume = 200;
			// ClrBit(FLAGS2, FL2_ADC_OK);
		// }
	// }else{
		// ADCcntBuff--;
		// ADCbuffer += res;
	// }
	
	// ADCSRA |= (1 << ADSC);	//запуск следующего измерения
// }
// void CheckButton(void){
	// if(BitIsSet(FLAGS1,FL1_BTN_WAIT)) return;
	
	// if(BitIsSet(FLAGS1,FL1_BTN_MMM)){
		// if(BitIsSet(PINC, PinC_KN_MMM)) ClrBit(FLAGS1,FL1_BTN_MMM);
		// return;
	// }else{
		// if(BitIsClr(PINC, PinC_KN_MMM)){
			// SetBit(FLAGS1, FL1_BTN_MMM);
			// SetBit(FLAGS1, FL1_BTN);
			// return;
		// }
	// }
	// if(BitIsSet(FLAGS1,FL1_BTN_PLS)){
		// if(BitIsSet(PINC, PinC_KN_PLS)) ClrBit(FLAGS1,FL1_BTN_PLS);
	// }else{
		// if(BitIsClr(PINC, PinC_KN_PLS)){
			// SetBit(FLAGS1, FL1_BTN_PLS);
			// SetBit(FLAGS1, FL1_BTN);
		// }
	// }
	// if(BitIsSet(FLAGS1,FL1_BTN_MIN)){
		// if(BitIsSet(PINC, PinC_KN_MIN)) ClrBit(FLAGS1,FL1_BTN_MIN);
	// }else{
		// if(BitIsClr(PINC, PinC_KN_MIN)){
			// SetBit(FLAGS1, FL1_BTN_MIN);
			// SetBit(FLAGS1, FL1_BTN);
		// }
	// }
	
	// CountMUL_BTN_WAIT = MUL_BTN_WAIT;
	// SetBit(FLAGS1, FL1_BTN_WAIT);
// }
// void ExecuteButton(void){
	// CountMUL_LCD = 0;
	
	// if(BitIsSet(FLAGS1,FL1_BTN_MMM)){
		// switch(STATUS){
			// case 0:{
				// STATUS = 1;
				// break;
			// }
			// case 1:{
				// STATUS = 2;
				// MENU = 0;
				// LCDvolume = SETVAL_HIGH;
				// break;
			// }
			// case 2:{
				// switch(MENU){
					// case 0:{
						// MENU = 1;
						// LCDvolume = SETVAL_LOW;
						// break;
					// }
					// case 1:{
						// MENU = 2;
						// LCDvolume = SETVAL_DELAY;
						// break;
					// }
					// case 2:{
						// MENU = 0;
						// LCDvolume = SETVAL_HIGH;
						// break;
					// }
				// }
				// break;
			// }
		// }
		// return;
	// }
	// if(BitIsSet(FLAGS1,FL1_BTN_PLS)){
		// switch(STATUS){
			// case 0:{
				// STATUS = 1;
				// break;
			// }
			// case 1:{
				// PORTD |= (1 << PinD_RELE);
				// break;
			// }
			// case 2:{
				// switch(MENU){
					// case 0:{
						// if(SETVAL_HIGH >= DEF_SETVAL_HIGH_MAX) break;
						// SETVAL_HIGH += DEF_SETVAL_HIGH_STEP;
						// LCDvolume = SETVAL_HIGH;
						// break;
					// }
					// case 1:{
						// if(SETVAL_LOW >= DEF_SETVAL_LOW_MAX || (SETVAL_LOW + DEF_SETVAL_MIN_INT) >= SETVAL_HIGH) break;
						// SETVAL_LOW += DEF_SETVAL_LOW_STEP;
						// LCDvolume = SETVAL_LOW;
						// break;
					// }
					// case 2:{
						// if(SETVAL_DELAY >= DEF_SETVAL_DELAY_MAX) break;
						// SETVAL_DELAY += DEF_SETVAL_DELAY_STEP;
						// LCDvolume = SETVAL_DELAY;
						// break;
					// }
				// }
				// break;
			// }
		// }
		// return;
	// }
	// if(BitIsSet(FLAGS1,FL1_BTN_MIN)){
		// switch(STATUS){
			// case 0:{
				// STATUS = 1;
				// break;
			// }
			// case 1:{
				// PORTD &= ~(1 << PinD_RELE);
				// break;
			// }
			// case 2:{
				// switch(MENU){
					// case 0:{
						// if(SETVAL_HIGH <= DEF_SETVAL_HIGH_MIN || (SETVAL_LOW + DEF_SETVAL_MIN_INT) <= SETVAL_HIGH) break;
						// SETVAL_HIGH -= DEF_SETVAL_HIGH_STEP;
						// LCDvolume = SETVAL_HIGH;
						// break;
					// }
					// case 1:{
						// if(SETVAL_LOW <= DEF_SETVAL_LOW_MIN) break;
						// SETVAL_LOW -= DEF_SETVAL_LOW_STEP;
						// LCDvolume = SETVAL_LOW;
						// break;
					// }
					// case 2:{
						// if(SETVAL_DELAY <= DEF_SETVAL_DELAY_MIN) break;
						// SETVAL_DELAY -= DEF_SETVAL_DELAY_STEP;
						// LCDvolume = SETVAL_DELAY;
						// break;
					// }
				// }
				// break;
			// }
		// }
		// return;
	// }
// }
// void ClearLCD(void){
	// //погасим все разряды
	// PORTB |= (1 << PinB_LCD_3B | 1 << PinB_LCD_2B | 1 << PinB_LCD_2F | 1 << PinB_LCD_2A | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D | 1 << PinB_LCD_3D | 1 << PinB_LCD_1EF);
	// PORTC |= (1 << PinC_LCD_2E | 1 << PinC_LCD_2C);
	// PORTD |= (1 << PinD_LCD_3C | 1 << PinD_LCD_3E | 1 << PinD_LCD_3G | 1 << PinD_LCD_3A | 1 << PinD_LCD_3F);
// }
// void OutLCD(void){	//вывод на дисплей числа
// /**
	// LCD индикатор у нас 2,5 разряда. 
	// первый разряд выводит только 1
	// второй и третий разряд - полноценное 7 - сегментное число.
	// Индикация подразумевает вывод дробного числа где третий разряд это десятки.
	// Просто выводим число uint8_t в диапазоне от 0 до 199
	// Если число больше - выводим 1FF.
// */
	// uint8_t tmp;
	// uint8_t num = LCDvolume;
	
	// CntLCDdigit++;
	// if(CntLCDdigit == 3) CntLCDdigit = 0;
	
	// ClearLCD();
	// if(num > 199){	//выводим 1FF
		// switch(CntLCDdigit){
			// case 0:{
				// PORTB &= ~(1 << PinB_LCD_1EF);
				// break;
			// }
			// case 1:{
				// PORTB &= ~(1 << PinB_LCD_2F | 1 << PinB_LCD_2A | 1 << PinB_LCD_2G);
				// PORTC &= ~(1 << PinC_LCD_2E);
				// break;
			// }
			// case 2:{
				// PORTD &= ~(1 << PinD_LCD_3E | 1 << PinD_LCD_3G | 1 << PinD_LCD_3A | 1 << PinD_LCD_3F);
				// break;
			// }
		// }
		// return;
	// }
	// if(CntLCDdigit == 0) if(num >= 100) PORTB &= ~(1 << PinB_LCD_1EF);
	// num = num % 100;	//оставим остаток от деления на 100
	// if(CntLCDdigit == 1){
		// tmp = num / 10;		//десятки
		// switch(tmp){
			// case 0:{
				// PORTB &= ~(1 << PinB_LCD_2A | 1 << PinB_LCD_2B | 1 << PinB_LCD_2D | 1 << PinB_LCD_2F);
				// PORTC &= ~(1 << PinC_LCD_2C | 1 << PinC_LCD_2E);
				// break;
			// }
			// case 1:{
				// PORTB &= ~(1 << PinB_LCD_2B);
				// PORTC &= ~(1 << PinC_LCD_2C);
				// break;
			// }
			// case 2:{
				// PORTB &= ~(1 << PinB_LCD_2A | 1 << PinB_LCD_2B | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D);
				// PORTC &= ~(1 << PinC_LCD_2E);
				// break;
			// }
			// case 3:{
				// PORTB &= ~(1 << PinB_LCD_2A | 1 << PinB_LCD_2B | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D);
				// PORTC &= ~(1 << PinC_LCD_2C);
				// break;
			// }
			// case 4:{
				// PORTB &= ~(1 << PinB_LCD_2B | 1 << PinB_LCD_2G | 1 << PinB_LCD_2F);
				// PORTC &= ~(1 << PinC_LCD_2C);
				// break;
			// }
			// case 5:{
				// PORTB &= ~(1 << PinB_LCD_2A | 1 << PinB_LCD_2F | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D);
				// PORTC &= ~(1 << PinC_LCD_2C);
				// break;
			// }
			// case 6:{
				// PORTB &= ~(1 << PinB_LCD_2A | 1 << PinB_LCD_2D | 1 << PinB_LCD_2F | 1 << PinB_LCD_2G);
				// PORTC &= ~(1 << PinC_LCD_2C | 1 << PinC_LCD_2E);
				// break;
			// }
			// case 7:{
				// PORTB &= ~(1 << PinB_LCD_2A | 1 << PinB_LCD_2B);
				// PORTC &= ~(1 << PinC_LCD_2C);
				// break;
			// }
			// case 8:{
				// PORTB &= ~(1 << PinB_LCD_2A | 1 << PinB_LCD_2B | 1 << PinB_LCD_2D | 1 << PinB_LCD_2F | 1 << PinB_LCD_2G);
				// PORTC &= ~(1 << PinC_LCD_2C | 1 << PinC_LCD_2E);
				// break;
			// }
			// case 9:{
				// PORTB &= ~(1 << PinB_LCD_2A | 1 << PinB_LCD_2B | 1 << PinB_LCD_2D | 1 << PinB_LCD_2F | 1 << PinB_LCD_2G);
				// PORTC &= ~(1 << PinC_LCD_2C);
				// break;
			// }
			// default: break;
		// }
	// }
	// if(CntLCDdigit == 2){
		// num = num % 10;	//оставим остаток от деления на 10
		// switch(num){
			// case 0:{
				// PORTD &= ~(1 << PinD_LCD_3A | 1 << PinD_LCD_3C | 1 << PinD_LCD_3E | 1 << PinD_LCD_3F);
				// PORTB &= ~(1 << PinB_LCD_3B | 1 << PinB_LCD_3D);
				// break;
			// }
			// case 1:{
				// PORTD &= ~(1 << PinD_LCD_3C);
				// PORTB &= ~(1 << PinB_LCD_3B);
				// break;
			// }
			// case 2:{
				// PORTD &= ~(1 << PinD_LCD_3A | 1 << PinD_LCD_3E | 1 << PinD_LCD_3G);
				// PORTB &= ~(1 << PinB_LCD_3B | 1 << PinB_LCD_3D);
				// break;
			// }
			// case 3:{
				// PORTD &= ~(1 << PinD_LCD_3A | 1 << PinD_LCD_3C | 1 << PinD_LCD_3G);
				// PORTB &= ~(1 << PinB_LCD_3B | 1 << PinB_LCD_3D);
				// break;
			// }
			// case 4:{
				// PORTD &= ~(1 << PinD_LCD_3C | 1 << PinD_LCD_3F | 1 << PinD_LCD_3G);
				// PORTB &= ~(1 << PinB_LCD_3B);
				// break;
			// }
			// case 5:{
				// PORTD &= ~(1 << PinD_LCD_3A | 1 << PinD_LCD_3C | 1 << PinD_LCD_3F | 1 << PinD_LCD_3G);
				// PORTB &= ~(1 << PinB_LCD_3D);
				// break;
			// }
			// case 6:{
				// PORTD &= ~(1 << PinD_LCD_3A | 1 << PinD_LCD_3C | 1 << PinD_LCD_3E | 1 << PinD_LCD_3F | 1 << PinD_LCD_3G);
				// PORTB &= ~(1 << PinB_LCD_3D);
				// break;
			// }
			// case 7:{
				// PORTD &= ~(1 << PinD_LCD_3A | 1 << PinD_LCD_3C);
				// PORTB &= ~(1 << PinB_LCD_3B);
				// break;
			// }
			// case 8:{
				// PORTD &= ~(1 << PinD_LCD_3A | 1 << PinD_LCD_3C | 1 << PinD_LCD_3E | 1 << PinD_LCD_3F | 1 << PinD_LCD_3G);
				// PORTB &= ~(1 << PinB_LCD_3B | 1 << PinB_LCD_3D);
				// break;
			// }
			// case 9:{
				// PORTD &= ~(1 << PinD_LCD_3A | 1 << PinD_LCD_3C | 1 << PinD_LCD_3F | 1 << PinD_LCD_3G);
				// PORTB &= ~(1 << PinB_LCD_3B | 1 << PinB_LCD_3D);
				// break;
			// }
			// default: break;
		// }
	// }
// }
// void LoadValuesFromEEPROM(void){
	// SETVAL_HIGH = eeprom_read_byte(&EE_SETVAL_HIGH);
	// SETVAL_LOW = eeprom_read_byte(&EE_SETVAL_LOW);
	// SETVAL_DELAY = eeprom_read_byte(&EE_SETVAL_DELAY);
// }
// void UpdateValuesFromEEPROM(void){
	// eeprom_update_byte(&EE_SETVAL_HIGH, SETVAL_HIGH);
	// eeprom_update_byte(&EE_SETVAL_LOW, SETVAL_LOW);
	// eeprom_update_byte(&EE_SETVAL_DELAY, SETVAL_DELAY);		
// }
// //===============================================================================
// ISR(TIMER2_OVF_vect){	//~2мс
	// SetBit(FLAGS1, FL1_BLC2);
// }
// ISR(TIMER1_COMPA_vect){	//~0,25сек
	// TCNT1H = 0;			// сброс счетных регистров
	// TCNT1L = 0;
	// SetBit(FLAGS1, FL1_BLC250);
// }
// ISR(ADC_vect){	//~125кГц
	// SetBit(FLAGS2, FL2_ADC);
// }
//===============================================================================
//===============================================================================
			

				




