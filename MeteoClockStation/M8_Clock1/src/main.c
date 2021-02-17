#include "main.h"
//===============================================================================
int main(void){  
	_delay_ms(500);
    init();
	ADCSRA |= (1 << ADSC);	//запуск измерения АЦП
	
    while(1){
		if(BitIsClr(ADCSRA, ADSC)){	//если бит сброшен - преобразование закончено
			CalculateLight();
			ADCSRA |= (1 << ADSC);	//запуск следующего измерения
		}
		if(BitIsSet(FLAGS1,FL1_BLC1000)){	//тик ~1сек
		//Отправляем пакет в эфир
			nrf24_send(tx_nrf);
			while(nrf24_isSending());
			nrf24_powerUpRx();
			
			ClrBit(FLAGS1,FL1_BLC1000);
			ClrBit(PORTD,PinD_LED);
		}
		if(statusRxGPS == 2){	//Принято NMEA сообщение от модуля GPS
			AnalizRxGPS();
		}
		if(BitIsClr(PIND,PinD_BTN)){	//
			// uint8_t tmp;
			
			// tmp = nrf24_getStatus();
			
			// if(tmp == 0x0E){
				// SetBit(PORTD,PinD_LED);
				// _delay_ms(1000);
				// ClrBit(PORTD,PinD_LED);
			// }
			// SetBit(PORTD,PinD_LED);
			// tx_nrf[3] = 'M';
			// tx_nrf[4] = 'E';
			// tx_nrf[5] = 'T';
			// tx_nrf[6] = 'E';
			// tx_nrf[7] = 'O';

			// nrf24_send(tx_nrf);
			// while(nrf24_isSending());
			// nrf24_powerUpRx();
			
			// ClrBit(PORTD,PinD_LED);
			_delay_ms(1000);
			
		}
	// InvBit(PORTD,PinD_LED);
	// ClrBit(PORTD,PinD_LED);
	// _delay_ms(250);
	// SetBit(PORTD,PinD_LED);
	// _delay_ms(250);
    }
    return 0;
}
//===============================================================================
void init(void){
	SetBit(ACSR, ACD);//отключим аналоговый компаратор	
	// порты
// пины индикатора PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,  PC0,PC1,  PD3,PD4,PD5,PD6,PD7
	
	// Port A
	// DDRA |= (1 << PinPWM_SCK | 1 << PinPWM_MOSI);
	// DDRA &= ~(1 << PinDS_DQ);
	// PORTA &= ~(1 << PinDS_DQ | 1 << PinPWM_SCK | 1 << PinPWM_MOSI);
	
	// // Port B
	// DDRB = (1 << PinB_LCD_3B | 1 << PinB_LCD_2B | 1 << PinB_LCD_2F | 1 << PinB_LCD_2A | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D | 1 << PinB_LCD_3D | 1 << PinB_LCD_1EF);
	// PORTB = (1 << PinB_LCD_3B | 1 << PinB_LCD_2B | 1 << PinB_LCD_2F | 1 << PinB_LCD_2A | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D | 1 << PinB_LCD_3D | 1 << PinB_LCD_1EF);
	
	// // Port C
	// DDRC = (1 << PinC_LCD_2E | 1 << PinC_LCD_2C);
	// PORTC = (1 << PinC_KN_MMM | 1 << PinC_KN_PLS | 1 << PinC_KN_MIN | 1 << PinC_LCD_2E | 1 << PinC_LCD_2C);
	
	// Port D
	DDRD = (1 << PinD_LED);	// | 1 << PinD_LCD_3F
	PORTD = (1 << PinD_BTN);
	
    //1(16бит)таймер - тик ~1сек
    TIMSK |= (1 << OCIE1A);   // разрешить прерывание по совпадению А 1(16разр) счетчика
    
	TCNT1H = 0;			// сброс счетных регистров
	TCNT1L = 0;
	TCCR1A = 0;
	//(8MGz)0.125мкс * 256 = 32мкс; 1с / 32мкс = 31250 => 0x7A12
	TCCR1B = (1 << CS12);	//*256
	OCR1AH = ((0x7A12 & 0xFF00) >> 8);
	OCR1AL = (0x7A12 & 0x00FF);
	
	// //2(8бит)таймер - тик ~8мс
	// TIMSK |= (1 << TOIE2);   // разрешить прерывание по переполнению 2(8разр) счетчика
	// // запускать будем потом: TCNT2 = 0; TCCR2 = (1 << CS22);   // (8М)0,125мкс*256*64 = ~2мс (переполнение счетчика)
	
	//настраиваем АЦП
	ADMUX = (1 << REFS0 | 0 << REFS1 | 1 << MUX0 | 1 << MUX1 | 1 << MUX2);	//ADC7 и опора AVCC с конденсатором на AREF
	ADCSRA = (1 << ADEN | 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0);	//ADC включён, запуск преобразования, прескалер 128 (1МГц / 128 = ~8кГц)
	 // | 1 << ADSC  1 << ADIE |прерывание,

	// Настройка UART на 4800/9600 bps(бод)
	UBRRH = 0;
	UBRRL = 103;	//UBRR = 8000000/(16 * 4800) - 1 = 103,16 //при U2X = 0
	// UBRRL = 51;	//UBRR = 8000000/(16 * 9600) - 1 = 51,16 //при U2X = 0
	UCSRB = (1 << RXEN)|(1 << RXCIE);	//|(1 << TXCIE) вкл приемник(RXEN=1),вкл передатчик(TXEN=1),вкл прерывание по приему(RXCIE=1),вкл прерывание по передаче(TXCIE=1)
	// Для доступа к регистру UCSRC необходимо выставить бит URSEL(кроме ATTiny2313, там его нет)!!!
	UCSRC = (1 << URSEL)|(3 << UCSZ0);	// асинхронный режим(UMSEL=0), 8 бит(UCSZ2-0=011), 1 стоп-бит(USBS=0),без контрол¤ четности(UPM1-0=00)
	// (1 << UPM1)| четный(UPM1-0=10)		

	nrf24_init();
   _delay_ms(2000);
   nrf24_config(10, SIZE_RX_ARRAY); //10й канал, кол-во байт посылка 
   nrf24_tx_address(nrf_address);  // Set the TX address
   nrf24_rx_address(nrf_address);  // Set the RX address
   nrf24_powerUpRx();

	sei();
}
void AnalizRxGPS(void){
/** Пример сообщения 
	$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A \r\n
		где:
			- $ (24) стартовый символ сообщения
			- GPRMC –NMEA заголовок
			- 123419 –UTC время, 12:34:19
			- А –статус (А-активный, V-игнорировать)
			- 4807.038,N –Широта, 48 градусов 07.038 минут северной широты
			- 01131.000,Е –Долгота, 11 градусов31.000 минута восточной долготы
			- 022.4 –Скорость, в узлах
			- 084.4 –Направление движения, в градусах
			- 230394 –Дата, 23 марта 1994 года
			- 003.1,W –Магнитные вариации
			- * (2A) разделитель
			- 6A контрольная сумма
			- преревод строки (0D 0A)
			
	$GPRMC,010042.033,V,,,,,,,160406,,*2D
*/
	
	uint8_t i,c,cs;
	while(1){
		if(rx_gps[0] != '$') break;
		if(rx_gps[1] != 'G') break;
		if(rx_gps[2] != 'P') break;
		if(rx_gps[3] != 'R') break;
		if(rx_gps[4] != 'M') break;
		if(rx_gps[5] != 'C') break;
		//если здесь значит сообщение $GPRMC - то что нам нужно
		//посчитаем контрольную сумму
		for(i = 1, c = cntRxGPS - 5, cs = 0; i < c; i++) cs ^= rx_gps[i];
		
		i = rx_gps[cntRxGPS - 4] - 0x30;	//старший разряд
		if(i > 9) i -= 7;
		c = rx_gps[cntRxGPS - 3] - 0x30;	//младший разряд
		if(c > 9) c -= 7;
		
		i = (i << 4) | c;
		if(cs != i) break;	//контрольная сумма не сошлась
// rx_gps[94] = i;
// rx_gps[95] = cs;
// nrf24_send((unsigned char*)&rx_gps[0]);
// while(nrf24_isSending());
// _delay_ms(20);
// // nrf24_powerUpRx();
// nrf24_send((unsigned char*)&rx_gps[32]);
// while(nrf24_isSending());
// _delay_ms(20);
// // nrf24_powerUpRx();

// nrf24_send((unsigned char*)&rx_gps[64]);
// while(nrf24_isSending());
// // nrf24_powerUpRx();
		tx_nrf[0] = '$';
		tx_nrf[1] = (rx_gps[7] - 0x30) * 10 + (rx_gps[8] - 0x30);	//часы	(время UTC)
		tx_nrf[2] = (rx_gps[9] - 0x30) * 10 + (rx_gps[10] - 0x30);	//минуты
		tx_nrf[3] = (rx_gps[11] - 0x30) * 10 + (rx_gps[12] - 0x30);	//секунды
		
		//ищем нужную по счету запятую после которой идет дата
		for(i = 12, c = 8; c > 0 && i < cntRxGPS; i++){
			if(rx_gps[i] == ',') c--;
		}			
		tx_nrf[4] = (rx_gps[i] - 0x30) * 10 + (rx_gps[i + 1] - 0x30); i += 2;	//день
		tx_nrf[5] = (rx_gps[i] - 0x30) * 10 + (rx_gps[i + 1] - 0x30); i += 2;	//месяц
		tx_nrf[6] = (rx_gps[i] - 0x30) * 10 + (rx_gps[i + 1] - 0x30); i += 2;	//год
		
		//Валидность принятых данных о времени и даты
		//ищем нужную по счету запятую после которой идет дата
		for(i = 0, c = 2; c > 0 && i < cntRxGPS; i++){
			if(rx_gps[i] == ',') c--;
		}			
		// if(rx_gps[14] == 'A') tx_nrf[7] = 0x77; else tx_nrf[7] = 0;
		tx_nrf[7] = rx_gps[i];
		
		for(i = 0, cs = 0; i < 15; i++) cs ^= tx_nrf[i];
		tx_nrf[15] = cs;	//Контрольная сумма посылки (8 битное исключающее ИЛИ (^) всех байтов 00-30 включительно)

		break;
	}

		// tx_nrf[0] = rx_gps[0];
		// tx_nrf[1] = rx_gps[1];
		// tx_nrf[2] = rx_gps[2];
		// tx_nrf[3] = rx_gps[3];
		// tx_nrf[4] = rx_gps[4];
		// tx_nrf[5] = rx_gps[5];
		// tx_nrf[6] = rx_gps[6];
		// tx_nrf[29] = rx_gps[cntRxGPS - 5];
		// tx_nrf[30] = rx_gps[cntRxGPS - 4];
		// tx_nrf[31] = rx_gps[cntRxGPS - 3];

	//Буфер проанализирован, освобождаем
	cntRxGPS = 0;	//указатель текущей позиции
	statusRxGPS = 0;	//статус буфера - свободен
}
void CalculateLight(void){
	uint16_t res;
	double dtmp;
	
	res = (ADCL | (ADCH << 8));
	
	if(ADCcntBuff == 0){
		ADCcntBuff = ADC_MAX_CNT_BUFF;
		
		dtmp = ADCbuffer / ADC_MAX_CNT_BUFF;
		ADCvolume = (uint16_t)dtmp;
		ADCbuffer = 0;
		
		tx_nrf[8] = ((ADCvolume & 0xFF00) >> 8);
		tx_nrf[9] = (ADCvolume & 0x00FF);
	}else{
		ADCcntBuff--;
		ADCbuffer += res;
	}
}
//===============================================================================
ISR(TIMER1_COMPA_vect){	//~1сек
	TCNT1H = 0;			// сброс счетных регистров
	TCNT1L = 0;
	SetBit(FLAGS1, FL1_BLC1000);
}
ISR(USART_RXC_vect){	//
	uint8_t rec = UDR;
	
	if(rec == '$' && statusRxGPS != 0) SetBit(PORTD,PinD_LED);
	switch(statusRxGPS){
		case 0:{
			if(rec == '$'){
				rx_gps[0] = '$';
				cntRxGPS = 1;
				statusRxGPS = 1;
			}
		}break;
		case 1:{
			if(cntRxGPS < SIZE_RX_GPS_BUFFER){
				rx_gps[cntRxGPS] = rec;
				cntRxGPS++;
				if(rec == 0x0A && rx_gps[cntRxGPS - 2] == 0x0D) statusRxGPS = 2;
			}else SetBit(PORTD,PinD_LED);
		}break;
		case 2:{	//если здесь значит буфер обработать не успели
			SetBit(PORTD,PinD_LED);
		}break;
		default: SetBit(PORTD,PinD_LED);
	}
}
//===============================================================================
//===============================================================================



// ISR(TIMER2_OVF_vect){	//~2мс
	// TCCR2 = 0;	//стоп таймер 2(8бит)
	// SetBit(FLAGS1, FL1_BLC2);
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
	// // if(BitIsSet(FLAGS2, FL2_ADC_OK)){	//если напряжение измерено работаем с реле по уставкам 
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
			

				




