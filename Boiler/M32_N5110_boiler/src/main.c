#include "main.h"
//===============================================================================
int main(void){  
	LoadValuesFromEEPROM();
	CalculateCoefficients();
    init();
	LCD_init();
	DS18B20_convertTemp();
	
	_delay_ms(500);
	LCD_clear(1);
	_delay_ms(500);
	LCD_clear(0);
	_delay_ms(500);

	// LCD_gotoXY(0, 0);	// row = 0-5(������); col = 0-83(�������)
	// LCD_OutChar6x8('R');
	
	// _delay_ms(1000);
	
// __asm volatile ("nop");	
	
    while(1){
		CheckButton();
		if(BitIsSet(STATUS,ST_BTN)){
			ExecuteButton();
			ClrBit(STATUS,ST_BTN);
		}
		if(BitIsSet(STATUS,ST_BLC)){
			if(BitIsSet(STATUS,ST_WORK_T12_M12) || BitIsSet(STATUS,ST_WORK_T3_M3)) ClrBit(PortLED, PinLED);
			else SetBit(PortLED, PinLED);		// ���������
			ReadMAX6675(1);
			ReadMAX6675(2);
			DS18B20_getTemp();
			DS18B20_convertTemp();
			Work_T12_M12();
			Work_T3_M3();
			if(BitIsClr(STATUS,ST_MENU)) OutInfoToLCD();
			else{
				CountMUL_MENU_WAIT--;
				if(CountMUL_MENU_WAIT == 0){	//������� �� ����
					ClrBit(STATUS,ST_MENU);
					LCD_clear(0);
					UpdateValuesFromEEPROM();
					CalculateCoefficients();
					if(LED_LIGTH) ClrBit(PortLCD,PinLCD_BL);			// ������� ��������� �������
					else SetBit(PortLCD,PinLCD_BL);			// �������� ��������� �������
					OutInfoToLCD();
				}
			}
			if(BitIsSet(STATUS,ST_WORK_T12_M12) || BitIsSet(STATUS,ST_WORK_T3_M3)) SetBit(PortLED, PinLED);
			else ClrBit(PortLED, PinLED);		// ���������
			ClrBit(STATUS,ST_BLC);
		}
    }
			// _delay_ms(250);
    return 0;
}
//===============================================================================
void Work_T12_M12(void){
	if(BitIsSet(STATUS, ST_M12_START)){	//������ ������������ �1 � �2
		// �������� ��������� ������������
		if(ui8_PWM2_LEVEL == 0){	//�2 ����������
			if(ui8_PWM1_LEVEL == 0) ui8_M12_STATUS = 0;	//�1 � �2 �����������
			else if(ui8_PWM1_LEVEL == 255) ui8_M12_STATUS = 2; //�1 � 100% ��������, �2 ����������
			else ui8_M12_STATUS = 1;	//�1 � �������� �������, �2 ����������
		}else{
			if(ui8_PWM2_LEVEL == 255) ui8_M12_STATUS = 4;	//�1 � 100% ��������, �2 � 100% ��������
			else ui8_M12_STATUS = 3;	//�1 � 100% ��������, �2 � �������� �������
		}
		
		switch(ui8_M12_STATUS){
			case 0:{
				dbl_PWM1_LEVEL = ui8_PWM1_Start_Discret;
				ui8_PWM1_LEVEL = ui8_PWM1_Start_Discret;
				PWM_send(PWM1, ui8_PWM1_LEVEL);
				break;
			}
			case 1:{
				dbl_PWM1_LEVEL += dbl_PWM1_Step_Start_Discret;
				if(dbl_PWM1_LEVEL >= 255.0){
					ui8_PWM1_LEVEL = 255;
					ui16_M12_CNT = 0;
				}else ui8_PWM1_LEVEL = (uint8_t)dbl_PWM1_LEVEL;
				PWM_send(PWM1, ui8_PWM1_LEVEL);
				break;
			}
			case 2:{
				ui16_M12_CNT++;
				if(ui16_M12_CNT >= M12_BETWEEN_START_PERIOD){
					dbl_PWM2_LEVEL = ui8_PWM2_Start_Discret;
					ui8_PWM2_LEVEL = ui8_PWM2_Start_Discret;
					PWM_send(PWM2, ui8_PWM2_LEVEL);
					ui16_M12_CNT = 0;
				}
				break;
			}
			case 3:{
				dbl_PWM2_LEVEL += dbl_PWM2_Step_Discret;
				if(dbl_PWM2_LEVEL >= 255.0){
					ui8_PWM2_LEVEL = 255;
				}else ui8_PWM2_LEVEL = (uint8_t)dbl_PWM2_LEVEL;
				PWM_send(PWM2, ui8_PWM2_LEVEL);
				break;
			}
			case 4:{
				ClrBit(STATUS, ST_M12_START);
				break;
			}
			default: break;
		}
	}
	if(BitIsSet(STATUS, ST_M12_STOP)){	//��������� ������������ �1 � �2
		// �������� ��������� ������������
		if(ui8_PWM2_LEVEL == 0){	//�2 ����������
			if(ui8_PWM1_LEVEL == 0) ui8_M12_STATUS = 3;	//�1 � �2 �����������
			else if(ui8_PWM1_LEVEL == 255) ui8_M12_STATUS = 1; //�1 � 100% ��������, �2 ����������
			else ui8_M12_STATUS = 2;	//�1 � �������� ����������, �2 ����������
		}else ui8_M12_STATUS = 0;	//�1 � 100% ��������, �2 ��������

		switch(ui8_M12_STATUS){
			case 0:{
				dbl_PWM2_LEVEL = 0.0;
				ui8_PWM2_LEVEL = 0;
				PWM_send(PWM2, ui8_PWM2_LEVEL);
				ui16_M12_CNT = 0;
				break;
			}
			case 1:{
				ui16_M12_CNT++;
				if(ui16_M12_CNT >= M12_BETWEEN_STOP_PERIOD){
					ui16_M12_CNT = 0;
					dbl_PWM1_LEVEL -= dbl_PWM1_Step_Stop_Discret;
					ui8_PWM1_LEVEL = (uint8_t)dbl_PWM1_LEVEL;
					if(ui8_PWM1_LEVEL == 255) ui8_PWM1_LEVEL -= 1;
					PWM_send(PWM1, ui8_PWM1_LEVEL);
				}
				break;
			}
			case 2:{
				dbl_PWM1_LEVEL -= dbl_PWM1_Step_Stop_Discret;
				ui8_PWM1_LEVEL = (uint8_t)dbl_PWM1_LEVEL;
				if(ui8_PWM1_LEVEL <= ui8_PWM1_Start_Discret){
					dbl_PWM1_LEVEL = 0.0;
					ui8_PWM1_LEVEL = 0;
				}
				PWM_send(PWM1, ui8_PWM1_LEVEL);
				break;
			}
			case 3:{
				ClrBit(STATUS, ST_M12_STOP);
				break;
			}
			default: break;
		}
	}
	if(STATUS_DS == 1 && STATUS_TC1 == 1 && VALUE_TC1 > (uint16_t)DS_Temp && ((VALUE_TC1 - (uint16_t)DS_Temp) >= (uint16_t)SETVAL_DELTA_TEMP_SHUTDOWN)){
		if(BitIsClr(STATUS, ST_WORK_T12_M12)) SetBit(STATUS, ST_WORK_T12_M12);
			uint8_t yesRoom, yesOven, noRoom, noOven;
			
			if((uint8_t)DS_Temp <= (SETVAL_TEMP_ROOM - SETVAL_DELTA_TEMP_ROOM)) yesRoom = 1; else yesRoom = 0;
			if(VALUE_TC1 <= (SETVAL_TEMP_TOP_OVEN - SETVAL_DELTA_TEMP_TOP_OVEN)) yesOven = 1; else yesOven = 0;
			if((uint8_t)DS_Temp >= SETVAL_TEMP_ROOM) noRoom = 1; else noRoom = 0;
			if(VALUE_TC1 >= SETVAL_TEMP_TOP_OVEN) noOven = 1; else noOven = 0;
			
			//������� ������� ������������ 1 �������(�1 � �2)
			if(yesRoom && yesOven && ui8_PWM1_LEVEL == 0 && BitIsClr(STATUS, ST_M12_START)){
				ClrBit(STATUS, ST_M12_STOP);
				SetBit(STATUS, ST_M12_START);
			}
			//������� ��������� ������������ 1 �������(�1 � �2)
			if((noRoom || noOven) && ui8_PWM1_LEVEL != 0 && BitIsClr(STATUS, ST_M12_STOP)){
				ClrBit(STATUS, ST_M12_START);
				SetBit(STATUS, ST_M12_STOP);
			}
	}else{
		if(BitIsClr(STATUS, ST_M12_STOP) && ui8_PWM1_LEVEL != 0){
			ClrBit(STATUS, ST_M12_START);
			SetBit(STATUS, ST_M12_STOP);
		}
		if(BitIsSet(STATUS, ST_WORK_T12_M12)) ClrBit(STATUS, ST_WORK_T12_M12);
	}
}
void Work_T3_M3(void){
	if(STATUS_TC2 == 1 && VALUE_TC2 >= SETVAL_TEMP_PYROLYS_START){
		uint16_t workTemp;
		double diskret;
		uint8_t level;
		
		if(BitIsClr(STATUS, ST_WORK_T3_M3)) SetBit(STATUS, ST_WORK_T3_M3);
		if(VALUE_TC2 >= SETVAL_TEMP_PYROLYS_FULL) diskret = 255.0;
		else{
			workTemp = VALUE_TC2 - SETVAL_TEMP_PYROLYS_START;
			diskret = workTemp * dbl_PWM3_Step_Discret + ui8_PWM3_Start_Discret;
		}
		if(diskret > 255.0) level = 0xFF; else level = (uint8_t)diskret;
		if(ui8_PWM3_LEVEL != level){
			ui8_PWM3_LEVEL = level;
			PWM_send(PWM3, ui8_PWM3_LEVEL);
		}
	}else{
		if(ui8_PWM3_LEVEL != 0){
			ui8_PWM3_LEVEL = 0;
			PWM_send(PWM3, ui8_PWM3_LEVEL);
		}
		if(BitIsSet(STATUS, ST_WORK_T3_M3)) ClrBit(STATUS, ST_WORK_T3_M3);
	}
}
void CalculateCoefficients(void){	//�������� ������������� � ���������� ������
	double dd;
	
	dd = (255.0 * SETVAL_PWM_FAN1_START) / 100;
	ui8_PWM1_Start_Discret = (uint8_t)dd;
	dbl_PWM1_Step_Start_Discret = (255.0 - ui8_PWM1_Start_Discret) / (SETVAL_M1_START_PERIOD * 60);
	
	dbl_PWM1_Step_Stop_Discret = (255.0 - ui8_PWM1_Start_Discret) / (SETVAL_M1_STOP_PERIOD * 60);

	dd = (255.0 * SETVAL_PWM_FAN2_START) / 100;
	ui8_PWM2_Start_Discret = (uint8_t)dd;
	dbl_PWM2_Step_Discret = (255.0 - ui8_PWM2_Start_Discret) / M2_START_PERIOD;

	dd = (255.0 * SETVAL_PWM_FAN3_START) / 100;
	ui8_PWM3_Start_Discret = (uint8_t)dd;
	dbl_PWM3_Step_Discret = (255.0 - ui8_PWM3_Start_Discret) / (SETVAL_TEMP_PYROLYS_FULL - SETVAL_TEMP_PYROLYS_START);
}
void ExecuteButton(void){
	if(BitIsSet(STATUS,ST_MENU)) CountMUL_MENU_WAIT = MUL_MENU_WAIT;
	
	if(BitIsSet(BUTTON,MMM)){
		//
		if(BitIsClr(STATUS,ST_MENU)){	//������ ��������� ����
			SetBit(STATUS,ST_MENU);
			MENU = 0;
			ClrBit(PortLCD,PinLCD_BL);			// ������� ��������� �������
			CountMUL_MENU_WAIT = MUL_MENU_WAIT;
			OutMenuToLCD();
		}else{
			MENU++;
			if(MENU == 12) MENU = 0;
			OutMenuToLCD();
		}
		ClrBit(BUTTON,MMM);
	}
	if(BitIsSet(BUTTON,PLS)){
		//
		if(BitIsClr(STATUS,ST_MENU)){
			LED_LIGTH = 1;
			ClrBit(PortLCD,PinLCD_BL);			// ������� ��������� �������
		}else{	//��������� ����
			switch(MENU){
				case 0:{
					if(SETVAL_TEMP_ROOM == DEF_SETVAL_TEMP_ROOM_MAX) break;
					SETVAL_TEMP_ROOM += DEF_SETVAL_TEMP_ROOM_STEP;
					break;
				}
				case 1:{
					if(SETVAL_DELTA_TEMP_ROOM == DEF_SETVAL_DELTA_TEMP_ROOM_MAX) break;
					SETVAL_DELTA_TEMP_ROOM += DEF_SETVAL_DELTA_TEMP_ROOM_STEP;
					break;
				}
				case 2:{
					if(SETVAL_TEMP_TOP_OVEN == DEF_SETVAL_TEMP_TOP_OVEN_MAX) break;
					SETVAL_TEMP_TOP_OVEN += DEF_SETVAL_TEMP_TOP_OVEN_STEP;
					break;
				}
				case 3:{
					if(SETVAL_DELTA_TEMP_TOP_OVEN == DEF_SETVAL_DELTA_TEMP_TOP_OVEN_MAX) break;
					SETVAL_DELTA_TEMP_TOP_OVEN += DEF_SETVAL_DELTA_TEMP_TOP_OVEN_STEP;
					break;
				}
				case 4:{
					if(SETVAL_TEMP_PYROLYS_START == DEF_SETVAL_TEMP_PYROLYS_START_MAX) break;
					SETVAL_TEMP_PYROLYS_START += DEF_SETVAL_TEMP_PYROLYS_START_STEP;
					if(SETVAL_TEMP_PYROLYS_FULL < (SETVAL_TEMP_PYROLYS_START + 100)){
						SETVAL_TEMP_PYROLYS_FULL = (SETVAL_TEMP_PYROLYS_START + 100);
					}
					break;
				}
				case 5:{
					if(SETVAL_TEMP_PYROLYS_FULL < (SETVAL_TEMP_PYROLYS_START + 100)){
						SETVAL_TEMP_PYROLYS_FULL = (SETVAL_TEMP_PYROLYS_START + 100);
					}
					if(SETVAL_TEMP_PYROLYS_FULL == DEF_SETVAL_TEMP_PYROLYS_FULL_MAX) break;
					SETVAL_TEMP_PYROLYS_FULL += DEF_SETVAL_TEMP_PYROLYS_FULL_STEP;
					break;
				}
				case 6:{
					if(SETVAL_DELTA_TEMP_SHUTDOWN == DEF_SETVAL_DELTA_TEMP_SHUTDOWN_MAX) break;
					SETVAL_DELTA_TEMP_SHUTDOWN += DEF_SETVAL_DELTA_TEMP_SHUTDOWN_STEP;
					break;
				}
				case 7:{
					if(SETVAL_PWM_FAN1_START == DEF_SETVAL_PWM_FAN1_START_MAX) break;
					SETVAL_PWM_FAN1_START += DEF_SETVAL_PWM_FAN1_START_STEP;
					break;
				}
				case 8:{
					if(SETVAL_PWM_FAN2_START == DEF_SETVAL_PWM_FAN2_START_MAX) break;
					SETVAL_PWM_FAN2_START += DEF_SETVAL_PWM_FAN2_START_STEP;
					break;
				}
				case 9:{
					if(SETVAL_PWM_FAN3_START == DEF_SETVAL_PWM_FAN3_START_MAX) break;
					SETVAL_PWM_FAN3_START += DEF_SETVAL_PWM_FAN3_START_STEP;
					break;
				}
				case 10:{
					if(SETVAL_M1_START_PERIOD == DEF_SETVAL_M1_START_PERIOD_MAX) break;
					SETVAL_M1_START_PERIOD += DEF_SETVAL_M1_START_PERIOD_STEP;
					break;
				}
				case 11:{
					if(SETVAL_M1_STOP_PERIOD == DEF_SETVAL_M1_STOP_PERIOD_MAX) break;
					SETVAL_M1_STOP_PERIOD += DEF_SETVAL_M1_STOP_PERIOD_STEP;
					break;
				}
			}
			OutMenuToLCD();
		}
		ClrBit(BUTTON,PLS);
	}
	if(BitIsSet(BUTTON,MIN)){
		//
		if(BitIsClr(STATUS,ST_MENU)){
			LED_LIGTH = 0;
			SetBit(PortLCD,PinLCD_BL);			// �������� ��������� �������
		}else{	//��������� ����
			switch(MENU){
				case 0:{
					if(SETVAL_TEMP_ROOM == DEF_SETVAL_TEMP_ROOM_MIN) break;
					SETVAL_TEMP_ROOM -= DEF_SETVAL_TEMP_ROOM_STEP;
					break;
				}
				case 1:{
					if(SETVAL_DELTA_TEMP_ROOM == DEF_SETVAL_DELTA_TEMP_ROOM_MIN) break;
					SETVAL_DELTA_TEMP_ROOM -= DEF_SETVAL_DELTA_TEMP_ROOM_STEP;
					break;
				}
				case 2:{
					if(SETVAL_TEMP_TOP_OVEN == DEF_SETVAL_TEMP_TOP_OVEN_MIN) break;
					SETVAL_TEMP_TOP_OVEN -= DEF_SETVAL_TEMP_TOP_OVEN_STEP;
					break;
				}
				case 3:{
					if(SETVAL_DELTA_TEMP_TOP_OVEN == DEF_SETVAL_DELTA_TEMP_TOP_OVEN_MIN) break;
					SETVAL_DELTA_TEMP_TOP_OVEN -= DEF_SETVAL_DELTA_TEMP_TOP_OVEN_STEP;
					break;
				}
				case 4:{
					if(SETVAL_TEMP_PYROLYS_START == DEF_SETVAL_TEMP_PYROLYS_START_MIN) break;
					SETVAL_TEMP_PYROLYS_START -= DEF_SETVAL_TEMP_PYROLYS_START_STEP;
					break;
				}
				case 5:{
					if(SETVAL_TEMP_PYROLYS_FULL <= (SETVAL_TEMP_PYROLYS_START + 100)){
						SETVAL_TEMP_PYROLYS_FULL = (SETVAL_TEMP_PYROLYS_START + 100);
						break;
					}
					SETVAL_TEMP_PYROLYS_FULL -= DEF_SETVAL_TEMP_PYROLYS_FULL_STEP;
					break;
				}
				case 6:{
					if(SETVAL_DELTA_TEMP_SHUTDOWN == DEF_SETVAL_DELTA_TEMP_SHUTDOWN_MIN) break;
					SETVAL_DELTA_TEMP_SHUTDOWN -= DEF_SETVAL_DELTA_TEMP_SHUTDOWN_STEP;
					break;
				}
				case 7:{
					if(SETVAL_PWM_FAN1_START == DEF_SETVAL_PWM_FAN1_START_MIN) break;
					SETVAL_PWM_FAN1_START -= DEF_SETVAL_PWM_FAN1_START_STEP;
					break;
				}
				case 8:{
					if(SETVAL_PWM_FAN2_START == DEF_SETVAL_PWM_FAN2_START_MIN) break;
					SETVAL_PWM_FAN2_START -= DEF_SETVAL_PWM_FAN2_START_STEP;
					break;
				}
				case 9:{
					if(SETVAL_PWM_FAN3_START == DEF_SETVAL_PWM_FAN3_START_MIN) break;
					SETVAL_PWM_FAN3_START -= DEF_SETVAL_PWM_FAN3_START_STEP;
					break;
				}
				case 10:{
					if(SETVAL_M1_START_PERIOD == DEF_SETVAL_M1_START_PERIOD_MIN) break;
					SETVAL_M1_START_PERIOD -= DEF_SETVAL_M1_START_PERIOD_STEP;
					break;
				}
				case 11:{
					if(SETVAL_M1_STOP_PERIOD == DEF_SETVAL_M1_STOP_PERIOD_MIN) break;
					SETVAL_M1_STOP_PERIOD -= DEF_SETVAL_M1_STOP_PERIOD_STEP;
					break;
				}
			}
			OutMenuToLCD();
		}
		ClrBit(BUTTON,MIN);
	}
}
ISR(TIMER1_COMPA_vect){
	TCNT1H = 0;			// ����� ������� ���������
	TCNT1L = 0;
	SetBit(STATUS, ST_BLC);
}
ISR(TIMER2_OVF_vect){
	CountMUL_BTN_WAIT--;
	if(CountMUL_BTN_WAIT == 0){
		TCCR2 = 0;	// ������ 2 ����
		ClrBit(STATUS, ST_BTN_WAIT);
	}
}
void init(void){
	SetBit(ACSR, ACD);//�������� ���������� ����������	
	// �����
	// Port A
	DDRA |= (1 << PinPWM_SCK | 1 << PinPWM_MOSI);
	DDRA &= ~(1 << PinDS_DQ);
	PORTA &= ~(1 << PinDS_DQ | 1 << PinPWM_SCK | 1 << PinPWM_MOSI);
	
	// Port B
	DDRB |= (1 << PinLED);
	PORTB &= ~(1 << PinLED);
	// Port C
	DDRC |= (1 << PinTC_SCL | 1 << PinTC_CE1 | 1 << PinTC_CE2);
	DDRC &= ~(1 << PinTC_SO | 1 << PinBTN_MMM | 1 << PinBTN_PLS | 1 << PinBTN_MIN);
	PORTC |= (1 << PinTC_CE1 | 1 << PinTC_CE2 | 1 << PinBTN_MMM | 1 << PinBTN_PLS | 1 << PinBTN_MIN);
	PORTC &= ~(1 << PinTC_SCL | 1 << PinTC_SO); 
	
	// Port D
	DDRD |= (1 << PinLCD_RST | 1 << PinLCD_CE | 1 << PinLCD_DC | 1 << PinLCD_CLK | 1 << PinLCD_DIN);
	PORTD |= (1 << PinLCD_RST | 1 << PinLCD_CE);
	PORTD &= ~(1 << PinLCD_DC | 1 << PinLCD_CLK | 1 << PinLCD_DIN);
	//��������� �������
	if(LED_LIGTH) PORTD &= ~(1 << PinLCD_BL); else PORTD |= (1 << PinLCD_BL);
	DDRD |= (1 << PinLCD_BL);
	
    //1(16���)������ - �������� ��� ~1���
    SFIOR |= (1 << PSR10 | 1 << PSR2);   // ����� ���������� �������� 1 � 0
    TIMSK |= (1 << OCIE1A);   // ��������� ���������� �� ���������� � 1(16����) ��������
    
	TCNT1H = 0;			// ����� ������� ���������
	TCNT1L = 0;
	TCCR1A = 0;
	TCCR1B = (1 << CS12);// �������� �� 256  1/8000000 = 125�� * 256 = 32���
	//0x7A12 ��� 31250 * 0,000032 = 1,0 ���
	OCR1AH = ((0x7A12 & 0xFF00) >> 8);
	OCR1AL = (0x7A12 & 0x00FF);
	
	//2(8���)������ - ���������� ���������� (������)
	TIMSK |= (1 << TOIE2);   // ��������� ���������� �� ������������ 2(8����) ��������
	// ��������� ����� �����
	// TCNT2 = 0;
	// TCCR2 |= (1 << CS20 | 1 << CS21 | 1 << CS22);   // (8�)0,125���*256*1024 = 32�� (������������ ��������)
	
	sei();
}
void LoadValuesFromEEPROM(void){
	LED_LIGTH = eeprom_read_byte(&EE_LED_LIGTH);
	SETVAL_TEMP_ROOM = eeprom_read_byte(&EE_SETVAL_TEMP_ROOM);
	SETVAL_DELTA_TEMP_ROOM = eeprom_read_byte(&EE_SETVAL_DELTA_TEMP_ROOM);		
	SETVAL_TEMP_TOP_OVEN = eeprom_read_byte(&EE_SETVAL_TEMP_TOP_OVEN);		
	SETVAL_DELTA_TEMP_TOP_OVEN = eeprom_read_byte(&EE_SETVAL_DELTA_TEMP_TOP_OVEN);	
	SETVAL_TEMP_PYROLYS_START = eeprom_read_word(&EE_SETVAL_TEMP_PYROLYS_START);	
	SETVAL_TEMP_PYROLYS_FULL = eeprom_read_word(&EE_SETVAL_TEMP_PYROLYS_FULL);	
	SETVAL_DELTA_TEMP_SHUTDOWN = eeprom_read_byte(&EE_SETVAL_DELTA_TEMP_SHUTDOWN);	
	SETVAL_PWM_FAN1_START = eeprom_read_byte(&EE_SETVAL_PWM_FAN1_START);		
	SETVAL_PWM_FAN2_START = eeprom_read_byte(&EE_SETVAL_PWM_FAN2_START);		
	SETVAL_PWM_FAN3_START = eeprom_read_byte(&EE_SETVAL_PWM_FAN3_START);
	SETVAL_M1_START_PERIOD = eeprom_read_byte(&EE_SETVAL_M1_START_PERIOD);		
	SETVAL_M1_STOP_PERIOD = eeprom_read_byte(&EE_SETVAL_M1_STOP_PERIOD);		
}
void UpdateValuesFromEEPROM(void){
	eeprom_update_byte(&EE_LED_LIGTH, LED_LIGTH);
	eeprom_update_byte(&EE_SETVAL_TEMP_ROOM, SETVAL_TEMP_ROOM);
	eeprom_update_byte(&EE_SETVAL_DELTA_TEMP_ROOM, SETVAL_DELTA_TEMP_ROOM);		
	eeprom_update_byte(&EE_SETVAL_TEMP_TOP_OVEN, SETVAL_TEMP_TOP_OVEN);		
	eeprom_update_byte(&EE_SETVAL_DELTA_TEMP_TOP_OVEN, SETVAL_DELTA_TEMP_TOP_OVEN);	
	eeprom_update_word(&EE_SETVAL_TEMP_PYROLYS_START, SETVAL_TEMP_PYROLYS_START);	
	eeprom_update_word(&EE_SETVAL_TEMP_PYROLYS_FULL, SETVAL_TEMP_PYROLYS_FULL);	
	eeprom_update_byte(&EE_SETVAL_DELTA_TEMP_SHUTDOWN, SETVAL_DELTA_TEMP_SHUTDOWN);	
	eeprom_update_byte(&EE_SETVAL_PWM_FAN1_START, SETVAL_PWM_FAN1_START);		
	eeprom_update_byte(&EE_SETVAL_PWM_FAN2_START, SETVAL_PWM_FAN2_START);		
	eeprom_update_byte(&EE_SETVAL_PWM_FAN3_START, SETVAL_PWM_FAN3_START);		
	eeprom_update_byte(&EE_SETVAL_M1_START_PERIOD, SETVAL_M1_START_PERIOD);		
	eeprom_update_byte(&EE_SETVAL_M1_STOP_PERIOD, SETVAL_M1_STOP_PERIOD);		
}
void LCD_init(void){
	_delay_us(10);
	ClrBit(PortLCD, PinLCD_RST);
	_delay_us(100);
	SetBit(PortLCD, PinLCD_RST);
	_delay_us(10);
	
	LCD_sendByte(0x21, 0);	//0b00100001 => 0x21 ����������� ������� ������, ��� �������,�������������� ���������
	LCD_sendByte(0x13, 0);	//0b00010011 => 0x13 ��������� ���������� �������� Bias ������� n = 4 (1:48)
	LCD_sendByte(0x04, 0);	//0b00000100 => 0x04 ������������� ���������� 00
	LCD_sendByte(0xB8, 0);	//0b10111000 => 0xB8 ��������� Vop
	
	LCD_sendByte(0x20, 0);	//0b00100000 => 0x20 ������� ������� ������, ��� �������,�������������� ���������
	LCD_sendByte(0x0C, 0);	//0b00001100 => 0x0C ���������� ������������ ������� - ���������� �����
	
	LCD_clear(0);
}
void LCD_sendByte(uint8_t byte, uint8_t cmd){	//byte - ���������� ����; cmd = 1 - ������, 0 - �������
	uint8_t i;
	
	if(cmd == 0) ClrBit(PortLCD, PinLCD_DC); else SetBit(PortLCD, PinLCD_DC);
	
	ClrBit(PortLCD, PinLCD_CE);
	
	for(i = 0; i < 8; i++){
		if(BitIsSet(byte, 7)) SetBit(PortLCD, PinLCD_DIN); else ClrBit(PortLCD, PinLCD_DIN);
		SetBit(PortLCD, PinLCD_CLK);
		byte <<= 1;
		ClrBit(PortLCD, PinLCD_CLK);
	}
	SetBit(PortLCD, PinLCD_CE);
}
void LCD_gotoXY(uint8_t row, uint8_t col){	// row = 0-5(������); col = 0-83(�������)
	LCD_sendByte((row + 0x40), 0);
	LCD_sendByte((col + 0x80), 0);
}
void LCD_clear(uint8_t bit){	//���������� ������ bit=0 0x00 ��� bit>0 0xFF
	uint16_t i;
	uint8_t byte = bit? 0xFF : 0x00;
	
	LCD_gotoXY(0, 0);
	for(i = 0; i < 504; i++) LCD_sendByte(byte, 1);
}
void LCD_OutChar6x8(char ch){
	uint8_t i;
	
	for(i = 0; i < 6; i++) LCD_sendByte(pgm_read_byte(&(Font_6x8_Data[(uint8_t)ch][i])), 1);
}
void LCD_OutString6x8(char* str){
	uint8_t i;
	uint8_t len = pgm_read_byte(&(str[0]));
	
	for(i = 1; i <= len; i++) LCD_OutChar6x8(pgm_read_byte(&(str[i])));
}
uint8_t LCD_OutChar12x16(char ch, uint8_t row, uint8_t col){
	uint8_t i;
	
	LCD_gotoXY(row, col);	// row = 0-5(������); col = 0-83(�������)
	for(i = 0; i < 12; i++) LCD_sendByte(pgm_read_byte(&(Font_12x16_Data[(uint8_t)ch][i])), 1);
	LCD_gotoXY((row + 1), col);	// row = 0-5(������); col = 0-83(�������)
	for(i = 12; i < 24; i++) LCD_sendByte(pgm_read_byte(&(Font_12x16_Data[(uint8_t)ch][i])), 1);
	
	return (col + 12);
}
uint8_t LCD_OutString12x16(char* str, uint8_t row, uint8_t col){
	uint8_t i;
	uint8_t len = pgm_read_byte(&(str[0]));
	
	for(i = 1; i <= len; i++){LCD_OutChar12x16(pgm_read_byte(&(str[i])), row, col); col += 12;}
	
	return col;
}
uint8_t LCD_OutNumber12x16(uint8_t val, uint8_t row, uint8_t col){
	uint8_t d, fnam;
	
	fnam = 0;	//������ �������� �����
	d = val / 100;
	if(d != 0){LCD_OutChar12x16((d + 0x30), row, col); col += 12; fnam = 1;}
	val = val % 100;
	
	d = val / 10;
	if(d != 0){LCD_OutChar12x16((d + 0x30), row, col); col += 12;}
	else if(fnam == 1){LCD_OutChar12x16(0x30, row, col); col += 12;}
	d = val % 10;
	LCD_OutChar12x16((d + 0x30), row, col);
	
	return (col + 12);
}
uint8_t LCD_OutBigNumber12x16(uint16_t val, uint8_t row, uint8_t col){
	uint16_t d;
	uint8_t fnam;
	
	fnam = 0;	//������ �������� �����
	d = val / 10000;
	if(d != 0){LCD_OutChar12x16((d + 0x30), row, col); col += 12; fnam = 1;}
	else if(fnam == 1){LCD_OutChar12x16(0x30, row, col); col += 12;}
	val = val % 10000;
	
	d = val / 1000;
	if(d != 0){LCD_OutChar12x16((d + 0x30), row, col); col += 12; fnam = 1;}
	else if(fnam == 1){LCD_OutChar12x16(0x30, row, col); col += 12;}
	val = val % 1000;
	
	d = val / 100;
	if(d != 0){LCD_OutChar12x16((d + 0x30), row, col); col += 12; fnam = 1;}
	else if(fnam == 1){LCD_OutChar12x16(0x30, row, col); col += 12;}
	val = val % 100;
	
	d = val / 10;
	if(d != 0){LCD_OutChar12x16((d + 0x30), row, col); col += 12; fnam = 1;}
	else if(fnam == 1){LCD_OutChar12x16(0x30, row, col); col += 12;}
	d = val % 10;
	
	LCD_OutChar12x16((d + 0x30), row, col);
	
	return (col + 12);
}
void ReadMAX6675(uint8_t dat){	//dat - ����� ������� 1,2; ������ ����������� � ���� ���������� VALUE_TC1(2) � STATUS_TC1(2)
/**
PC7 SCL	������ ���������� ��� ��������
PC4 SO	������ ��� ��������
PC6 CE	1 ������	������ ���������� � ���������
PC5 CE	2 ������	������ ���������� � ���������

������ ������ �� ��� MAX6675:
�������� CE � 0, ������ ����� �� SCL 16 ��� � � ����� SCL = 1 ������ ���� � ������ MISO.
����� ���� ������� ������ � ������� ����� ������
� ����� CE ���������  1, SCL ������ �������� � 0.

0 ��� - 3-� ��������� - �� ��������
1 ��� - ������ 0 - ������� MAX6675
2 ��� 0 ���� ��� ������, 1 ���� ��������� � ������
3-14 ���� (12 ����) �������� �����������, 2 ������� ������� - ������� ����� (�0,25)
	��� ���� = 0 ��. �� �������
	��� ������� - +1023,75
15 ��� - ���� ����������� = ������ 0.
*/
	uint8_t i;
	uint16_t tmp = 0;
	uint8_t status = 0;
	
	if(dat != 1 && dat != 2){
		STATUS_TC1 = 0;
		STATUS_TC2 = 0;
		return;
	}
	if(dat == 1) ClrBit(PortTC, PinTC_CE1); else ClrBit(PortTC, PinTC_CE2);
	
	i = 16;
	while(i--){
		SetBit(PortTC, PinTC_SCL);
		tmp <<= 1;
		if(BitIsSet(PortPinTC, PinTC_SO)) tmp |= 0x0001; else tmp &= 0xFFFE;
		ClrBit(PortTC, PinTC_SCL);
	}
	if(dat == 1) SetBit(PortTC, PinTC_CE1); else SetBit(PortTC, PinTC_CE2);
	
// LCD_clear(0);	
// LCD_gotoXY(4, 0);	// row = 0-5(������); col = 0-83(�������)
	// i = 16;
	// while(i--){
	// if(tmp & (1 << 15)) LCD_OutChar6x8(0x31); else LCD_OutChar6x8(0x30);
	// tmp <<= 1;
// }
	
	if((tmp & (1 << 1 | 1 << 2 | 1 << 15))){
		status = 2;
		tmp = 0;
	}
	if(status == 0){
		tmp >>= 5;	//������� ������ ����� �������� �����������
		if(tmp > (uint16_t)999) status = 3;
		else status = 1;
	}
	if(dat == 1){
		STATUS_TC1 = status;
		VALUE_TC1 = tmp;
	}else{
		STATUS_TC2 = status;
		VALUE_TC2 = tmp;
	}
}
void OutMenuToLCD(void){	//����� ���� � ������� �� �������
	uint8_t i, col;
	
	LCD_clear(0);	//���������� ������ bit=0 0x00 ��� bit>0 0xFF
	LCD_gotoXY(0, 14);	// row = 0-5(������); col = 0-83(�������)
	LCD_OutString6x8((char*)LabelSetting);
	
	LCD_gotoXY(1, 0);	// row = 0-5(������); col = 0-83(�������)
	i = 14;
	while(i--) LCD_OutChar6x8('=');
	
	switch(MENU){
		case 0:{
			LCD_gotoXY(2, 9);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_TEMP);
			LCD_gotoXY(3, 15);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_ROOM);
			
			col = LCD_OutNumber12x16(SETVAL_TEMP_ROOM, 4, 18);
			LCD_OutString12x16((char*)LabelCC, 4, col);
			break;
		}
		case 1:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_DELTA_TEMP);
			LCD_gotoXY(3, 15);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_ROOM);
			
			col = LCD_OutNumber12x16(SETVAL_DELTA_TEMP_ROOM, 4, 21);
			LCD_OutString12x16((char*)LabelCC, 4, col);
			break;
		}
		case 2:{
			LCD_gotoXY(2, 9);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_TEMP);
			LCD_gotoXY(3, 3);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_TOP_OVEN);
			
			col = LCD_OutNumber12x16(SETVAL_TEMP_TOP_OVEN, 4, 21);
			LCD_OutString12x16((char*)LabelCC, 4, col);
			break;
		}
		case 3:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_DELTA_TEMP);
			LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_TOP_OVEN);
			
			col = LCD_OutNumber12x16(SETVAL_DELTA_TEMP_TOP_OVEN, 4, 21);
			LCD_OutString12x16((char*)LabelCC, 4, col);
			break;
		}
		case 4:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_TEMP);
			LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_START_PYROLYS);
			
			col = LCD_OutBigNumber12x16(SETVAL_TEMP_PYROLYS_START, 4, 12);
			LCD_OutString12x16((char*)LabelCC, 4, col);
			break;
		}
		case 5:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_TEMP);
			LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_FULL_PYROLYS);
			
			col = LCD_OutBigNumber12x16(SETVAL_TEMP_PYROLYS_FULL, 4, 12);
			LCD_OutString12x16((char*)LabelCC, 4, col);
			break;
		}
		case 6:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_RAZN_TEMP);
			LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_SHUTDOWN);
			
			col = LCD_OutNumber12x16(SETVAL_DELTA_TEMP_SHUTDOWN, 4, 12);
			LCD_OutString12x16((char*)LabelCC, 4, col);
			break;
		}
		case 7:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_PWM_START);
			LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_FAN);
			LCD_OutChar6x8('1');
			
			col = LCD_OutNumber12x16(SETVAL_PWM_FAN1_START, 4, 21);
			LCD_OutChar12x16('%', 4, col);
			break;
		}
		case 8:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_PWM_START);
			LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_FAN);
			LCD_OutChar6x8('2');
			
			col = LCD_OutNumber12x16(SETVAL_PWM_FAN2_START, 4, 21);
			LCD_OutChar12x16('%', 4, col);
			break;
		}
		case 9:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_PWM_START);
			LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_FAN);
			LCD_OutChar6x8('3');
			
			col = LCD_OutNumber12x16(SETVAL_PWM_FAN3_START, 4, 21);
			LCD_OutChar12x16('%', 4, col);
			break;
		}
		case 10:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_TIME_START);
			LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_FAN);
			LCD_OutChar6x8('1');
			
			col = LCD_OutNumber12x16(SETVAL_M1_START_PERIOD, 4, 21);
			LCD_OutString12x16((char*)Label_MIN, 4, col);
			break;
		}
		case 11:{
			LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_TIME_STOP);
			LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
			LCD_OutString6x8((char*)Label_FAN);
			LCD_OutChar6x8('1');
			
			col = LCD_OutNumber12x16(SETVAL_M1_STOP_PERIOD, 4, 21);
			LCD_OutString12x16((char*)Label_MIN, 4, col);
			break;
		}
	}
	
}
void OutInfoToLCD(void){	//����� ����������� ��������� �� �������
	// uint16_t val;
	
	LCD_clear(0);	//���������� ������ bit=0 0x00 ��� bit>0 0xFF
	
	LCD_gotoXY(0, 0);	// row = 0-5(������); col = 0-83(�������)
	LCD_OutString6x8((char*)LabelT1);
	OutTemperatureDS();

	LCD_gotoXY(1, 0);	// row = 0-5(������); col = 0-83(�������)
	LCD_OutString6x8((char*)LabelT2);
	OutTemperatureTC(STATUS_TC1, VALUE_TC1);
	
	LCD_gotoXY(2, 0);	// row = 0-5(������); col = 0-83(�������)
	LCD_OutString6x8((char*)LabelT3);
	OutTemperatureTC(STATUS_TC2, VALUE_TC2);
	
	LCD_gotoXY(3, 0);	// row = 0-5(������); col = 0-83(�������)
	LCD_OutString6x8((char*)LabelM1);
	OutUI8_6x8(CalculatePercent(ui8_PWM1_LEVEL));
	LCD_OutChar6x8('%');
	
	LCD_gotoXY(4, 0);	// row = 0-5(������); col = 0-83(�������)
	LCD_OutString6x8((char*)LabelM2);
	OutUI8_6x8(CalculatePercent(ui8_PWM2_LEVEL));
	LCD_OutChar6x8('%');

	LCD_gotoXY(5, 0);	// row = 0-5(������); col = 0-83(�������)
	LCD_OutString6x8((char*)LabelM3);
	OutUI8_6x8(CalculatePercent(ui8_PWM3_LEVEL));
	LCD_OutChar6x8('%');
}
uint8_t CalculatePercent(uint8_t diskr){
	double val;
	val = (100.0 * diskr) / 255;
	return (uint8_t)val;
}
void OutTemperatureTC(uint8_t status, uint16_t value){
	uint8_t d, fnam;
	
	switch(status){
		case 0:{
			LCD_OutString6x8((char*)LabelUNC);
			break;
		}
		case 1:{
			fnam = 0;
			d = value / 100;
			if(d != 0){LCD_OutChar6x8(d + 0x30); fnam = 1;}
			value = value % 100;
			
			d = value / 10;
			if(d != 0) LCD_OutChar6x8(d + 0x30);
			else if(fnam == 1) LCD_OutChar6x8(0x30);
			d = value % 10;
			
			LCD_OutChar6x8(d + 0x30);
			
			LCD_OutString6x8((char*)LabelCC);
			break;
		}
		case 2:{
			LCD_OutString6x8((char*)LabelBRK);
			break;
		}
		case 3:{
			LCD_OutString6x8((char*)LabelOVR);
			break;
		}
		default:{
			LCD_OutString6x8((char*)LabelERR);
			break;
		}
	}
}
void OutTemperatureDS(void){
	uint8_t d, fnam;
	uint8_t ival = (uint8_t)DS_Temp;
	double fval = DS_Temp;
	
	if(STATUS_DS == 1){
		fnam = 0;
		if(ival < 0) LCD_OutChar6x8('-');
		
		d = ival / 100;
		if(d != 0){LCD_OutChar6x8(d + 0x30); fnam = 1;}
		ival = ival % 100;
		
		d = ival / 10;
		if(d != 0) LCD_OutChar6x8(d + 0x30);
		else if(fnam == 1) LCD_OutChar6x8(0x30);
		d = ival % 10;
		
		LCD_OutChar6x8(d + 0x30);
		
		LCD_OutChar6x8('.');
		
		ival = (uint8_t)DS_Temp;
		fval = (fval - ival) * 10.0;
		d = (uint8_t)fval;
		LCD_OutChar6x8(d + 0x30);
		
		LCD_OutString6x8((char*)LabelCC);
	}else LCD_OutString6x8((char*)LabelBRK);
}
void OutUI8_6x8(uint8_t value){
	uint8_t d, fnam;
	
	fnam = 0;
	d = value / 100;
	if(d != 0){LCD_OutChar6x8(d + 0x30); fnam = 1;}
	value = value % 100;
	
	d = value / 10;
	if(d != 0) LCD_OutChar6x8(d + 0x30);
	else if(fnam == 1) LCD_OutChar6x8(0x30);
	d = value % 10;
	
	LCD_OutChar6x8(d + 0x30);
}
void CheckButton(void){
	if(BitIsSet(STATUS,ST_BTN_WAIT)) return;

	if(BitIsClr(PortPinBTN, PinBTN_MMM)){
		SetBit(BUTTON, MMM);
		SetBit(STATUS, ST_BTN);
	}
	if(BitIsClr(PortPinBTN, PinBTN_PLS)){
		SetBit(BUTTON, PLS);
		SetBit(STATUS, ST_BTN);
	}
	if(BitIsClr(PortPinBTN, PinBTN_MIN)){
		SetBit(BUTTON, MIN);
		SetBit(STATUS, ST_BTN);
	}
	
	CountMUL_BTN_WAIT = MUL_BTN_WAIT;
	TCNT2 = 0;
	TCCR2 |= (1 << CS20 | 1 << CS21 | 1 << CS22);   // (8�)0,125���*256*1024 = 32�� (������������ ��������)
	SetBit(STATUS, ST_BTN_WAIT);
}
//===============================================================================
/**���������� ��� ������ � �������� ����������� DS18B20
��������� ��������� ������ ������� 12-������ � ��� �������������� ����������� ������� ���� 750��
������ ���������� ���� �� ���� 800��
9-������ ��������������� - 94��, �������� 0,5��

��������� ������ � ��������:
�������� ������� �������������� �����������:
	������������� (�������� ������)
	�������� ������� DS18B20_SKIP_ROM		0xCC ��������� � ������������� �� ���� ���������� ��� �������� ��� ������
	�������� ������� DS18B20_CONVERT_T		0x44 ����� �������������� �����������

������ �����������:
	������������� (�������� ������)
	�������� ������� DS18B20_SKIP_ROM		0xCC ��������� � ������������� �� ���� ���������� ��� �������� ��� ������
	�������� ������� DS18B20_R_SCRATCHPAD	0xBE ������ ����������� ������ (���������)
	������ 
	��������� ����������

*/
uint8_t DS18B20_init(void){	//Out: 0 - OK; 1 - ��� �������
	uint8_t i,d;
	
	ClrBit(PortDS, PinDS_DQ);
	ClrBit(PortDdrDS, PinDS_DQ);
	
	if(BitIsClr(PortPinDS, PinDS_DQ)) return 1; //�������� ������� 1 � �����, ���� ��� - ��� error
	SetBit(PortDdrDS, PinDS_DQ);	//����� ����� � 0
	_delay_us(500);
	ClrBit(PortDdrDS, PinDS_DQ);	//��������� �����
	_delay_us(60);
	
	d = 0;
	i = 240;
	while(i--){
		_delay_us(1);
		if(BitIsClr(PortPinDS, PinDS_DQ)){d = 1; break;}	//PRESENCE �� ������� �������
	}
	if(d == 0) return 1;
	_delay_us(480);
	return 0;
}
uint8_t DS18B20_send(uint8_t sbyte){	//���� ������ ����� 0�FF, �� �� ������ ����� �������� ����
	uint8_t i, ans;
	
	ans = 0;
	i = 8;
	while(i--){
		SetBit(PortDdrDS, PinDS_DQ);	//����� ����� � 0
		_delay_us(1);
		if(sbyte & 0x01) ClrBit(PortDdrDS, PinDS_DQ);	//��������� �����
		sbyte >>= 1;
		_delay_us(10);
		ans >>= 1;
		if(BitIsSet(PortPinDS, PinDS_DQ)) ans |= 0x80;
		_delay_us(90);
		ClrBit(PortDdrDS, PinDS_DQ);	//��������� �����
	}
	return ans;
}
uint8_t DS18B20_read(void){	//��������� 9 ������ � �����, ������������ CRC. Out: 0 - OK, 1 - error CRC
	uint8_t i,rb;
	uint8_t DS_crc8 = CRC8INIT;
	
	for(i = 0; i < 9;i++){
		rb = DS18B20_send(0xFF);
		DS_TempData[i] = rb;
		DS_crc8 = DS_calcCRC8(rb, DS_crc8);
	}
	if(DS_crc8 != CRC8INIT) return 1;
	return 0;
}
uint8_t DS_calcCRC8(uint8_t data, uint8_t crc){	//Out: CRC
// ����� ������ ������� CRC ���������� ��������
// ��� ��������� ������ �������� 8 ���
// ��� ������ ������� 9 ���
// ���� � ���������� crc == 0, �� ������ �������
	uint8_t i, feedback_bit;
	
	i = 8;
	while(i--){
		feedback_bit = (crc ^ data) & 0x01;
		if(feedback_bit == 0x01) crc = crc ^ CRC8POLY;
		crc = (crc >> 1) & 0x7F;
		if(feedback_bit == 0x01) crc = crc | 0x80;
		data >>= 1;
	}
	return crc;
}
void DS18B20_SetResolution(uint8_t numBit){	//��������� ���������� �������, �� ��������� ������������ ���������� 12 ���
	DS18B20_init();
	DS18B20_send(DS18B20_SKIP_ROM);
	DS18B20_send(DS18B20_W_SCRATCHPAD);
	DS18B20_send(0xFF);
	DS18B20_send(0xFF);
	DS18B20_send(numBit);

	DS18B20_init();
	DS18B20_send(DS18B20_SKIP_ROM);
	DS18B20_send(DS18B20_C_SCRATCHPAD);
}
void DS18B20_convertTemp(void){	//�������� ������� �������������� �����������.
	if(DS18B20_init() != 0){STATUS_DS = 0; return;}
	DS18B20_send(DS18B20_SKIP_ROM);
	DS18B20_send(DS18B20_CONVERT_T);
}
void DS18B20_getTemp(void){	//������, ��������� � ���������� �����������
	if(DS18B20_init() != 0){STATUS_DS = 0; return;}		// error �������������
	
	DS18B20_send(DS18B20_SKIP_ROM);
	DS18B20_send(DS18B20_R_SCRATCHPAD);
	if(DS18B20_read() != 0){STATUS_DS = 0; return;}		// error CRC
	
	uint16_t t = (DS_TempData[1] << 8) + DS_TempData[0];
	DS_Temp = (t & 0x07FF) >> 4;
	DS_Temp += (t & 0x000F) * 0.0625;
	if(t & 0x8000) DS_Temp *= -1;
	STATUS_DS = 1;
}
//===============================================================================
void PWM_send(uint8_t numPWM, uint8_t level){
	PWM_sendByte(numPWM);
	PWM_sendByte(level);
	PWM_sendByte((numPWM + level));
	_delay_ms(10);
}
void PWM_sendByte(uint8_t data){
	uint8_t i;
	
	i = 8;
	while(i--){
		if(data & 0x80) SetBit(PortPWM,PinPWM_MOSI); else ClrBit(PortPWM,PinPWM_MOSI);
		_delay_ms(1);	
		SetBit(PortPWM,PinPWM_SCK);
		_delay_ms(1);	
		ClrBit(PortPWM,PinPWM_SCK);
		data <<= 1;
		_delay_ms(1);	
	}
	_delay_ms(5);
}
//===============================================================================

 
// char GetHex(uint8_t b){
	// switch(b){
		// case 0: return '0';
		// case 1: return '1';
		// case 2: return '2';
		// case 3: return '3';
		// case 4: return '4';
		// case 5: return '5';
		// case 6: return '6';
		// case 7: return '7';
		// case 8: return '8';
		// case 9: return '9';
		// case 10: return 'A';
		// case 11: return 'B';
		// case 12: return 'C';
		// case 13: return 'D';
		// case 14: return 'E';
		// case 15: return 'F';
	// }
	// return 0;
// }

				




