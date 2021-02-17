#include "main.h"
//===============================================================================
int main(void){  
	_delay_ms(500);
    init();
	
    while(1){
		if(BitIsSet(FLAGS, FL_INT_IRQ)){
			if(nrf24_dataReady()){
				SetBit(PORTD,6);
				nrf24_getData(rx_array);
				uint8_t i;

				for(i = 0;i < SIZE_RX_ARRAY;i++){
					while(BitIsClr(UCSRA, UDRE));
					UDR = rx_array[i];
				}
				ClrBit(PORTD,6);
			}
		}
		if(BitIsClr(PIND, 2)){
			uint8_t ch, res, tmp;
			SetBit(PORTD,6);
			
			
			for(ch = 1; ch < 128;ch++){
				nrf24_config(ch, SIZE_RX_ARRAY);
				_delay_ms(20);
				nrf24_readRegister(CD, &tmp, 1);
				if(tmp & 0x01) res = 0xFF; else res = 0x55;	//Обнаружена несущая на текущем канале
				while(BitIsClr(UCSRA, UDRE));
				UDR = ch;
				UDR = res;
			}
			nrf24_config(10, SIZE_RX_ARRAY);
			ClrBit(PORTD,6);
			// 
			// 
			
			// }
		}
    }
	// _delay_ms(250);
    return 0;
}
//===============================================================================
void init(void){
	// порты
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
	
	// Port D
	DDRD = 0b01000000;
	PORTD = 0b00001100;
	
    // //1(16бит)таймер - основной тик ~0,25сек
    // TIMSK1 |= (1 << OCIE1A);   // разрешить прерывание по совпадению ј 1(16разр) счетчика
    
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
	// TCCR2B |= (1 << CS21);   // (8М)1мкс*256*8 = ~2мс (переполнение счетчика)
	
	//Настроим прерывание по выводу PB3(PCINT3)
	GIMSK = (1 << PCIE);	//глобальное разрешение прерывания по PCINT0...7
	PCMSK = (1 << PCINT3);	//разрешение прерывания только по PB3(PCINT3)
	
	// Настройка UART на 38400 bps(бод)
	UBRRH = 0;
	UBRRL = 12;	//UBRR = 8000000/(16 * 38400) - 1 = 12,02 //при U2X = 0
	UCSRB = (1 << RXEN)|(1 << RXCIE)|(1 << TXEN); //(1 << RXEN)|(1 << RXCIE)|(1 << TXCIE);	// вкл приемник(RXEN=1),вкл передатчик(TXEN=1),вкл прерывание по приему(RXCIE=1),вкл прерывание по передаче(TXCIE=1)
	// Для доступа к регистру UCSRC необходимо выставить бит URSEL(кроме ATTiny2313, там его нет)!!!
	UCSRC = (3 << UCSZ0);	// асинхронный режим(UMSEL=0), 8 бит(UCSZ2-0=011), 1 стоп-бит(USBS=0),без контрол¤ четности(UPM1-0=00)
	// (1 << UPM1)| четный(UPM1-0=10)		
	
	nrf24_init();
   _delay_ms(2000);
   nrf24_config(10, SIZE_RX_ARRAY); //10й канал, кол-во байт посылка 
   nrf24_tx_address(nrf_address);  // Set the TX address
   nrf24_rx_address(nrf_address);  // Set the RX address
   nrf24_powerUpRx();

	sei();
}
ISR(USART_RX_vect){	// прием байта завершен
	// // TCNT0 = 0;
	// // if(RX_cnt == RX_BUFFER_SIZE){	//перебор
		// // RX_cnt = 0;
		// // TCCR0B = 0;   //стоп счетчика
		// // return;
	// // }
	// // RX_BUFFER[RX_cnt] = UDR;
	// // RX_cnt++;
	// // TCCR0B = (1 << CS02);   //запуск (перезапуск) счетчика
	uint8_t tmp = UDR;
	if(tmp == 'N'){
		tmp = nrf24_getStatus();
		// StartTX_RS232(TRUE);
		while(BitIsClr(UCSRA, UDRE));
		UDR = tmp;
		// StartTX_RS232(FALSE);
	}
	if(tmp == 0x01){
		nrf24_readRegister(0x01, &tmp, 1);
		// tmp = nrf24_getStatus();
		// StartTX_RS232(TRUE);
		while(BitIsClr(UCSRA, UDRE));
		UDR = tmp;
		// StartTX_RS232(FALSE);
	}
	
}
ISR(PCINT_B_vect){	// передача байта завершена
		// // SetBit(PORTD,6);
	if(BitIsClr(PINB, 3)) SetBit(FLAGS, FL_INT_IRQ);
}
// ISR(USART_TX_vect){	// передача байта завершена
	// // SetBit(FLAGS1, FL1_TX);
// }
// void StartTX_RS232(uint8_t tx){
	// if(tx){		//переводим USART в режим передачи
		// UCSRB |= (1 << TXEN);
		// UCSRB &= ~(1 << RXEN);
	// }else{		//переводим USART в режим приема
		// while(BitIsClr(UCSRA, UDRE));;	//дождемся передачи последнего байта
		// UCSRB |= (1 << RXEN);
		// UCSRB &= ~(1 << TXEN);
	// }
// }
// void Tick2(void){
	// if(BitIsSet(FLAGS1,FL1_BTN_WAIT)){
		// CountMUL_BTN_WAIT--;
		// if(CountMUL_BTN_WAIT == 0){
			// ClrBit(FLAGS1, FL1_BTN_WAIT);
		// }
	// }
	// switch(STATUS){
		// case 1:{
			// // if(BitIsSet(FLAGS2, FL2_ADC_OK)) LCDvolume = ADCvolume; else LCDvolume = 200;
			// LCDvolume = ADCvolume;
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
	// // if(BitIsSet(FLAGS2, FL2_ADC_OK)){	//если напр¤жение измерено работаем с реле по уставкам 
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
	// // }
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
		// // if(ADCvolume > 100 && ADCvolume < (ADC_MAX_VOLTAGE * 10)) SetBit(FLAGS2, FL2_ADC_OK);
		// // else{
			// // ADCvolume = 200;
			// // ClrBit(FLAGS2, FL2_ADC_OK);
		// // }
	// }else{
		// ADCcntBuff--;
		// ADCbuffer += res;
	// }
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
				// ClrBit(FLAGS2, FL2_RELE_DELAY);
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
						// if(SETVAL_LOW >= DEF_SETVAL_LOW_MAX || SETVAL_LOW >= (SETVAL_HIGH - DEF_SETVAL_MIN_INT)) break;
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
						// if(SETVAL_HIGH <= DEF_SETVAL_HIGH_MIN || SETVAL_HIGH <= (SETVAL_LOW + DEF_SETVAL_MIN_INT)) break;
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
	// //погасим все разр¤ды
	// PORTB |= (1 << PinB_LCD_3B | 1 << PinB_LCD_2B | 1 << PinB_LCD_2F | 1 << PinB_LCD_2A | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D | 1 << PinB_LCD_3D | 1 << PinB_LCD_1EF);
	// PORTC |= (1 << PinC_LCD_2E | 1 << PinC_LCD_2C);
	// PORTD |= (1 << PinD_LCD_3C | 1 << PinD_LCD_3E | 1 << PinD_LCD_3G | 1 << PinD_LCD_3A | 1 << PinD_LCD_3F);
// }
// void OutLCD(void){	//вывод на дисплей числа
// /**
	// LCD индикатор у нас 2,5 разр¤да. 
	// первый разр¤д выводит только 1
	// второй и третий разр¤д - полноценное 7 - сегментное число.
	// »ндикаци¤ подразумевает вывод дробного числа где третий разр¤д это дес¤тки.
	// ѕросто выводим число uint8_t в диапазоне от 0 до 199
	// ≈сли число больше - выводим 1FF.
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
	// num = num % 100;	//оставим остаток от делени¤ на 100
	// if(CntLCDdigit == 1){
		// tmp = num / 10;		//дес¤тки
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
		// num = num % 10;	//оставим остаток от делени¤ на 10
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
//===============================================================================
// ISR(TIMER2_OVF_vect){	//~2мс
	// SetBit(FLAGS1, FL1_BLC2);
// }
// ISR(TIMER1_COMPA_vect){	//~0,25сек
	// TCNT1H = 0;			// сброс счетных регистров
	// TCNT1L = 0;
	// SetBit(FLAGS1, FL1_BLC250);
// }
// ISR(ADC_vect){	//~125к√ц
	// SetBit(FLAGS2, FL2_ADC);
// }
//===============================================================================
//===============================================================================
			

				




