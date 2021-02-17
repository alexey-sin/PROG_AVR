#include "main.h"
//===============================================================================
int main(void){  
	_delay_ms(500);
    init();
	ADCSRA |= (1 << ADSC);	//������ ��������� ���
	
    while(1){
		if(BitIsClr(ADCSRA, ADSC)){	//���� ��� ������� - �������������� ���������
			CalculateLight();
			ADCSRA |= (1 << ADSC);	//������ ���������� ���������
		}
		if(BitIsSet(FLAGS1,FL1_BLC1000)){	//��� ~1���
		//���������� ����� � ����
			nrf24_send(tx_nrf);
			while(nrf24_isSending());
			nrf24_powerUpRx();
			
			ClrBit(FLAGS1,FL1_BLC1000);
			ClrBit(PORTD,PinD_LED);
		}
		if(statusRxGPS == 2){	//������� NMEA ��������� �� ������ GPS
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
	SetBit(ACSR, ACD);//�������� ���������� ����������	
	// �����
// ���� ���������� PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,  PC0,PC1,  PD3,PD4,PD5,PD6,PD7
	
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
	
    //1(16���)������ - ��� ~1���
    TIMSK |= (1 << OCIE1A);   // ��������� ���������� �� ���������� � 1(16����) ��������
    
	TCNT1H = 0;			// ����� ������� ���������
	TCNT1L = 0;
	TCCR1A = 0;
	//(8MGz)0.125��� * 256 = 32���; 1� / 32��� = 31250 => 0x7A12
	TCCR1B = (1 << CS12);	//*256
	OCR1AH = ((0x7A12 & 0xFF00) >> 8);
	OCR1AL = (0x7A12 & 0x00FF);
	
	// //2(8���)������ - ��� ~8��
	// TIMSK |= (1 << TOIE2);   // ��������� ���������� �� ������������ 2(8����) ��������
	// // ��������� ����� �����: TCNT2 = 0; TCCR2 = (1 << CS22);   // (8�)0,125���*256*64 = ~2�� (������������ ��������)
	
	//����������� ���
	ADMUX = (1 << REFS0 | 0 << REFS1 | 1 << MUX0 | 1 << MUX1 | 1 << MUX2);	//ADC7 � ����� AVCC � ������������� �� AREF
	ADCSRA = (1 << ADEN | 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0);	//ADC �������, ������ ��������������, ��������� 128 (1��� / 128 = ~8���)
	 // | 1 << ADSC  1 << ADIE |����������,

	// ��������� UART �� 4800/9600 bps(���)
	UBRRH = 0;
	UBRRL = 103;	//UBRR = 8000000/(16 * 4800) - 1 = 103,16 //��� U2X = 0
	// UBRRL = 51;	//UBRR = 8000000/(16 * 9600) - 1 = 51,16 //��� U2X = 0
	UCSRB = (1 << RXEN)|(1 << RXCIE);	//|(1 << TXCIE) ��� ��������(RXEN=1),��� ����������(TXEN=1),��� ���������� �� ������(RXCIE=1),��� ���������� �� ��������(TXCIE=1)
	// ��� ������� � �������� UCSRC ���������� ��������� ��� URSEL(����� ATTiny2313, ��� ��� ���)!!!
	UCSRC = (1 << URSEL)|(3 << UCSZ0);	// ����������� �����(UMSEL=0), 8 ���(UCSZ2-0=011), 1 ����-���(USBS=0),��� ������� ��������(UPM1-0=00)
	// (1 << UPM1)| ������(UPM1-0=10)		

	nrf24_init();
   _delay_ms(2000);
   nrf24_config(10, SIZE_RX_ARRAY); //10� �����, ���-�� ���� ������� 
   nrf24_tx_address(nrf_address);  // Set the TX address
   nrf24_rx_address(nrf_address);  // Set the RX address
   nrf24_powerUpRx();

	sei();
}
void AnalizRxGPS(void){
/** ������ ��������� 
	$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A \r\n
		���:
			- $ (24) ��������� ������ ���������
			- GPRMC �NMEA ���������
			- 123419 �UTC �����, 12:34:19
			- � ������� (�-��������, V-������������)
			- 4807.038,N �������, 48 �������� 07.038 ����� �������� ������
			- 01131.000,� ��������, 11 ��������31.000 ������ ��������� �������
			- 022.4 ���������, � �����
			- 084.4 ������������ ��������, � ��������
			- 230394 �����, 23 ����� 1994 ����
			- 003.1,W ���������� ��������
			- * (2A) �����������
			- 6A ����������� �����
			- �������� ������ (0D 0A)
			
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
		//���� ����� ������ ��������� $GPRMC - �� ��� ��� �����
		//��������� ����������� �����
		for(i = 1, c = cntRxGPS - 5, cs = 0; i < c; i++) cs ^= rx_gps[i];
		
		i = rx_gps[cntRxGPS - 4] - 0x30;	//������� ������
		if(i > 9) i -= 7;
		c = rx_gps[cntRxGPS - 3] - 0x30;	//������� ������
		if(c > 9) c -= 7;
		
		i = (i << 4) | c;
		if(cs != i) break;	//����������� ����� �� �������
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
		tx_nrf[1] = (rx_gps[7] - 0x30) * 10 + (rx_gps[8] - 0x30);	//����	(����� UTC)
		tx_nrf[2] = (rx_gps[9] - 0x30) * 10 + (rx_gps[10] - 0x30);	//������
		tx_nrf[3] = (rx_gps[11] - 0x30) * 10 + (rx_gps[12] - 0x30);	//�������
		
		//���� ������ �� ����� ������� ����� ������� ���� ����
		for(i = 12, c = 8; c > 0 && i < cntRxGPS; i++){
			if(rx_gps[i] == ',') c--;
		}			
		tx_nrf[4] = (rx_gps[i] - 0x30) * 10 + (rx_gps[i + 1] - 0x30); i += 2;	//����
		tx_nrf[5] = (rx_gps[i] - 0x30) * 10 + (rx_gps[i + 1] - 0x30); i += 2;	//�����
		tx_nrf[6] = (rx_gps[i] - 0x30) * 10 + (rx_gps[i + 1] - 0x30); i += 2;	//���
		
		//���������� �������� ������ � ������� � ����
		//���� ������ �� ����� ������� ����� ������� ���� ����
		for(i = 0, c = 2; c > 0 && i < cntRxGPS; i++){
			if(rx_gps[i] == ',') c--;
		}			
		// if(rx_gps[14] == 'A') tx_nrf[7] = 0x77; else tx_nrf[7] = 0;
		tx_nrf[7] = rx_gps[i];
		
		for(i = 0, cs = 0; i < 15; i++) cs ^= tx_nrf[i];
		tx_nrf[15] = cs;	//����������� ����� ������� (8 ������ ����������� ��� (^) ���� ������ 00-30 ������������)

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

	//����� ���������������, �����������
	cntRxGPS = 0;	//��������� ������� �������
	statusRxGPS = 0;	//������ ������ - ��������
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
ISR(TIMER1_COMPA_vect){	//~1���
	TCNT1H = 0;			// ����� ������� ���������
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
		case 2:{	//���� ����� ������ ����� ���������� �� ������
			SetBit(PORTD,PinD_LED);
		}break;
		default: SetBit(PORTD,PinD_LED);
	}
}
//===============================================================================
//===============================================================================



// ISR(TIMER2_OVF_vect){	//~2��
	// TCCR2 = 0;	//���� ������ 2(8���)
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
	// // if(BitIsSet(FLAGS2, FL2_ADC_OK)){	//���� ���������� �������� �������� � ���� �� �������� 
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
	// //������� ��� �������
	// PORTB |= (1 << PinB_LCD_3B | 1 << PinB_LCD_2B | 1 << PinB_LCD_2F | 1 << PinB_LCD_2A | 1 << PinB_LCD_2G | 1 << PinB_LCD_2D | 1 << PinB_LCD_3D | 1 << PinB_LCD_1EF);
	// PORTC |= (1 << PinC_LCD_2E | 1 << PinC_LCD_2C);
	// PORTD |= (1 << PinD_LCD_3C | 1 << PinD_LCD_3E | 1 << PinD_LCD_3G | 1 << PinD_LCD_3A | 1 << PinD_LCD_3F);
// }
// void OutLCD(void){	//����� �� ������� �����
// /**
	// LCD ��������� � ��� 2,5 �������. 
	// ������ ������ ������� ������ 1
	// ������ � ������ ������ - ����������� 7 - ���������� �����.
	// ��������� ������������� ����� �������� ����� ��� ������ ������ ��� �������.
	// ������ ������� ����� uint8_t � ��������� �� 0 �� 199
	// ���� ����� ������ - ������� 1FF.
// */
	// uint8_t tmp;
	// uint8_t num = LCDvolume;
	
	// CntLCDdigit++;
	// if(CntLCDdigit == 3) CntLCDdigit = 0;
	
	// ClearLCD();
	// if(num > 199){	//������� 1FF
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
	// num = num % 100;	//������� ������� �� ������� �� 100
	// if(CntLCDdigit == 1){
		// tmp = num / 10;		//�������
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
		// num = num % 10;	//������� ������� �� ������� �� 10
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
			

				




